#pragma once

#define GLEW_STATIC
#include "std_header.h"
#include "callback.h"
#include "data_binding.h"
#include "shader.h"

/*
	void full_initial(GLFWwindow* &window, int _width, int _height)
		新建窗口，载入素材
*/
int full_initial(GLFWwindow* &window, int _width, int _height);

/*
	将字符串分割
*/
std::vector<std::string> split(const std::string &, const char &);