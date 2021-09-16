#pragma once
struct Vec3
{
	int x;
	int y;
	int z;

	Vec3 operator+(const Vec3& other);
};

