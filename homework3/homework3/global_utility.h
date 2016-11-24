#pragma once

#define GLEW_STATIC
#include "std_header.h"
#include "callback.h"
#include "data_binding.h"

#define INIT_ERROR 1

/*
	int full_initial(GLFWwindow* &window, int _width, int _height)
		新建窗口，载入素材

		return 0 成功
		return -1 失败
*/
int full_initial(GLFWwindow* &window, int _width, int _height);