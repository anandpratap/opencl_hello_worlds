#ifndef __DEVICE_H
#define __DEVICE_H
#include "common.h"
#include "context.h"

class DeviceManager{
private:
	cl_platform_id m_platform;
	std::vector<cl_device_id> m_devices;
public:
	DeviceManager();
	size_t ndevices(void);
	cl_device_id* devices(void);
};

#endif
