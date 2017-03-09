#include "src/api.h"

int main(int argc, char **argv){
	spd::set_pattern("logger: %v");
	auto console = spd::stdout_color_mt("console");
	console->info("This is the host program!");
	auto device_manager = DeviceManager();
	auto kernel = Kernel("hello_world.aocx");
	auto context = Context(device_manager);
	kernel.create_program(context, device_manager);
	auto queue = Queue(context, device_manager);

	cl_uint ndim = 1;
	size_t global_work_size[] = {3, 1, 1};
	size_t local_work_size[] = {3, 1, 1};
	bool block = true;
	queue.enqueue_ndrange(kernel, ndim, global_work_size, local_work_size, block);
	queue.enqueue_ndrange(kernel, ndim, global_work_size, local_work_size, block);
	return 0;
}
