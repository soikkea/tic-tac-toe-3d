#pragma once
#include "Utility3d.h"

class Window
{
private:
	util::Mesh cube_;
	Matrix projection_matrix_;
	Vector3 camera_;
	Vector3 lookDir_;
	int screen_width_;
	int screen_height_;

	float theta_;
	float cameraYaw_;
	float cameraPitch_;
public:
	Window();
	void Open();
};

