#pragma once
#include "std_header.h"

//开启帧数检测
#define fps_tester

namespace time_system {
	//全局初始化时间系统
	int time_system_init();

	//获取帧数间的时间变动
	GLfloat delta_time();

	//获取当前帧时间点
	const GLfloat &get_preframe_time();
	//获取上一帧时间点
	const GLfloat &get_curframe_time();

	// 刷新帧数timer，请勿在消息循环外使用
	void refresh();
}
