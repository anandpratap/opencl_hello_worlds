__kernel void convection(const int nx, const int ny, __global float *iu, __global float *iu2){
	float fac = 0.2;

#pragma unroll
	for(int i=0; i<nx; i++){
		iu2[i*ny] = iu[i*ny];
	}

#pragma unroll
	for(int j=0; j<ny; j++){
		iu2[j] = iu[j];
	}

#pragma unroll
	for(int i=1; i<nx; i++){
		for(int j=1; j<ny; j++){
			iu2[i*ny + j] = iu[i*ny + j] - fac* (iu[i*ny+j] - iu[(i-1)*ny+j]) - fac*(iu[i*ny+j] - iu[i*ny + j-1]);
		}
	}
}
