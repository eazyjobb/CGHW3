#pragma once
#include "std_header.h"

namespace time_system {
	int time_system_init();
	GLfloat delta_time();
	const GLfloat &get_preframe_time();
	const GLfloat &get_curframe_time();
	void refresh();
}
