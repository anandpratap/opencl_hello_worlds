#ifndef __KERNEL_CPP
#define __KERNEL_CPP
#include "kernel.h"

Kernel::Kernel(std::string ifilename, bool iload): m_filename(ifilename){
	if(iload)
		load();
};
void Kernel::load(void){
	std::ifstream kernel_file(m_filename, std::ios::in | std::ios::binary);
	m_buffer = std::vector<char>((std::istreambuf_iterator<char>(kernel_file)),
								 (std::istreambuf_iterator<char>()));
	m_size = m_buffer.size();
};
size_t Kernel::size(void){
	return m_size;
};
unsigned char* Kernel::data(void){
	return reinterpret_cast<unsigned char*>(m_buffer.data());
};
void Kernel::create_program(Context& context, DeviceManager& device_manager){
	cl_int status;
	unsigned char* binaries[] = {data()};
	size_t binaries_size[] = {size()};
	
	std::vector<cl_int> binary_status(device_manager.ndevices());
	m_program = clCreateProgramWithBinary(context.context(), device_manager.ndevices(),
										  device_manager.devices(), binaries_size,
										  (const unsigned char **) binaries, binary_status.data(), &status);
	check_error(status, "Failed to create program with binary");
	
	status = clBuildProgram(m_program, 0, NULL, "", NULL, NULL);
	check_error(status, "Failed to build program");
	
	const char *name = "hello_world";
	m_kernel = clCreateKernel(m_program, name, &status);
		check_error(status, "Failed to create kernel");
};

cl_kernel Kernel::get(void){
	return m_kernel;
}

#endif
