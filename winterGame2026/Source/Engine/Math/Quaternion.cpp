#include "Quaternion.h"
#include<cmath>
#include<algorithm>
Quaternion::Quaternion() :
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f)
{
}

Quaternion::Quaternion(float x, float y, float z, float w) :
	x(x),
	y(y),
	z(z),
	w(w)
{
}

Quaternion Quaternion::Identity()
{
	return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

float Quaternion::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

Quaternion Quaternion::Normalize() const
{
	float length = Length();

	if (length <= 0.00001f)
	{
		return Identity();
	}

	float inv = 1.0f / length;

	return Quaternion(x * inv, y * inv, z * inv, w * inv);
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::Inverse() const
{
	float lengthSq = x * x + y * y + z * z + w * w;

	if (lengthSq <= 0.00001f)
	{
		return Identity();
	}

	Quaternion conjugate = Conjugate();

	float inv = 1.0f / lengthSq;

	return Quaternion(conjugate.x * inv,
		conjugate.y * inv,
		conjugate.z * inv,
		conjugate.w * inv);
}

Vector3 Quaternion::Rotate(const Vector3& vec) const
{
	//ベクトルを純粋クォータニオンに変換
	Quaternion qVec(vec.x, vec.y, vec.z, 0.0f);
	//回転を適用する
	Quaternion result = (*this) * qVec * Inverse();
	//ベクトルに戻して返す
	return Vector3(result.x, result.y, result.z);
}

Quaternion Quaternion::AngleAxis(float angle, const Vector3& axis)
{
	//回転軸を正規化
	Vector3 n = axis.Normalized();
	//半角を求める
	float half = angle * 0.5f;

	float s = sinf(half);
	float c = cosf(half);

	return Quaternion(n.x * s, n.y * s, n.z * s, c);
}

Matrix4x4 Quaternion::ToMatrix() const
{
	Quaternion q = Normalize();

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;

	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;

	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	Matrix4x4 mat
	(
		1.0f - 2.0f * (yy + zz),
		2.0f * (xy - wz),
		2.0f * (xz + wy),
		0.0f,

		2.0f * (xy + wz),
		1.0f - 2.0f * (xx + zz),
		2.0f * (yz - wx),
		0.0f,

		2.0f * (xz - wy),
		2.0f * (yz + wx),
		1.0f - 2.0f * (xx + yy),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);

	return mat;
}

Quaternion Quaternion::FromMatrix(const Matrix4x4& mat)
{
	Quaternion q;

	float trace = mat.m00 + mat.m11 + mat.m22;

	if (trace > 0.0f)
	{
		float s = sqrtf(trace + 1.0f) * 2.0f;

		q.w = 0.25f * s;
		q.x = (mat.m21 - mat.m12) / s;
		q.y = (mat.m02 - mat.m20) / s;
		q.z = (mat.m10 - mat.m01) / s;
	}
	else if (mat.m00 > mat.m11 && mat.m00 > mat.m22)
	{
		float s = sqrtf(1.0f + mat.m00 - mat.m11 - mat.m22) * 2.0f;

		q.w = (mat.m21 - mat.m12) / s;
		q.x = 0.25f * s;
		q.y = (mat.m01 + mat.m10) / s;
		q.z = (mat.m02 + mat.m20) / s;
	}
	else if (mat.m11 > mat.m22)
	{
		float s = sqrtf(1.0f + mat.m11 - mat.m00 - mat.m22) * 2.0f;

		q.w = (mat.m02 - mat.m20) / s;
		q.x = (mat.m01 + mat.m10) / s;
		q.y = 0.25f * s;
		q.z = (mat.m12 + mat.m21) / s;
	}
	else
	{
		float s = sqrtf(1.0f + mat.m22 - mat.m00 - mat.m11) * 2.0f;

		q.w = (mat.m10 - mat.m01) / s;
		q.x = (mat.m02 + mat.m20) / s;
		q.y = (mat.m12 + mat.m21) / s;
		q.z = 0.25f * s;
	}

	return q.Normalize();
}


Quaternion Quaternion::LookRotation(const Vector3& forward, const Vector3& up)
{
	Vector3 f = forward.Normalized();

	// Yaw
	float yaw = atan2f(f.x, f.z);

	// Pitch
	float pitch = -asinf(f.y);

	Quaternion qYaw =
		Quaternion::AngleAxis(yaw, Vector3::Up());

	Quaternion qPitch =
		Quaternion::AngleAxis(pitch, Vector3::Right());

	return qYaw * qPitch;
}

float Quaternion::Dot(const Quaternion& q) const
{
	return   x * q.x + y * q.y + z * q.z + w * q.w;
}

Quaternion Quaternion::Slerp(const Quaternion& from, const Quaternion& to, float t)
{
	//保管範囲を0～1にする
	t = std::clamp(t, 0.0f, 1.0f);

	Quaternion q1 = from.Normalize();
	Quaternion q2 = to.Normalize();
	//2つのクォータニオンの角度を調べる
	float dot = q1.Dot(q2);

	//逆向きなら反転
	if (dot < 0.0f)
	{
		q2.x = -q2.x;
		q2.y = -q2.y;
		q2.z = -q2.z;
		q2.w = -q2.w;

		dot = -dot;
	}

	// ほぼ同じ向きならLerpにする
	if (dot > 0.9995f)
	{
		Quaternion result(
			q1.x + (q2.x - q1.x) * t,
			q1.y + (q2.y - q1.y) * t,
			q1.z + (q2.z - q1.z) * t,
			q1.w + (q2.w - q1.w) * t);

		return result.Normalize();
	}

	//角度を求める
	float theta0 = acosf(dot);
	//tだけ進めた角度
	float theta = theta0 * t;
	float sinTheta = sinf(theta);
	float sinTheta0 = sinf(theta0);
	float s0 = cosf(theta) - dot * sinTheta / sinTheta0;
	float s1 = sinTheta / sinTheta0;

	return Quaternion(
		q1.x * s0 + q2.x * s1,
		q1.y * s0 + q2.y * s1,
		q1.z * s0 + q2.z * s1,
		q1.w * s0 + q2.w * s1
	).Normalize();
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	return Quaternion(
		w * q.x + x * q.w + y * q.z - z * q.y,
		w * q.y - x * q.z + y * q.w + z * q.x,
		w * q.z + x * q.y - y * q.x + z * q.w,
		w * q.w - x * q.x - y * q.y - z * q.z
	);
}
