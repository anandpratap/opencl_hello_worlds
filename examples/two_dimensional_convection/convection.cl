#define nx 100
#define ny 100
__kernel void convection(__global float * restrict input, __global float * restrict  output){
	float fac = 0.2f;
	float output_cache[nx*ny], input_cache[nx*ny];

#pragma unroll
	for(int i=0; i<nx*ny; i++){
		input_cache[i] = input[i];
	}

#pragma unroll
	for(int i=0; i<nx; i++){
		output_cache[i*ny] = input_cache[i*ny];
	}

#pragma unroll
	for(int j=0; j<ny; j++){
		output_cache[j] = input_cache[j];
	}

#pragma unroll
	for(int i=1; i<nx; i++){
		for(int j=1; j<ny; j++){
			output_cache[i*ny + j] = input_cache[i*ny + j] - ((input_cache[i*ny+j] - input_cache[(i-1)*ny+j]) + (input_cache[i*ny+j] - input_cache[i*ny + j-1]))*fac;
		}
	}

#pragma unroll
	for(int i=0; i<nx*ny; i++){
		output[i] = output_cache[i];
	}

}
