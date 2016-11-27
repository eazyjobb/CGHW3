#pragma once
#include "std_header.h"

namespace time_system {
	extern GLfloat preframe_time, curframe_time;

	int time_system_init();
	GLfloat delta_time();
	void refresh();
}
