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

cl_int Queue::enqueue_ndrange(Kernel& kernel, cl_uint ndim, size_t *global_work_size, size_t *local_work_size, bool iblock){
	cl_int status;
	status = clEnqueueNDRangeKernel(m_queue, kernel.get(), ndim, nullptr, global_work_size, local_work_size, 0, nullptr, nullptr);
	check_error(status, "Failed to launch kernel");
	// Wait for command queue to complete pending events
	if(iblock){
		status = clFinish(m_queue);
		check_error(status, "Finish");
	}
};
#endif
