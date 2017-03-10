#ifndef __CONTEXT_CPP
#define __CONTEXT_CPP
#include "context.h"

Context::Context(DeviceManager& idevice_manager): m_device_manager(idevice_manager){
	cl_int status;
	m_context = clCreateContext(nullptr, m_device_manager.ndevices(), m_device_manager.devices(), nullptr, nullptr, &status);
	check_error(status, "Cannot create context.");
	spd::get("console")->info("Created context");
};

cl_context Context::get(void){
	return m_context;
};

#endif
