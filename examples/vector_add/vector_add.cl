__kernel void vector_add(__global const float *x,
		__global const float *y,
		__global float *restrict z) {
	size_t idx = get_global_id(0);
	z[idx] = x[idx] + y[idx];
 }

