#include "src/api.h"
#include <cmath>

class Convection{
public:
	const int m_nx, m_ny;
	float *x, *y, *u, *u2, *u_cl;
	Convection(int nx, int ny): m_nx(nx), m_ny(ny){
		auto size = nx*ny;
		x = new float[size];
		y = new float[size];
		u = new float[size];
		u_cl = new float[size];
		u2 = new float[size];
		setup();
	};
	void setup(){
		const auto dx = 2.0/(m_nx-1);
		const auto dy = 2.0/(m_ny-1);
		for(int i=0; i<m_nx; i++){
			for(int j=0; j<m_ny; j++){
				auto xl = x[i*m_ny + j] = -1 + i*dx;
				auto yl = y[i*m_ny + j] = -1 + j*dx;
				auto dist = std::sqrt(xl*xl + yl*yl);
				auto r = 0.2;
				u[i*m_ny + j] = std::max(1 - dist/r, 0.0);
			}
		}
	};
	void write_solution(){
		std::string filename = "solution.npz";
		const unsigned int shape[] = {static_cast<unsigned int>(m_nx), static_cast<unsigned int>(m_ny)};
		cnpy::npz_save(filename,"x",x,shape,2,"w");
		cnpy::npz_save(filename,"y",y,shape,2,"a");
		cnpy::npz_save(filename,"u",u,shape,2,"a");
		cnpy::npz_save(filename,"u_cl",u_cl,shape,2,"a");
	};

	void step(const int nx, const int ny, float *iu, float *iu2){
        float fac = 0.2;
        for(int i=0; i<nx; i++){
                iu2[i*ny] = iu[i*ny];
        }

        for(int j=0; j<ny; j++){
                iu2[j] = iu[j];
        }

        for(int i=1; i<nx; i++){
			for(int j=1; j<ny; j++){
					iu2[i*ny + j] = iu[i*ny + j] - fac* (iu[i*ny+j] - iu[(i-1)*ny+j]) - fac*(iu[i*ny+j] - iu[i*ny + j-1]);
                }
        }

	}
	void run(int nstep){
		for(int i=0; i<nstep; i++){
			if(i%2 == 0){
				step(m_nx, m_ny, u, u2);
			}
			else{
				step(m_nx, m_ny, u2, u);
			}
		}

		if(nstep%2 != 0){
			float *tmp;
			tmp = u;
			u = u2;
			u2 = tmp;
		}
	}
};

int main(int argc, char **argv){
	spd::set_pattern("logger: %v");
	auto console = spd::stdout_color_mt("console");
	auto timer = Timer();
	auto convection = Convection(100, 100);
	int nstep = 100;
	/* opencl stuff */
	auto device_manager = DeviceManager();
	auto context = Context(device_manager);

	auto kernel = Kernel("convection.aocx");
	kernel.create_program(context, device_manager);

	auto queue = Queue(context, device_manager);

	auto size = convection.m_nx * convection.m_ny;

	cl_mem u_buffer, u2_buffer;
	u_buffer = context.create_buffer(CL_MEM_READ_WRITE, size*sizeof(float), nullptr);
	u2_buffer = context.create_buffer(CL_MEM_READ_WRITE, size*sizeof(float), nullptr);
	cl_event event_copy;
	queue.write_buffer(u_buffer, CL_FALSE, 0, size*sizeof(float), convection.u, 0, nullptr, &event_copy);

	/* ---- */
	queue.finish();
	cl_uint work_dim = 1;
	size_t global_work_size[] = {1, 1, 1};
	auto device_timer = Timer();
	for(int i=0; i<nstep; i++){
		if(i%2 == 0){
			kernel.set_arg(0, sizeof(cl_mem), &u_buffer);
			kernel.set_arg(1, sizeof(cl_mem), &u2_buffer);
		}

		else{
			kernel.set_arg(1, sizeof(cl_mem), &u_buffer);
			kernel.set_arg(0, sizeof(cl_mem), &u2_buffer);
		}
		queue.ndrange_kernel(kernel.get(), work_dim, nullptr, global_work_size, nullptr, 0, nullptr, nullptr);
	}
	queue.finish();
	console->info("Device time: {}s", device_timer.diff());
	cl_mem solution_buffer = nstep%2 == 0 ? u_buffer : u2_buffer;
	queue.read_buffer(solution_buffer, CL_FALSE, 0, size*sizeof(float), convection.u_cl, 0, nullptr, nullptr);
	queue.finish();

	auto host_timer = Timer();
	convection.run(nstep);
	console->info("Host time: {}s", host_timer.diff());

	convection.write_solution();




	return 0;
}
