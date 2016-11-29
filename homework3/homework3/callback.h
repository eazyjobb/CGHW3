#pragma once

#include "std_header.h"
#include "data_binding.h"
#include "coord.h"

namespace callback {
	//全局回调初始化
	int callback_init();
	//键盘回调
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	//鼠标按键
	void mouse_button_callback(GLFWwindow* window, int key, int action, int mode);
	//事件处理
	void handle_events(GLFWwindow*);
	//鼠标移动回调
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	//鼠标滚轮回调
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}
