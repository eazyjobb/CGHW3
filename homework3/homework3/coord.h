#pragma once
#include "std_header.h"
#include "time_system.h"

namespace coord {
	//无 roll 角 fps摄像机
	class camera {
	private:
		// 箭头方向与欧拉角
		glm::vec3 cameraPos, cameraFront, cameraUp, cameraRight;
		GLfloat yaw, pitch, fov;
		// 投影矩阵，视野矩阵
		glm::mat4 view, projection;
		// 内部刷新镜头向量
		void refresh_camera();
		// 内部刷新视野矩阵
		void refresh_view();
	public:
		//Speed 为移动速度，Sensitivity为镜头灵敏度
		GLfloat cameraSpeedZ, cameraSpeedX, cameraSensitivityX, cameraSensitivityY, cameraSensitivityFov;

		//默认视野为
		//
		//fov = 20.0f; yaw = -90.0f; pitch = 0.0f;
		//cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

		camera();

		//镜头旋转
		void camera_rotate(GLfloat delta_yaw, GLfloat delta_pitch);
		//镜头Fov
		void camera_fov(GLfloat delta_fov);
		//前后左右移动
		void camera_front(GLfloat movement);
		void camera_back(GLfloat movement);
		void camera_left(GLfloat movement);
		void camera_right(GLfloat movement);
		//获取镜头位置
		const glm::vec3 &getCameraPos() const;
		//获取镜头方向
		const glm::vec3 &getCameraFront() const;
		const glm::vec3 &getCameraUp() const;
		const glm::vec3 &getCameraRight() const;
		//获取视野，投影矩阵
		const glm::mat4 &getView() const;
		const glm::mat4 &getProjection() const;
		//直接修改镜头接口，内部带有限制，会保证内部变量统一性，请查看实现
		void setYaw(GLfloat);
		void setPitch(GLfloat);
		void setFov(GLfloat);
		void setCameraPos(glm::vec3);
	};

	//全局镜头列表
	const std::unordered_map <std::string, camera> &get_camera_list();
	//当前镜头指针
	const std::unordered_map <std::string, camera>::iterator &get_current_camera();
	//镜头切换
	//不存在，则会变为默认镜头
	void set_current_camera(const std::string &);
	//镜头是否切换中
	const bool &get_swithing();
	//全局镜头初始化
	int coord_init();
}
