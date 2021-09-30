#include "Utility3d.h"
#include <raymath.h>

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
		// TODO???
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

		triOut2.p[0] = *insidePoints[1];
		triOut2.p[1] = triOut1.p[2];
		triOut2.p[2] = Vector3IntersectPlane(planePoint, planeNormal, *insidePoints[1], *outsidePoints[0]);

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