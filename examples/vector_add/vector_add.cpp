#include "src/api.h"
#include <cmath>
float rand_float() {
	return float(rand()) / float(RAND_MAX) * 20.0f - 10.0f;
}

int main(int argc, char **argv){
	const size_t size = 10;

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
	x_buffer = clCreateBuffer(context.get(), CL_MEM_READ_ONLY, size*sizeof(float), nullptr, &status);
	check_error(status, "Cannot create x_buffer.");
	y_buffer = clCreateBuffer(context.get(), CL_MEM_READ_ONLY, size*sizeof(float), nullptr, &status);
	check_error(status, "Cannot create y_buffer.");
	z_buffer = clCreateBuffer(context.get(), CL_MEM_WRITE_ONLY, size*sizeof(float), nullptr, &status);
	check_error(status, "Cannot create z_buffer.");

	cl_event write_event[2];
	status = clEnqueueWriteBuffer(queue.get(), x_buffer, CL_FALSE, 0, size*sizeof(float), x_host.data(), 0, nullptr, &write_event[0]);
	check_error(status, "Cannot write to x_buffer.");

	status = clEnqueueWriteBuffer(queue.get(), y_buffer, CL_FALSE, 0, size*sizeof(float), y_host.data(), 0, nullptr, &write_event[1]);
	check_error(status, "Cannot write to y_buffer.");


	status = clSetKernelArg(kernel.get(), 0, sizeof(cl_mem), &x_buffer);
	check_error(status, "Cannot set arg 0.");
	status = clSetKernelArg(kernel.get(), 1, sizeof(cl_mem), &y_buffer);
	check_error(status, "Cannot set arg 1.");
	status = clSetKernelArg(kernel.get(), 2, sizeof(cl_mem), &z_buffer);
	check_error(status, "Cannot set arg 2.");

	
	cl_uint ndim = 1;
	size_t global_work_size[] = {size, 1, 1};
	//size_t local_work_size[] = {1, 1, 1};
	
	bool block = true;
	cl_event kernel_event;
	cl_event finish_event;
	status = clEnqueueNDRangeKernel(queue.get(), kernel.get(), 1, nullptr, global_work_size, nullptr, 2, write_event, &kernel_event);
	check_error(status, "Cannot enqueue kernel.");
	status = clEnqueueReadBuffer(queue.get(), z_buffer, CL_FALSE, 0, size*sizeof(float), z_host.data(), 1 , &kernel_event, &finish_event);
	clFinish(queue.get());


	float error = 0.0;
	for(int i=0; i<size; i++){
		error += std::pow((z_host[i] - (x_host[i] + y_host[i])), 2);
		console->info("{:f} {:f}", x_host[i]+y_host[i], z_host[i]);
	}
	error = std::sqrt(error);
	console->info("Error = {:f} (should be zero)", error);
	//queue.enqueue_ndrange(kernel, ndim, global_work_size, local_work_size, block);
	//queue.enqueue_ndrange(kernel, ndim, global_work_size, local_work_size, block);
	return 0;
}
