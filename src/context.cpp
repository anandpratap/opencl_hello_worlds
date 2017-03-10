#ifndef __CONTEXT_CPP
#define __CONTEXT_CPP
#include "context.h"

Context::Context(DeviceManager& idevice_manager): m_device_manager(idevice_manager), m_mem_objects(0){
	cl_int status;
	m_context = clCreateContext(nullptr, m_device_manager.ndevices(), m_device_manager.devices(), nullptr, nullptr, &status);
	check_error(status, "Cannot create context.");
	spd::get("console")->info("Created context");
};

Context::~Context(){
	for(auto mem_object : m_mem_objects){
		auto status = clReleaseMemObject(mem_object);
		auto error_string = fmt::format("Cannot release mem object");
		check_error(status, error_string);
	}
	
	clReleaseContext(get());
};

cl_context Context::get(void){
	return m_context;
};

cl_mem Context::create_buffer(cl_mem_flags flags, size_t size, void *host_ptr){
	cl_int status;
	auto buffer = clCreateBuffer(get(), flags, size, host_ptr, &status);
	auto error_string = fmt::format("Cannot create buffer");
	check_error(status, error_string);
	m_mem_objects.push_back(buffer);
	return buffer;
};

void Context::release_mem(cl_mem mem_object){
	auto status = clReleaseMemObject(mem_object);
	auto error_string = fmt::format("Cannot release mem object");
	check_error(status, error_string);
	m_mem_objects.remove(mem_object);
}

#endif
