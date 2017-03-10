#ifndef __KERNEL_H
#define __KERNEL_H
#include "common.h"
#include "context.h"
#include "device.h"

class Kernel{
private:
	std::string m_filename;
	size_t m_size;
	std::vector<char> m_buffer;
	cl_program m_program;
	cl_kernel m_kernel;
public:
	Kernel(std::string ifilename, bool iload = true);
	void load(void);
	size_t size(void);
	unsigned char* data(void);
	cl_kernel get(void);
	void create_program(Context& context, DeviceManager& device_manager);
	cl_int set_arg(cl_uint arg_index, size_t arg_size, const void *arg_value);
};
#endif
