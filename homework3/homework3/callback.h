﻿#pragma once

#include "std_header.h"
#include "data_binding.h"
#include "coord.h"

namespace callback {
	extern bool keys[1024];
	extern bool first_mouse;
	extern bool mouse_keys[8];
	extern GLfloat mouse_last_x, mouse_last_y;
	/*
		键盘回调函数
	*/
	int callback_init();
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mouse_button_callback(GLFWwindow* window, int key, int action, int mode);
	void handle_events(GLFWwindow*);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}
