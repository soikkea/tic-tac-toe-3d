#pragma once

#include <vector>
#include <raylib.h>

namespace util {
	struct Triangle
	{
		Vector3 p[3];
	};

	struct Mesh
	{
		std::vector<Triangle> tris;
	};

	Vector3 MultiplyMatrixVector3(Vector3& const vec, Matrix& const mat);
}
