#include "Utility3d.h"
#include <raymath.h>
#include <strstream>
#include <fstream>
#include <stdexcept>

util::Mesh util::LoadMeshFromObj(std::string fileName)
{
	auto mesh = util::Mesh();

	std::ifstream f(fileName);
	if (!f.is_open()) {
		throw std::runtime_error(fileName + " not found");
	}

	std::vector<Vector3> verts;

	while (!f.eof())
	{
		char line[128];
		f.getline(line, 128);

		std::strstream s;
		s << line;

		char ignore;

		if (line[0] == 'v') {
			Vector3 v{};
			s >> ignore >> v.x >> v.y >> v.z;
			verts.push_back(v);
		}

		if (line[0] == 'f') {
			int face[3]{};
			s >> ignore >> face[0] >> face[1] >> face[2];
			mesh.tris.push_back({ verts[face[0] - 1], verts[face[1] - 1], verts[face[2] - 1] });
		}
	}

	f.close();

	return mesh;
}

Vector3 util::MultiplyMatrixVector3(Vector3& const vec, Matrix& const mat)
{
	Vector3 result = Vector3Transform(vec, mat);
	float w = vec.x * mat.m3 + vec.y * mat.m7 + vec.z * mat.m11 + mat.m15;

	if (w != 0.0f) {
		result = Vector3Scale(result, 1.0f / w);
	}
	return result;
}

Matrix util::MakeProjectionMatrix(float fov, float aspectRatio, float fNear, float fFar)
{
	float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * PI);

	return Matrix{
		aspectRatio * fovRad, 0.0f, 0.0f, 0.0f,
		0.0f, fovRad, 0.0f, 0.0f,
		0.0f, 0.0f, fFar / (fFar - fNear), 1.0f,
		0.0f, 0.0f, (-fFar * fNear) / (fFar - fNear), 0.0f
	};
}

int util::TriangleClipAgainstPlane(Vector3 planePoint, Vector3 planeNormal, Triangle& triIn, Triangle& triOut1, Triangle& triOut2)
{
	planeNormal = Vector3Normalize(planeNormal);

	auto distFromPlane = [&](Vector3& p)
	{
		return Vector3DotProduct(planeNormal, p) - Vector3DotProduct(planeNormal, planePoint);
	};

	Vector3* insidePoints[3]{};
	int insidePointCount = 0;
	Vector3* outsidePoints[3]{};
	int outsidePointCount = 0;

	float d0 = distFromPlane(triIn.p[0]);
	float d1 = distFromPlane(triIn.p[1]);
	float d2 = distFromPlane(triIn.p[2]);

	auto pointsInside = std::vector<int>{};

	if (d0 >= 0) {
		insidePoints[insidePointCount++] = &triIn.p[0];
		pointsInside.push_back(0);
	}
	else {
		outsidePoints[outsidePointCount++] = &triIn.p[0];
	}
	if (d1 >= 0) {
		insidePoints[insidePointCount++] = &triIn.p[1];
		pointsInside.push_back(1);
	}
	else {
		outsidePoints[outsidePointCount++] = &triIn.p[1];
	}
	if (d2 >= 0) {
		insidePoints[insidePointCount++] = &triIn.p[2];
		pointsInside.push_back(2);
	}
	else {
		outsidePoints[outsidePointCount++] = &triIn.p[2];
	}

	if (insidePointCount == 0) {
		return 0;
	}
	if (insidePointCount == 3) {
		triOut1 = triIn;
		return 1;
	}
	if (insidePointCount == 1 && outsidePointCount == 2) {
		triOut1.p[0] = *insidePoints[0];
		triOut1.hide_edge_1 = triIn.hide_edge_1;
		triOut1.hide_edge_2 = triIn.hide_edge_2;
		triOut1.hide_edge_3 = triIn.hide_edge_3;
		triOut1.color = triIn.color;

		auto pointInside = pointsInside[0];
		switch (pointInside)
		{
		case 0:
			triOut1.hide_edge_2 = false;
			break;
		case 1:
			triOut1.hide_edge_3 = false;
			break;
		case 2:
			triOut1.hide_edge_1 = false;
			break;
		default:
			break;
		}

		triOut1.p[1] = Vector3IntersectPlane(planePoint, planeNormal, *insidePoints[0], *outsidePoints[0]);
		triOut1.p[2] = Vector3IntersectPlane(planePoint, planeNormal, *insidePoints[0], *outsidePoints[1]);

		return 1;
	}
	if (insidePointCount == 2 && outsidePointCount == 1) {
		triOut1.hide_edge_1 = triIn.hide_edge_1;
		triOut1.hide_edge_2 = triIn.hide_edge_2;
		triOut1.hide_edge_3 = triIn.hide_edge_3;

		triOut2.hide_edge_1 = triIn.hide_edge_1;
		triOut2.hide_edge_2 = triIn.hide_edge_2;
		triOut2.hide_edge_3 = triIn.hide_edge_3;

		int a = pointsInside[0];
		int b = pointsInside[1];

		if (a == 0 && b == 1) {
			triOut1.hide_edge_2 = false;
			triOut2.hide_edge_1 = false;
			triOut2.hide_edge_3 = false;
		}
		else if (a == 1 && b == 2) {
			triOut1.hide_edge_3 = false;
			triOut2.hide_edge_1 = false;
			triOut2.hide_edge_2 = false;
		}
		

		triOut1.p[0] = *insidePoints[0];
		triOut1.p[1] = *insidePoints[1];
		triOut1.p[2] = Vector3IntersectPlane(planePoint, planeNormal, *insidePoints[0], *outsidePoints[0]);
		triOut1.color = triIn.color;

		triOut2.p[0] = *insidePoints[1];
		triOut2.p[1] = triOut1.p[2];
		triOut2.p[2] = Vector3IntersectPlane(planePoint, planeNormal, *insidePoints[1], *outsidePoints[0]);
		triOut2.color = triIn.color;

		return 2;
	}
	return 0;
}

Vector3 util::Vector3IntersectPlane(Vector3& planePoint, Vector3& planeNormal, Vector3& lineStart, Vector3 lineEnd)
{
	planeNormal = Vector3Normalize(planeNormal);
	Vector3 lineStartToEnd = Vector3Subtract(lineEnd, lineStart);
	auto t = (Vector3DotProduct(planePoint, planeNormal) - Vector3DotProduct(lineStart, planeNormal)) / (Vector3DotProduct(lineStartToEnd, planeNormal));
	return Vector3Add(lineStart, Vector3Scale(lineStartToEnd, t));
}

util::Mesh util::MakeUnitCube()
{
	auto cube = util::Mesh();

	cube.tris = {
		{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
		{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},

		{ 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
		{ 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

		{ 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
		{ 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},

		{ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
		{ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},

		{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{ 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},

		{ 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
		{ 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
	};

	return cube;
}

Matrix util::BillboardMatrix(const Matrix& viewMatrix, const Vector3& position)
{
	auto mat = Matrix();

	mat.m0 = viewMatrix.m0;
	mat.m1 = viewMatrix.m4;
	mat.m2 = viewMatrix.m8;
	mat.m3 = 0;
	mat.m4 = viewMatrix.m1;
	mat.m5 = viewMatrix.m5;
	mat.m6 = viewMatrix.m9;
	mat.m7 = 0;
	mat.m8 = viewMatrix.m2;
	mat.m9 = viewMatrix.m6;
	mat.m10 = viewMatrix.m10;
	mat.m11 = 0;
	mat.m12 = position.x;
	mat.m13 = position.y;
	mat.m14 = position.z;
	mat.m15 = 1;

	return mat;
}

void util::Mesh::setColor(Color color)
{
	for (auto& tri : tris) {
		tri.color = color;
	}
}

void util::Mesh::showWireframe(bool enable)
{
	if (!enable) {
		for (auto& tri : tris)
		{
			tri.hide_edge_1 = true;
			tri.hide_edge_2 = true;
			tri.hide_edge_3 = true;
		}
		return;
	}
	for (auto& tri : tris)
	{
		auto edge_1 = Vector3Normalize(Vector3Subtract(tri.p[1], tri.p[0]));
		auto edge_2 = Vector3Normalize(Vector3Subtract(tri.p[2], tri.p[1]));
		auto edge_3 = Vector3Normalize(Vector3Subtract(tri.p[0], tri.p[2]));
		auto dot_1 = fabs(Vector3DotProduct(edge_2, edge_3));
		auto dot_2 = fabs(Vector3DotProduct(edge_1, edge_3));
		auto dot_3 = fabs(Vector3DotProduct(edge_1, edge_2));
		tri.hide_edge_1 = (dot_1 <= dot_2) && (dot_1 <= dot_3);
		tri.hide_edge_2 = (dot_2 <= dot_1) && (dot_2 <= dot_3);
		tri.hide_edge_3 = (dot_3 <= dot_1) && (dot_3 <= dot_2);
	}
}
