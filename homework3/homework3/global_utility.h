#pragma once

#define GLEW_STATIC
#include "std_header.h"
#include "callback.h"
#include "data_binding.h"

#define INIT_ERROR 1

/*
	int full_initial(GLFWwindow* &window, int _width, int _height)
		�½����ڣ������ز�

		return 0 �ɹ�
		return -1 ʧ��
*/
int full_initial(GLFWwindow* &window, int _width, int _height);