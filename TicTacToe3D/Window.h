#pragma once
#include "Utility3d.h"
#include <map>
#include <string>
#include "Vec3.h"
#include <set>

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
	float cameraDistance_;

	Vec3 selectedCube_;

	std::map<std::string, util::Model> models_;

	std::set<std::string> selectedCubes_;

	void SelectCube(const Vec3& pos);
	std::string getCubeIdString(const Vec3& pos);
	bool useCube(const Vec3& pos);
public:
	Window();
	void Open();
};

