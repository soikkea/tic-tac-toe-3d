#pragma once
#include "Utility3d.h"
#include <map>
#include <string>
#include "Vec3.h"
#include <set>
#include "Game.h"

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

	enum class Direction {FORWARD = 0, RIGHT, BACK, LEFT};

	const static std::map<Direction, Vec3> directionMap_;

	std::map<std::string, util::Model> models_;
	std::map<std::string, util::Mesh> meshes_;

	std::set<std::string> selectedCubes_;

	Game game_;

	void SelectCube(const Vec3& pos);
	std::string getCubeIdString(const Vec3& pos);
	bool useCube(const Vec3& pos);
public:
	Window();
	void Open();
};

