#include "coord.h"

namespace coord {
	std::unordered_map <std::string, camera> camera_list;
	std::unordered_map <std::string, camera>::iterator current_camera;
	bool swithing;

	const std::unordered_map<std::string, camera>& get_camera_list()
	{
		return camera_list;
	}

	const std::unordered_map<std::string, camera>::iterator & get_current_camera()
	{
		return current_camera;
	}

	const bool & get_swithing()
	{
		return swithing;
	}

	void set_current_camera(const std::string & x) {
		if (camera_list.count(x))
		current_camera = camera_list.find(x);
	}

	int coord_init() {
		swithing = false;
		camera_list.insert(std::make_pair("normal", camera()));
		current_camera = camera_list.find("normal");
		camera_list.insert(std::make_pair("oppsite", camera()));
		camera_list["oppsite"].setYaw(90.0f);
		camera_list["oppsite"].setCameraPos(glm::vec3(0.0f, 0.0f, -3.0f));
		return 0;
	}

	camera::camera() {
		fov = 20.0f; yaw = -90.0f; pitch = 0.0f;
		cameraSpeedZ = 10.0f; cameraSpeedX = 5.0f; cameraSensitivityX = 0.1f; cameraSensitivityY = 0.05f; cameraSensitivityFov = 1.0f;
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

		refresh_camera();
		projection = glm::perspective((GLfloat)glm::radians(fov), (GLfloat)window_width / (GLfloat)window_height, 0.1f, 100.0f);
	}

	void camera::camera_rotate(GLfloat _yaw, GLfloat _pitch) {
		yaw += _yaw * cameraSensitivityX;
		pitch += _pitch * cameraSensitivityY;

		if (pitch > 89.0f)	pitch = 89.0f;
		if (pitch < -89.0f)	pitch = -89.0f;

		refresh_camera();
	}

	void camera::camera_fov(GLfloat delta_fov)
	{
		fov += delta_fov * cameraSensitivityFov;
		if (fov <= 1.0f) fov = 1.0f;
		if (fov >= 45.0f) fov = 45.0f;
		projection = glm::perspective((GLfloat)glm::radians(fov), (GLfloat)window_width / (GLfloat)window_height, 0.1f, 100.0f);
	}

	void camera::camera_front(GLfloat movement) {
		cameraPos += cameraSpeedZ * movement * cameraFront;
		refresh_view();
	}

	void camera::camera_back(GLfloat movement) {
		cameraPos -= cameraSpeedZ * movement * cameraFront;
		refresh_view();
	}

	void camera::camera_left(GLfloat movement) {
		cameraPos -= cameraSpeedX * movement * cameraRight;
		refresh_view();
	}

	void camera::camera_right(GLfloat movement) {
		cameraPos += cameraSpeedX * movement * cameraRight;
		refresh_view();
	}

	void camera::refresh_view() {
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	const glm::vec3 &camera::getCameraPos() const
	{
		return cameraPos;
	}

	const glm::vec3 &camera::getCameraFront() const
	{
		return cameraFront;
	}

	const glm::vec3 &camera::getCameraUp() const
	{
		return cameraUp;
	}

	const glm::vec3 &camera::getCameraRight() const
	{
		return cameraRight;
	}

	const glm::mat4 &camera::getView() const
	{
		return view;
	}

	const glm::mat4 &camera::getProjection() const
	{
		return projection;
	}

	void camera::setYaw(GLfloat x)
	{
		yaw = x;
		refresh_camera();
	}

	void camera::setPitch(GLfloat x)
	{
		if (-89.0f <= x && x <= 89.0f)
			pitch = x;
		refresh_camera();
	}

	void camera::setFov(GLfloat x)
	{
		if (1.0f <= x && x <= 45.0f)
			fov = x;
		refresh_camera();
	}

	void camera::setCameraPos(glm::vec3 x) {
		cameraPos = x;
		refresh_view();
	}

	void camera::Bind(GLuint program, const char * _view, const char * _proj, const char * _viewPos) const
	{
		glUniform3f(glGetUniformLocation(program, _viewPos), cameraPos.x, cameraPos.y, cameraPos.z);
		glUniformMatrix4fv(glGetUniformLocation(program, _view), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(program, _proj), 1, GL_FALSE, glm::value_ptr(projection));
	}

	void camera::refresh_camera() {
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch + 90.0f));
		front.y = sin(glm::radians(pitch + 90.0f));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch + 90.0f));
		cameraUp = glm::normalize(front);
		cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
		refresh_view();
	}

}
