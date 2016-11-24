#include "global_utility.h"

int full_initial(GLFWwindow* &window, int _width, int _height) {
	/*
	���ڳ�ʼ��
	*/

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(_width, _height, "LearnOpenGL", nullptr, nullptr);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return INIT_ERROR;
	}

	glfwMakeContextCurrent(window);

	/*
	GLEW ��ʼ��
	*/

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return INIT_ERROR;
	}

	/*
	�ص�������
	*/

	glfwSetKeyCallback(window, key_callback);

	/*
		�زĶ���
	*/

	tester1::tester1_init();

	return 0;
}