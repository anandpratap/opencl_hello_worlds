#ifndef __DEVICE_CPP
#define __DEVICE_CPP
#include "device.h"
#include "kernel.h"
DeviceManager::DeviceManager(): m_platform(nullptr){
	cl_int status;

#if defined(USE_ALTERA_SDK)
	m_platform = findPlatform("Altera SDK for OpenCL");
#else
	m_platform = findPlatform("Intel(R) FPGA");
#endif
		check_null(m_platform, "Unable to find Intel(R) FPGA OpenCL platform!");

		auto console = spd::get("console");
		console->info("== Platform info ==");
		char buffer[1024];
		clGetPlatformInfo(m_platform, CL_PLATFORM_NAME, 1024, buffer, nullptr);
		console->info("Platform name: \t{}", buffer);

		clGetPlatformInfo(m_platform, CL_PLATFORM_VENDOR, 1024, buffer, nullptr);
		console->info("Platform vendor: \t{}", buffer);

		clGetPlatformInfo(m_platform, CL_PLATFORM_VERSION, 1024, buffer, nullptr);
		console->info("Platform version: \t{}", buffer);

		
		cl_uint num_devices;
		clGetDeviceIDs(m_platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices);
		console->info("Number of devices: \t{}", num_devices);
		m_devices = std::vector<cl_device_id>(num_devices);
		clGetDeviceIDs(m_platform, CL_DEVICE_TYPE_ALL, num_devices, m_devices.data(), nullptr);

		auto device = m_devices[0];
		clGetDeviceInfo(device, CL_DEVICE_NAME, 1024, buffer, nullptr);
		console->info("Device name: \t{}", buffer);

};

size_t DeviceManager::ndevices(void){
	return m_devices.size();
}

cl_device_id* DeviceManager::devices(void){
	return m_devices.data();
}

#endif
