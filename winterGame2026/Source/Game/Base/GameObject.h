#pragma once
#include"Engine/Math/Transform.h"
class GameObject
{
public:
	virtual ~GameObject() = default;
	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init()abstract;
	/// <summary>
	/// 終了時処理関数
	/// </summary>
	virtual void End()abstract;
	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update()abstract;
	/// <summary>
	/// 描画関数
	/// </summary>
	virtual void Draw()abstract;
	/// <summary>
	/// トランスフォーム取得
	/// </summary>
	/// <returns></returns>
	Transform& GetTransform() { return m_transform; };
	const Transform& GetTransform()const { return m_transform; }

	/// <summary>
	/// アクティブかどうか
	/// </summary>
	/// <returns></returns>
	bool IsActive() const { return m_isActive; }

	/// <summary>
	/// アクティブかどうかをセット
	/// </summary>
	/// <param name="active"></param>
	void SetActive(bool active) { m_isActive = active; }

	/// <summary>
	/// 削除依頼があるかどうか 
	/// </summary>
	/// <returns></returns>
	bool IsPendingDestroy()const { return m_isPendingDestroy; }
	
	/// <summary>
	/// 削除依頼を送る
	/// </summary>
	void Destroy() { m_isPendingDestroy = true; }

protected:
	//トランスフォーム
	Transform m_transform;
	//アクティブかどうか
	bool m_isActive = true;
	//削除依頼
	bool m_isPendingDestroy = false;
};

