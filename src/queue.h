#ifndef __QUEUE_H
#define __QUEUE_H
#include "common.h"
class Context;
class DeviceManager;
class Kernel;
class Queue{
private:
	cl_command_queue m_queue;
public:
	Queue(Context& icontext, DeviceManager& idevice_manager);
	cl_command_queue get(void);
	cl_int enqueue_ndrange(Kernel& kernel, cl_uint ndim, size_t *global_work_size, size_t *local_work_size, bool iblock=true);
};
#endif
