#ifndef __CONTEXT_H
#define __CONTEXT_H
#include "common.h"
#include "device.h"
class DeviceManager;
class Context{
private:
	DeviceManager& m_device_manager;
	cl_context m_context;
	std::list<cl_mem> m_mem_objects;
public:
	Context(DeviceManager& idevice_manager);
	~Context();
	cl_context get(void);
	cl_mem create_buffer(cl_mem_flags flags, size_t size, void *host_ptr);
	void release_mem(cl_mem mem_object);
};

#endif
