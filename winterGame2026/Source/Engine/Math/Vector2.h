#pragma once

/// <summary>
/// Vector2構造体
/// </summary>
struct Vector2
{
	//x,yの2要素
	float x, y;
	/// <summary>
	/// ベクトルの大きさをとる
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
	Vector2 Normalized()const;
	//ベクトルを反転する
	Vector2 operator-()const;
	//加算
	void operator+=(const Vector2& val);
	//減算
	void operator-=(const Vector2& val);
	//ベクトルをn倍する
	void operator*=(const float scale);
	//加算
	Vector2 operator+(const Vector2& val)const;
	//減算
	Vector2 operator-(const Vector2& val)const;
	//ベクトルの累乗
	Vector2 operator*(float scale)const;
};

/// <summary>
/// 2Dのサイズを定義する構造体
/// </summary>
struct Size
{
	//幅
	int w;
	//高さ
	int h;
};