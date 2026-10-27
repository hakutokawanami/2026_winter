#pragma once
#include"HitBox.h"
#include"Engine/Math/Vector3.h"
#include"Game/Common/PlayerID.h"
#include<vector>

class HitBoxSet
{
public:
	HitBoxSet(PlayerID owner,int modelHandle);

	/// <summary>
	/// 押し出し判定の大きさを設定する
	/// </summary>
	/// <param name="radius">半径</param>
	/// <param name="height">高さ</param>
	void SetPushBox(float radius,float height);

	/// <summary>
	/// やられ判定をボーンにつける
	/// </summary>
	/// <param name="def">判定の場所と半径</param>
	void AddHurtBox(const BoneCapsuleDef& def);
	/// <summary>
	/// 攻撃判定を出す
	/// </summary>
	/// <param name="def">判定の場所と半径</param>
	/// <param name="data"></param>
	/// <param name="instanceID"></param>
	void StartAttack(const BoneCapsuleDef& def, const AttackData& data, int instanceID);
	/// <summary>
	/// 攻撃判定を消す
	/// </summary>
	void EndAttack();

	/// <summary>
	/// プレイヤーの位置に合わせて判定を更新する
	/// </summary>
	/// <param name="playerPos">プレイヤーの位置</param>
	void Update(const Vector3& playerPos);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 押し出し判定を取得する関数
	/// </summary>
	/// <returns></returns>
	const HitBox& GetPushBox()const { return m_pushBox; }
	/// <summary>
	/// 攻撃判定を取得する関数
	/// </summary>
	/// <returns></returns>
	const HitBox& GetAttackBox()const { return m_attackBox; }
	/// <summary>
	/// やられ判定を取得する関数
	/// </summary>
	/// <returns></returns>
	const std::vector<HitBox>& GetHurtBoxes()const { return m_hurtBoxes; }
	/// <summary>
	/// 最後に受けた攻撃の番号を取得する関数
	/// </summary>
	/// <returns></returns>
	int GetLastReceivedAttackID()const { return m_lastReceivedAttackID; }
	/// <summary>
	/// 最後に受けた攻撃の番号をセットする関数
	/// </summary>
	/// <param name="attackInstanceID"></param>
	void SetLastReceivedAttackID(int attackInstanceID) { m_lastReceivedAttackID = attackInstanceID; }

private:
	/// <summary>
	/// ボーンについているカプセルの情報
	/// </summary>
	struct BoneAttachment
	{
		//ボーンのフレーム番号
		int frame;
		//カプセルの定義
		BoneCapsuleDef def;
	};
	/// <summary>
	/// ボーン名からフレーム番号を探す
	/// </summary>
	/// <param name="def"></param>
	/// <returns></returns>
	int FindBones(const wchar_t* boneName)const;
	/// <summary>
	/// カプセルをボーンの位置に合わせる
	/// </summary>
	/// <param name="box"></param>
	/// <param name="link"></param>
	void UpdateBoneCapsule(HitBox& box, const BoneAttachment& attachment);
private:
	//持ち主
	PlayerID m_owner;
	//ボーンの位置を取るためのモデル
	int m_modelHandle;
	//押し出し判定
	HitBox m_pushBox;
	//押し出し判定の高さ
	float m_pushBoxHeight;
	//やられ判定
	std::vector<HitBox>m_hurtBoxes;
	//やられ判定がついているボーン
	std::vector<BoneAttachment>m_hurtAttachments;

	//攻撃判定
	HitBox m_attackBox;
	//攻撃判定のカプセルの情報
	BoneAttachment m_attackAttachment;

	//最後に受けた攻撃の番号
	int m_lastReceivedAttackID;
};

