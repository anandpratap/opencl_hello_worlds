#ifndef __CONTEXT_H
#define __CONTEXT_H
#include "common.h"
#include "device.h"
class DeviceManager;
class Context{
private:
	DeviceManager& m_device_manager;
	cl_context m_context;
public:
	Context(DeviceManager& idevice_manager);
	cl_context get(void);
};

#endif
