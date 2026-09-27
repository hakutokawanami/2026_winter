#include "Matrix4x4.h"
#include <cmath>

Matrix4x4::Matrix4x4()
	: m00(0), m01(0), m02(0), m03(0),
	m10(0), m11(0), m12(0), m13(0),
	m20(0), m21(0), m22(0), m23(0),
	m30(0), m31(0), m32(0), m33(0)
{
}

Matrix4x4::Matrix4x4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
	: m00(m00), m01(m01), m02(m02), m03(m03),
	m10(m10), m11(m11), m12(m12), m13(m13),
	m20(m20), m21(m21), m22(m22), m23(m23),
	m30(m30), m31(m31), m32(m32), m33(m33)
{
}

Matrix4x4::Matrix4x4(const MATRIX& mat) :
	m00(mat.m[0][0]), m01(mat.m[1][0]), m02(mat.m[2][0]), m03(mat.m[3][0]),
	m10(mat.m[0][1]), m11(mat.m[1][1]), m12(mat.m[2][1]), m13(mat.m[3][1]),
	m20(mat.m[0][2]), m21(mat.m[1][2]), m22(mat.m[2][2]), m23(mat.m[3][2]),
	m30(mat.m[0][3]), m31(mat.m[1][3]), m32(mat.m[2][3]), m33(mat.m[3][3])
{
}

//------------------------------------------------------------

Matrix4x4 Matrix4x4::Identity()
{
	return Matrix4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Translate(float x, float y, float z)
{
	return Matrix4x4(
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Translate(const Vector3& pos)
{
	return Translate(pos.x, pos.y, pos.z);
}

Matrix4x4 Matrix4x4::Scale(float x, float y, float z)
{
	return Matrix4x4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
{
	return Scale(scale.x, scale.y, scale.z);
}

Matrix4x4 Matrix4x4::RotationX(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	return Matrix4x4(
		1, 0, 0, 0,
		0, c, -s, 0,
		0, s, c, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::RotationY(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	return Matrix4x4(
		c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::RotationZ(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	return Matrix4x4(
		c, -s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Transpose() const
{
	return Matrix4x4(
		m00, m10, m20, m30,
		m01, m11, m21, m31,
		m02, m12, m22, m32,
		m03, m13, m23, m33);
}

//------------------------------------------------------------

MATRIX Matrix4x4::ChangeDxMat() const
{
	MATRIX mat;

	mat.m[0][0] = m00; mat.m[0][1] = m10; mat.m[0][2] = m20; mat.m[0][3] = m30;
	mat.m[1][0] = m01; mat.m[1][1] = m11; mat.m[1][2] = m21; mat.m[1][3] = m31;
	mat.m[2][0] = m02; mat.m[2][1] = m12; mat.m[2][2] = m22; mat.m[2][3] = m32;
	mat.m[3][0] = m03; mat.m[3][1] = m13; mat.m[3][2] = m23; mat.m[3][3] = m33;

	return mat;
}

//------------------------------------------------------------

Vector3 Matrix4x4::TransformPoint(const Vector3& p) const
{
	return Vector3{
		m00 * p.x + m01 * p.y + m02 * p.z + m03,
		m10 * p.x + m11 * p.y + m12 * p.z + m13,
		m20 * p.x + m21 * p.y + m22 * p.z + m23
	};
}

Vector3 Matrix4x4::TransformVector(const Vector3& v) const
{
	return Vector3{
		m00 * v.x + m01 * v.y + m02 * v.z,
		m10 * v.x + m11 * v.y + m12 * v.z,
		m20 * v.x + m21 * v.y + m22 * v.z
	};
}

Vector3 Matrix4x4::GetTranslation() const
{
	return { m03,m13,m23 };
}

Matrix4x4 Matrix4x4::GetRotationMatrix() const
{
	Vector3 scale = GetScale();

	return Matrix4x4(
		m00 / scale.x, m01 / scale.y, m02 / scale.z, 0.0f,
		m10 / scale.x, m11 / scale.y, m12 / scale.z, 0.0f,
		m20 / scale.x, m21 / scale.y, m22 / scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Vector3 Matrix4x4::GetScale() const
{
	float x = Vector3(m00, m10, m20).Length();
	float y = Vector3(m01, m11, m21).Length();
	float z = Vector3(m02, m12, m22).Length();

	return { x,y,z };
}

//------------------------------------------------------------

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& right) const
{
	return Matrix4x4(
		m00 + right.m00, m01 + right.m01, m02 + right.m02, m03 + right.m03,
		m10 + right.m10, m11 + right.m11, m12 + right.m12, m13 + right.m13,
		m20 + right.m20, m21 + right.m21, m22 + right.m22, m23 + right.m23,
		m30 + right.m30, m31 + right.m31, m32 + right.m32, m33 + right.m33);
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& right) const
{
	return Matrix4x4(
		m00 - right.m00, m01 - right.m01, m02 - right.m02, m03 - right.m03,
		m10 - right.m10, m11 - right.m11, m12 - right.m12, m13 - right.m13,
		m20 - right.m20, m21 - right.m21, m22 - right.m22, m23 - right.m23,
		m30 - right.m30, m31 - right.m31, m32 - right.m32, m33 - right.m33);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& r) const
{
	return Matrix4x4(
		m00 * r.m00 + m01 * r.m10 + m02 * r.m20 + m03 * r.m30,
		m00 * r.m01 + m01 * r.m11 + m02 * r.m21 + m03 * r.m31,
		m00 * r.m02 + m01 * r.m12 + m02 * r.m22 + m03 * r.m32,
		m00 * r.m03 + m01 * r.m13 + m02 * r.m23 + m03 * r.m33,

		m10 * r.m00 + m11 * r.m10 + m12 * r.m20 + m13 * r.m30,
		m10 * r.m01 + m11 * r.m11 + m12 * r.m21 + m13 * r.m31,
		m10 * r.m02 + m11 * r.m12 + m12 * r.m22 + m13 * r.m32,
		m10 * r.m03 + m11 * r.m13 + m12 * r.m23 + m13 * r.m33,

		m20 * r.m00 + m21 * r.m10 + m22 * r.m20 + m23 * r.m30,
		m20 * r.m01 + m21 * r.m11 + m22 * r.m21 + m23 * r.m31,
		m20 * r.m02 + m21 * r.m12 + m22 * r.m22 + m23 * r.m32,
		m20 * r.m03 + m21 * r.m13 + m22 * r.m23 + m23 * r.m33,

		m30 * r.m00 + m31 * r.m10 + m32 * r.m20 + m33 * r.m30,
		m30 * r.m01 + m31 * r.m11 + m32 * r.m21 + m33 * r.m31,
		m30 * r.m02 + m31 * r.m12 + m32 * r.m22 + m33 * r.m32,
		m30 * r.m03 + m31 * r.m13 + m32 * r.m23 + m33 * r.m33);
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& right)
{
	*this = *this * right;
	return *this;
}

Vector3 Matrix4x4::operator*(const Vector3& right) const
{
	return TransformPoint(right);
}