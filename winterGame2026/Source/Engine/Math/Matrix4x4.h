#pragma once
#include <DxLib.h>
#include"Vector3.h"

class Matrix4x4
{
public:
	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Matrix4x4();

	/// <summary>
	/// 引数付きコンストラクタ
	/// </summary>
	Matrix4x4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	explicit Matrix4x4(const MATRIX& mat);

	//----------------------
	//行列を生成する関数たち
	//----------------------

	/// <summary>
	/// 単位行列を生成する
	/// </summary>
	/// <returns>単位行列</returns>
	static Matrix4x4 Identity();

	/// <summary>
	/// 平行移動行列を生成する
	/// </summary>
	/// <param name="x">x成分</param>
	/// <param name="y">y成分</param>
	/// <param name="z">z成分</param>
	/// <returns>平行移動行列</returns>
	static Matrix4x4 Translate(float x, float y, float z);
	/// <summary>
	/// 平行移動行列を生成する
	/// </summary>
	/// <param name="trans">移動値</param>
	/// <returns>平行移動行列</returns>
	static Matrix4x4 Translate(const Vector3& pos);

	/// <summary>
	/// 拡大縮小行列を生成する
	/// </summary>
	/// <param name="x">x成分</param>
	/// <param name="y">y成分</param>
	/// <param name="z">z成分</param>
	/// <returns>拡大縮小行列</returns>
	static Matrix4x4 Scale(float x, float y, float z);

	/// <summary>
	/// 拡大縮小行列を生成する
	/// </summary>
	/// <param name="scale">scale</param>
	/// <returns>拡大縮小行列</returns>
	static Matrix4x4 Scale(const Vector3& scale);

	/// <summary>
	/// x軸の回転行列を生成する
	/// </summary>
	/// <param name="angle">ラジアン</param>
	/// <returns>x軸の回転行列</returns>
	static Matrix4x4 RotationX(float angle);
	/// <summary>
	/// y軸の回転行列を生成する
	/// </summary>
	/// <param name="angle">ラジアン</param>
	/// <returns>y軸の回転行列</returns>
	static Matrix4x4 RotationY(float angle);
	/// <summary>
	/// z軸の回転行列を生成する
	/// </summary>
	/// <param name="angle">ラジアン</param>
	/// <returns>z軸の回転行列</returns>
	static Matrix4x4 RotationZ(float angle);

	//-------------------------
	// 行列の変換をする関数たち
	//-------------------------

	/// <summary>
	/// 行列を転置する
	/// </summary>
	/// <returns>転置した行列</returns>
	Matrix4x4 Transpose() const;

	/// <summary>
	/// DxLibのMATRIXに変換する関数
	/// </summary>
	/// <returns>DxLib::MATRIX</returns>
	MATRIX ChangeDxMat() const;

	//---------------------------
	// ベクトルを変換する関数たち
	//---------------------------

	/// <summary>
	/// 位置ベクトルを変換する関数（平行移動あり）
	/// </summary>
	/// <param name="point">位置ベクトル</param>
	/// <returns>行列を適用した位置ベクトル</returns>
	Vector3 TransformPoint(const Vector3& point) const;

	/// <summary>
	/// 方向ベクトルを変換する関数（平行移動あり）
	/// </summary>
	/// <param name="vector">方向ベクトル</param>
	/// <returns>行列を適用した方向ベクトル</returns>
	Vector3 TransformVector(const Vector3& vector) const;

	//------------------------
	// 取得、変換関数
	//------------------------

	/// <summary>
	/// 位置を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetTranslation() const;
	/// <summary>
	/// 回転を取得
	/// </summary>
	/// <returns></returns>
	Matrix4x4 GetRotationMatrix() const;
	/// <summary>
	/// 拡大縮小を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetScale() const;

	//-------------
	// 演算子
	//-------------

	Matrix4x4 operator+(const Matrix4x4& right) const;

	Matrix4x4 operator-(const Matrix4x4& right) const;

	Matrix4x4 operator*(const Matrix4x4& right) const;

	Matrix4x4& operator*=(const Matrix4x4& right);

	// TransformPointと同じ
	Vector3 operator*(const Vector3& right) const;
};