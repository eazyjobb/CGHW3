﻿#include "global_utility.h"

GLFWwindow* window;

int main()
{
	//std::ios::sync_with_stdio(false);
	//初始化
	if (full_initial(window, window_width, window_height) == -1) {
		std::cout << "INITIAL FAILED" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}

	//消息循环
	while (!glfwWindowShouldClose(window))
	{
		time_system::refresh();
		GLfloat pre = time_system::curframe_time;

		glfwPollEvents();

		callback::handle_events(window);

		//清空画布
		//GL_DEPTH_BUFFER_BIT GL_STENCIL_BUFFER_BIT

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//渲染

		//tester1::refresh();
		//tester2::refresh();
		//uni_tester::refresh();
		//texture_tester::refresh();
		coord_tester::refresh();

		//交换缓冲区域
		glfwSwapBuffers(window);

		while (((GLfloat)glfwGetTime() - pre) < 1.0f / 60.0f);
	}

	//tester1::release();
	//tester2::release();
	//uni_tester::release();
	//texture_tester::release();
	coord_tester::release();

	//结束，释放内存
	glfwTerminate();

	return 0;
}
