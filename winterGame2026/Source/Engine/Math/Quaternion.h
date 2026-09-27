#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"

class Quaternion
{
public:
	float x, y, z, w;
	/// <summary>
	/// コンストラクタで単位クォータニオン(回転なし)を生成
	/// </summary>
	Quaternion();
	/// <summary>
	/// 各成分を指定して生成
	/// </summary>
	Quaternion(float x, float y, float z, float w);

	/// <summary>
	/// 回転なしのクォータニオンを取得
	/// </summary>
	/// <returns></returns>
	static Quaternion Identity();

	/// <summary>
	/// クォータニオンの長さを取得
	/// </summary>
	/// <returns></returns>
	float Length()const;

	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns></returns>
	Quaternion Normalize()const;

	/// <summary>
	/// 共役クォータニオンを取得
	/// </summary>
	/// <returns></returns>
	Quaternion Conjugate()const;

	/// <summary>
	/// 逆クォータニオンを取得
	/// </summary>
	/// <returns></returns>
	Quaternion Inverse()const;

	/// <summary>
	/// ベクトルへ回転を適用 
	/// </summary>
	/// <param name="vec">ベクトル</param>
	/// <returns></returns>
	Vector3 Rotate(const Vector3& vec)const;

	/// <summary>
	/// 回転軸と回転角からQuaternionを生成する
	/// </summary>
	/// <param name="angle">回転角</param>
	/// <param name="axis">回転軸</param>
	/// <returns></returns>
	static Quaternion AngleAxis(float angle, const Vector3& axis);

	/// <summary>
	/// 行列に変換する
	/// </summary>
	/// <returns></returns>
	Matrix4x4 ToMatrix()const;

	/// <summary>
	/// 回転行列からクォータニオンを生成する
	/// </summary>
	/// <param name="mat"></param>
	/// <returns></returns>
	static Quaternion FromMatrix(const Matrix4x4& mat);

	/// <summary>
	/// 指定方向を向くクォータニオンを生成する
	/// </summary>
	/// <param name="forward"></param>
	/// <param name="up"></param>
	/// <returns></returns>
	static Quaternion LookRotation(const Vector3& forward, const Vector3& up);

	/// <summary>
	/// クォータニオン同士の内積
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	float Dot(const Quaternion& q)const;

	/// <summary>
	/// 球面線形保管
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <param name="t">0～1</param>
	/// <returns></returns>
	static Quaternion Slerp(const Quaternion& from, const Quaternion& to, float t);

	Quaternion operator*(const Quaternion& q)const;
};