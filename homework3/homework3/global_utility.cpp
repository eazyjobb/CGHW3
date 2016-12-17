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
	glfwWindowHint(GLFW_SAMPLES, 4);

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

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	/*
	回调函数绑定
	*/

	glfwSetKeyCallback(window, callback::key_callback);
	glfwSetMouseButtonCallback(window, callback::mouse_button_callback);
	glfwSetCursorPosCallback(window, callback::mouse_callback);
	glfwSetScrollCallback(window, callback::scroll_callback);

	/*
		素材读入
	*/

	if (callback::callback_init())
		return -1;

	if (shader::shader_init())
		return -1;

	if (texture::texture_init())
		return -1;

	if (coord::coord_init())
		return -1;

	if (time_system::time_system_init())
		return -1;

	if (light::init())
		return -1;

	//coord_tester::init();
	//light_tester::init();

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

void assignment(glm::mat4 & a, const aiMatrix4x4 & b) {
	//memcpy(&a, &b, sizeof(glm::mat4));
	for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) a[j][i] = b[i][j];
	
	//for debug
	//for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) if (a[i][j] != b[i][j]) assert(0);
}

void assignment(aiMatrix4x4 & a, const glm::mat4 & b) {
	//memcpy(&a, &b, sizeof(glm::mat4));
	for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) a[j][i] = b[i][j];
	//for debug
	//for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) if (a[i][j] != b[i][j]) assert(0);
}

/*
void assignment(glm::mat4 & a, const aiMatrix3x3 & b) {
	memset(&a, 0, sizeof(a));
	for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) a[j][i] = b[i][j];
	//for debug 
	for (int i = 0; i < 3; ++i) for (int j = 0; j <3 ; ++j) if (a[i][j] != b[i][j]) assert(0);
}
*/