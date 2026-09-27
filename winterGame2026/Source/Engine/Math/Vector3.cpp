#include "Vector3.h"
float Vector3::Length() const
{
	return std::hypot(x, y, z);
}

void Vector3::Normalize()
{
	auto len = std::hypot(x, y, z);
	if (len == 0.0f)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		return;
	}
	x /= len;
	y /= len;
	z /= len;
}

Vector3 Vector3::Normalized() const
{
	auto len = std::hypot(x, y, z);
	if (len == 0.0f)
	{
		return { 0.0f,0.0f,0.0f };
	}
	return{ x / len,y / len,z / len };
}

Vector3 Vector3::Zero()
{
	return Vector3(0.0f, 0.0f, 0.0f);
}

Vector3 Vector3::Forward()
{
	return Vector3(0.0f, 0.0f, 1.0f);
}

Vector3 Vector3::Right()
{
	return Vector3(1.0f, 0.0f, 0.0f);
}

Vector3 Vector3::Up()
{
	return Vector3(0.0f, 1.0f, 0.0f);
}

Vector3 Vector3::Under()
{
	return Vector3(0.0f, -1.0f, 0.0f);
}

float Vector3::Dot(const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::Cross(const Vector3& other) const
{
	return { y * other.z - z * other.y,
			 z * other.x - x * other.z,
			 x * other.y - y * other.x };
}

Vector3 Vector3::operator-() const
{
	return { -x,-y,-z };
}

void Vector3::operator+=(const Vector3& val)
{
	x += val.x;
	y += val.y;
	z += val.z;
}

void Vector3::operator-=(const Vector3& val)
{
	x -= val.x;
	y -= val.y;
	z -= val.z;
}

void Vector3::operator*=(const float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

Vector3 Vector3::operator+(const Vector3& val)const
{
	return { x + val.x,y + val.y,z + val.z };
}

Vector3 Vector3::operator-(const Vector3& val)const
{
	return { x - val.x,y - val.y,z - val.z };
}

Vector3 Vector3::operator*(float scale) const
{
	return { x * scale,y * scale,z * scale };
}

Vector3 Vector3::operator/(float scale) const
{
	return { x / scale,y / scale,z / scale };
}
