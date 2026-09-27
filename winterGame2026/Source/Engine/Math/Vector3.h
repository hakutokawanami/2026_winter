#pragma once
#include"DxLib.h"

/// <summary>
/// Vector3構造体
/// </summary>
struct Vector3
{
	//x,y,z,の3要素
	float x, y, z;
	/// <summary>
	/// ベクトルの大きさをとる関数
	/// </summary>
	/// <returns>ベクトルの大きさ</returns>
	float Length()const;
	/// <summary>
	/// 自身を正規化する
	/// </summary>
	void Normalize();
	/// <summary>
	/// 正規化されたベクトルを返す
	/// </summary>
	/// <returns>正規化されたベクトル</returns>
	Vector3 Normalized()const;

	/// <summary>
	/// x,y,zすべて0
	/// </summary>
	/// <returns></returns>
	static Vector3 Zero();
	/// <summary>
	/// {0 , 0 , 1}
	/// </summary>
	/// <returns></returns>
	static Vector3 Forward();
	/// <summary>
	/// {1 , 0 , 0}
	/// </summary>
	/// <returns></returns>
	static Vector3 Right();
	/// <summary>
	/// {0 , 1 , 0}
	/// </summary>
	/// <returns></returns>
	static Vector3 Up();
	/// <summary>
	/// {0 , -1 , 0}
	/// </summary>
	/// <returns></returns>
	static Vector3 Under();

	/// <summary>
	/// 内積した結果を返す
	/// </summary>
	/// <param name="other">ベクトル</param>
	/// <returns>内積の結果</returns>
	float Dot(const Vector3& other)const;
	/// <summary>
	/// 外積した結果を返す
	/// </summary>
	/// <param name="other">ベクトル</param>
	/// <returns>外積の結果</returns>
	Vector3 Cross(const Vector3& other)const;

	//DXlibのVECTOR型に変換できるようにする
	operator VECTOR() const
	{
		return VGet(x, y, z);
	}
	//DXlibのVECTOR型を代入できるようにする
	Vector3& operator=(const VECTOR& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	//ベクトルを反転する
	Vector3 operator-()const;
	//加算
	void operator+=(const Vector3& val);
	//減算
	void operator-=(const Vector3& val);
	//ベクトルをn倍する
	void operator*=(const float scale);
	//加算
	Vector3 operator+(const Vector3& val)const;
	//減算
	Vector3 operator-(const Vector3& val)const;
	//ベクトルの累乗
	Vector3 operator*(float scale)const;

	Vector3 operator/(float scale)const;
};

