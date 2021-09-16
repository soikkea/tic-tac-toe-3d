#include "Vec3.h"

Vec3 Vec3::operator+(const Vec3& other)
{
	return Vec3{x + other.x, y + other.y, z + other.z};
}
