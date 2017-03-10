#include "src/api.h"
#include <cmath>
float rand_float() {
	return float(rand()) / float(RAND_MAX) * 20.0f - 10.0f;
}

int main(int argc, char **argv){
	const size_t size = 100000;

	std::vector<float>x_host(size), y_host(size), z_host(size);
	for(size_t i=0; i<size; i++){
		x_host[i] = rand_float();
		y_host[i] = rand_float();
	}

	
	spd::set_pattern("logger: %v");
	auto console = spd::stdout_color_mt("console");
	console->info("This is the host program!");
	auto device_manager = DeviceManager();
	auto kernel = Kernel("vector_add.aocx");
	auto context = Context(device_manager);
	kernel.create_program(context, device_manager);
	auto queue = Queue(context, device_manager);


	cl_mem x_buffer, y_buffer, z_buffer;
	cl_int status;
	x_buffer = context.create_buffer(CL_MEM_READ_ONLY, size*sizeof(float), nullptr);
	y_buffer = context.create_buffer(CL_MEM_READ_ONLY, size*sizeof(float), nullptr);
	z_buffer = context.create_buffer(CL_MEM_WRITE_ONLY, size*sizeof(float), nullptr);

	cl_event write_event[2];
	queue.write_buffer(x_buffer, CL_FALSE, 0, size*sizeof(float), x_host.data(), 0, nullptr, &write_event[0]);
	queue.write_buffer(y_buffer, CL_FALSE, 0, size*sizeof(float), y_host.data(), 0, nullptr, &write_event[1]);
	

	kernel.set_arg(0, sizeof(cl_mem), &x_buffer);
	kernel.set_arg(1, sizeof(cl_mem), &y_buffer);
	kernel.set_arg(2, sizeof(cl_mem), &z_buffer);

	
	cl_uint work_dim = 1;
	size_t global_work_size[] = {size, 1, 1};

	cl_event kernel_event;
	cl_event finish_event;

	queue.ndrange_kernel(kernel.get(), work_dim, nullptr, global_work_size, nullptr, 2, write_event, &kernel_event);
	queue.read_buffer(z_buffer, CL_FALSE, 0, size*sizeof(float), z_host.data(), 1 , &kernel_event, &finish_event);
	queue.finish();


	float error = 0.0;
	for(int i=0; i<size; i++){
		error += std::pow((z_host[i] - (x_host[i] + y_host[i])), 2);
	}
	error = std::sqrt(error);
	console->info("Error = {:f} (should be zero)", error);
	return 0;
}
