#ifndef __COMMON_H
#define __COMMON_H

#include <assert.h>
#include <exception>
#include <fstream>
#include <iterator>
#include <vector>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
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


class Timer {
public:
    Timer() {
        reset();
    }
    void reset() {
        m_timestamp = std::chrono::high_resolution_clock::now();
    }
    float diff() {
        std::chrono::duration<float> fs = std::chrono::high_resolution_clock::now() - m_timestamp;
        return fs.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_timestamp;
};
#include "utils.h"
#endif
