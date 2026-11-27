#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"
#include"Quaternion.h"
struct Transform
{
	//ポジション
	Vector3 position{ 0.0f,0.0f,0.0f };
	//回転
	Quaternion rotation = Quaternion::Identity();
	//拡大縮小
	Vector3 scale{ 1.0f,1.0f,1.0f };

	/// <summary>
	/// ワールド行列に変換
	/// </summary>
	/// <returns>ワールド行列</returns>
	Matrix4x4 GetWorldMatrix()const;
	/// <summary>
	/// 回転行列を取得する
	/// </summary>
	/// <returns>回転行列</returns>
	Matrix4x4 GetRotationMatrix()const;
	/// <summary>
	/// オブジェクトの前方向ベクトルを返す
	/// </summary>
	/// <returns>前方向ベクトル</returns>
	Vector3 Forward()const;
	/// <summary>
	/// オブジェクトの右方向ベクトルを返す
	/// </summary>
	/// <returns>右方向ベクトル</returns>
	Vector3 Right()const;
	/// <summary>
	/// オブジェクトの上方向ベクトルを返す
	/// </summary>
	/// <returns>上方向ベクトル</returns>
	Vector3 Up()const;
	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="move">移動ベクトル</param>
	void Translate(const Vector3& move);
	/// <summary>
	/// ポジションをセットする
	/// </summary>
	/// <param name="pos">ポジション</param>
	void SetPosition(const Vector3& pos);
	/// <summary>
	/// ポジションを取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetPosition()const;
	/// <summary>
	/// 回転をセットする
	/// </summary>
	/// <param name="r">回転</param>
	void SetRotate(const Quaternion& q);

	/// <summary>
	/// クォータニオンを取得
	/// </summary>
	/// <returns></returns>
	Quaternion GetRotation()const;

	/// <summary>
	/// 拡大縮小をセット
	/// </summary>
	/// <param name="scale">拡大縮小倍率</param>
	void SetScale(const Vector3& s);

	/// <summary>
	/// 行列から各要素を取得
	/// </summary>
	/// <param name="mat"></param>
	/// <returns></returns>
	static Transform FromMatrix(const Matrix4x4& mat);
};
