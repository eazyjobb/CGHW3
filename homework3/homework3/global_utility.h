#pragma once

#define GLEW_STATIC
#include "std_header.h"
#include "callback.h"
#include "data_binding.h"
#include "shader.h"
#include "coord.h"
#include "time_system.h"
#include "model.h"

/*
	void full_initial(GLFWwindow* &window, int _width, int _height)
		新建窗口，载入素材
*/
int full_initial(GLFWwindow* &window, int _width, int _height);

/*
	将字符串分割
*/
std::vector<std::string> split(const std::string &, const char &);
/*
	重载，将 AIMatrix4*4 赋值给 glm::mat4
*/
void assignment(glm::mat4 &a, const aiMatrix4x4 *b);

//void assignment(aiMatrix4x4 &a, const glm::mat4 &b);

//void assignment(glm::mat4 &a, const aiMatrix3x3 &b);

