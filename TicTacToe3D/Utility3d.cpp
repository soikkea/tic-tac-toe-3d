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
