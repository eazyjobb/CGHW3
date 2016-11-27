#include "global_utility.h"

int full_initial(GLFWwindow* &window, int _width, int _height) {
	/*
	窗口初始化
	*/

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(_width, _height, "Homework 3", nullptr, nullptr);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	/*
	GLEW 初始化
	*/

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	/*
	回调函数绑定
	*/

	glfwSetKeyCallback(window, key_callback);

	/*
		素材读入
	*/

	if (shader::shader_init())
		return -1;

	if (texture::texture_init())
		return -1;

	if (coord::coord_init())
		return -1;
	//tester1::init();
	//tester2::init();
	//uni_tester::init();
	//texture_tester::init();
	coord_tester::init();

	return 0;
}

std::vector<std::string> split(const std::string &str, const char &ch) {
	std::string now; std::vector<std::string> ans;
	for (size_t i = 0; i < str.size(); ++ i)
	{
		if (ch == str.at(i))
		{
			ans.push_back(now);
			now = "";
		}
		else
			now += str.at(i);
	}
	ans.push_back(now);
	return ans;
}
