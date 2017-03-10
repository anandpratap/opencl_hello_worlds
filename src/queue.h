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
	void ndrange_kernel(cl_kernel kernel, cl_uint work_dim, const size_t *global_work_offset, const size_t *global_work_size, const size_t *local_work_size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event);
	void write_buffer(cl_mem buffer, cl_bool blocking_write, size_t offset, size_t size, const void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event);
	void read_buffer(cl_mem buffer, cl_bool blocking_read, size_t offset, size_t size, void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event);
	void finish(void);
};
#endif
