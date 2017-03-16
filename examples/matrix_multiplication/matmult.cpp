#include <cmath>
#include "src/api.h"
#include "sizes.h"

float rand_float() {
	return float(rand()) / float(RAND_MAX) * 20.0f - 10.0f;
}

template<class T>
void matmult(T a, T b, T c){
	for(size_t i=0; i<C_H; i++){
		for(size_t j=0; j<C_W; j++){
			c[i*C_W + j] = 0.0;
			for(size_t k=0; k<A_W; k++){
				c[i*C_W + j] += a[i*A_W + k]*b[k*B_W + j];
			}
		}		
	}
}

int main(int argc, char **argv){
	spd::set_pattern("%v");
	auto console = spd::stdout_color_mt("console");
	auto timer = Timer();

	const size_t size_a = A_H*A_W;
	const size_t size_b = B_H*B_W;
	const size_t size_c = C_H*C_W;
	/* generate input arrays */
	std::vector<float>a_host(size_a), b_host(size_b), c_host(size_c), c_cl(size_c);
	for(size_t i=0; i<size_a; i++){
		a_host[i] = rand_float();
	}
	for(size_t i=0; i<size_b;  i++){
		b_host[i] = rand_float();
	}

	matmult<float*>(a_host.data(), b_host.data(), c_host.data());


	auto device_manager = DeviceManager();
	auto context = Context(device_manager);

	auto kernel = Kernel("matmult.aocx");
	kernel.create_program(context, device_manager);

	auto queue = Queue(context, device_manager);
	
	timer.reset();
	/* define and create memory buffers */
	cl_mem a_buffer, b_buffer, c_buffer;
	a_buffer = context.create_buffer(CL_MEM_READ_ONLY, size_a*sizeof(float), nullptr);
	b_buffer = context.create_buffer(CL_MEM_READ_ONLY, size_b*sizeof(float), nullptr);
	c_buffer = context.create_buffer(CL_MEM_WRITE_ONLY, size_c*sizeof(float), nullptr);
	
	/* copy input data from host to device */
	cl_event write_event[2];
	queue.write_buffer(a_buffer, CL_FALSE, 0, size_a*sizeof(float), a_host.data(), 0, nullptr, &write_event[0]);
	queue.write_buffer(b_buffer, CL_FALSE, 0, size_b*sizeof(float), b_host.data(), 0, nullptr, &write_event[1]);
	

	/* set argument for the kernel */
	kernel.set_arg(0, sizeof(cl_mem), &a_buffer);
	kernel.set_arg(1, sizeof(cl_mem), &b_buffer);
	kernel.set_arg(2, sizeof(cl_mem), &c_buffer);

	/* run the kernel, this waits for the write event to finish */
	cl_uint work_dim = 2;
	size_t global_work_size[] = {C_W, C_H};
	size_t local_work_size[] = {BLOCK_SIZE, BLOCK_SIZE};
	cl_event kernel_event;
	queue.ndrange_kernel(kernel.get(), work_dim, nullptr, global_work_size, local_work_size, 2, write_event, &kernel_event);

	/* copy output data from device to host, this waits for the kernel to finish */
	cl_event finish_event;
	queue.read_buffer(c_buffer, CL_FALSE, 0, size_c*sizeof(float), c_cl.data(), 1 , &kernel_event, &finish_event);

	/* wait until all the items in the queue have finish executing */
	queue.finish();
	auto time_total = timer.diff();
	console->info("Total time taken: {}s", time_total);
	/* check the answer */
	float error = 0.0;
	for(int i=0; i<size_c; i++){
		error += std::pow((c_host[i] - c_cl[i]), 2);
	}

	for(int i=0; i<size_c; i++){
		console->info("{} == {}", c_host[i], c_cl[i]);
	}
	
	error = std::sqrt(error);
	console->info("Error = {:f} (should be zero)", error);


}
