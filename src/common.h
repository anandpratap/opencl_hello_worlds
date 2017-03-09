#ifndef __COMMON_H
#define __COMMON_H

#include <assert.h>
#include <exception>
#include <fstream>
#include <iterator>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>
#include "CL/opencl.h"
#include "spdlog/spdlog.h"
namespace spd = spdlog;

template<class T>
void check_null(T x, std::string message){
	if(x == nullptr){
		spd::get("console")->error("{}", message);
		std::terminate();
	};
};
#include "utils.h"
#endif
