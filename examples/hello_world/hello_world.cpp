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

	cl_uint work_dim = 1;
	size_t global_work_size[] = {3, 1, 1};
	queue.ndrange_kernel(kernel.get(), work_dim, nullptr, global_work_size, nullptr, 0, nullptr, nullptr);
	queue.finish();
	return 0;
}
