#pragma once

#include <vector>
#include <raylib.h>
#include <string>

namespace util {
	struct Triangle
	{
		Vector3 p[3];

		bool hide_edge_1 = true;
		bool hide_edge_2 = true;
		bool hide_edge_3 = true;

		Color color;
	};

	struct Mesh
	{
		std::vector<Triangle> tris;

		void setColor(Color color);
		void showWireframe(bool enable);
	};

	struct Model
	{
		util::Mesh mesh;
		Vector3 position;
	};

	util::Mesh LoadMeshFromObj(std::string fileName);

	Vector3 MultiplyMatrixVector3(Vector3& const vec, Matrix& const mat);

	Matrix MakeProjectionMatrix(float fov, float aspectRatio, float fNear, float fFar);

	int TriangleClipAgainstPlane(Vector3 planePoint, Vector3 planeNormal, Triangle& triIn, Triangle& triOut1, Triangle& triOut2);

	Vector3 Vector3IntersectPlane(Vector3& planePoint, Vector3& planeNormal, Vector3& lineStart, Vector3 lineEnd);

	util::Mesh MakeUnitCube();

	Matrix BillboardMatrix(const Matrix& viewMatrix, const Vector3& position);
}
