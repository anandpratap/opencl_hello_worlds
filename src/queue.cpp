#ifndef __QUEUE_CPP
#define __QUEUE_CPP
#include "queue.h"
#include "context.h"
#include "device.h"
#include "kernel.h"
Queue::Queue(Context& icontext, DeviceManager& idevice_manager){
	cl_int status;
	m_queue = clCreateCommandQueueWithProperties(icontext.get(), idevice_manager.devices()[0], 0, &status);
	check_error(status, "Cannot create queue.");
};

cl_command_queue Queue::get(void){
	return m_queue;
};

void Queue::ndrange_kernel(cl_kernel kernel, cl_uint work_dim, const size_t *global_work_offset, const size_t *global_work_size, const size_t *local_work_size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event){
	auto status = clEnqueueNDRangeKernel(get(), kernel, work_dim, global_work_offset, global_work_size, local_work_size, num_events_in_wait_list, event_wait_list, event);
	check_error(status, "Failed to launch kernel");
};

void Queue::write_buffer(cl_mem buffer, cl_bool blocking_write, size_t offset, size_t size, const void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event){
	auto status = clEnqueueWriteBuffer(get(), buffer, blocking_write, offset, size, ptr, num_events_in_wait_list, event_wait_list, event);
	auto error_string = fmt::format("Cannot write to buffer.");
	check_error(status, error_string);
}

void Queue::read_buffer(cl_mem buffer, cl_bool blocking_read, size_t offset, size_t size, void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event){
	auto status = clEnqueueReadBuffer(get(), buffer, blocking_read, offset, size, ptr, num_events_in_wait_list, event_wait_list, event);
	auto error_string = fmt::format("Cannot read buffer.");
	check_error(status, error_string);
}

void Queue::finish(void){
	auto status = clFinish(get());
	check_error(status, "Finish");
}

#endif
