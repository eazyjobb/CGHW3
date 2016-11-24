#include "global_utility.h"

GLFWwindow* window;

int main()
{

	//��ʼ��
	if (full_initial(window, 1200, 600) == INIT_ERROR) {
		system("pause");
		//��ʼ��ʧ�ܣ��ͷ��ڴ�
		glfwTerminate();
		return -1;
	}

	//��Ϣѭ��
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		//��ջ���
		//GL_DEPTH_BUFFER_BIT GL_STENCIL_BUFFER_BIT

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//��Ⱦ

		tester1::tester1_refresh();

		//������������
		glfwSwapBuffers(window);
	}

	//�������ͷ��ڴ�
	glfwTerminate();

	return 0;
}
