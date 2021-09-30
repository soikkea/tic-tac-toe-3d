#pragma once

#include <vector>
#include <raylib.h>

namespace util {
	struct Triangle
	{
		Vector3 p[3];

		bool hide_edge_1 = false;
		bool hide_edge_2 = false;
		bool hide_edge_3 = false;
	};

	struct Mesh
	{
		std::vector<Triangle> tris;
	};

	Vector3 MultiplyMatrixVector3(Vector3& const vec, Matrix& const mat);

	Matrix MakeProjectionMatrix(float fov, float aspectRatio, float fNear, float fFar);

	int TriangleClipAgainstPlane(Vector3 planePoint, Vector3 planeNormal, Triangle& triIn, Triangle& triOut1, Triangle& triOut2);

	Vector3 Vector3IntersectPlane(Vector3& planePoint, Vector3& planeNormal, Vector3& lineStart, Vector3 lineEnd);
}