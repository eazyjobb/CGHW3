#include "global_utility.h"

GLFWwindow* window;

int main()
{
	//std::ios::sync_with_stdio(false);
	//初始化
	if (full_initial(window, 1000, 1000) == -1) {
		std::cout << "INITIAL FAILED" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}

	//消息循环
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		//清空画布
		//GL_DEPTH_BUFFER_BIT GL_STENCIL_BUFFER_BIT

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//渲染

		tester1::tester1_refresh();

		//交换缓冲区域
		glfwSwapBuffers(window);
	}

	//结束，释放内存
	glfwTerminate();

	return 0;
}
