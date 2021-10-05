#pragma once
#include "Utility3d.h"
#include <map>
#include <string>

class Window
{
private:
	Matrix projection_matrix_;
	Vector3 camera_;
	Vector3 lookDir_;
	int screen_width_;
	int screen_height_;

	float theta_;
	float cameraYaw_;
	float cameraPitch_;

	std::map<std::string, util::Model> models_;
public:
	Window();
	void Open();
};

