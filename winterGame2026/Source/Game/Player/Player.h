#pragma once
#include"Game/Base/GameObject.h"
#include"Game/Common/PlayerID.h"
#include<string>
#include<memory>

class PlayerStateBase;
class AnimationController;
class BattleManager;
class HitBoxSet;
struct AttackData;
class Player : public GameObject
{
public:
	//最大HP
	static constexpr int kMaxHp = 1000;

	//アニメーション名たち
	static constexpr const wchar_t* kIdleAnimName = L"Armature|Idle";
	//歩きアニメーション
	static constexpr const wchar_t* kWalkForwardAnimName = L"Armature|WalkForward";
	static constexpr const wchar_t* kWalkBackAnimName = L"Armature|WalkBack";
	static constexpr const wchar_t* kWalkSideInAnimName = L"Armature|WalkSideIn";
	static constexpr const wchar_t* kWalkSideOutAnimName = L"Armature|WalkSideOut";
	//ステップアニメーション
	static constexpr const wchar_t* kStepForwardAnimName = L"Armature|StepForward";
	static constexpr const wchar_t* kStepBackAnimName = L"Armature|StepBack";
	static constexpr const wchar_t* kStepSideInAnimName = L"Armature|StepSideIn";
	static constexpr const wchar_t* kStepSideOutAnimName = L"Armature|StepSideOut";
	//攻撃アニメーション
	static constexpr const wchar_t* kJabAnimName = L"Armature|Jab";
	static constexpr const wchar_t* kLungeJabAnimName = L"Armature|LungeJab";
	//ヒットアニメーション
	static constexpr const wchar_t* kHitHeadLightAnimName = L"Armature|HitHeadLight";
	static constexpr const wchar_t* kHitHeadHeavyAnimName = L"Armature|HitHeadHeavy";
	//ガードヒットアニメーション
	static constexpr const wchar_t* kGuardHitAnimName = L"Armature|GuardHit";
	//つかみアニメーション
	static constexpr const wchar_t* kGrabAnimName = L"Armature|Grab";
	//投げアニメーション
	static constexpr const wchar_t* kThrowAnimName = L"Armature|Throw";
	static constexpr const wchar_t* kThrownAnimName = L"Armature|Thrown";
	//起き上がりアニメーション
	static constexpr const wchar_t* kGettingUpAnimName = L"Armature|GetUp";

public:
	Player(PlayerID id, int modelHandle, BattleManager* battleManager);
	~Player();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	/// <summary>
	/// 状態を変える
	/// </summary>
	/// <param name="newState">次のステート</param>
	void ChangeState(std::unique_ptr<PlayerStateBase>newState);

	/// <summary>
	/// 相手のプレイヤーの方向を向く
	/// </summary>
	void OpponentRotate();

	/// <summary>
	/// 少しずつ相手のプレイヤーの方向を向く
	/// </summary>
	void OpponentRotateSmooth();

	/// <summary>
	/// Dxライブラリにトランスフォームを適用する
	/// </summary>
	void ApplyDxLibMatrix();

	/// <summary>
	/// プレイヤーIDを取得する関数
	/// </summary>
	/// <returns></returns>
	const PlayerID GetPlayerID()const { return m_playerID; }

	/// <summary>
	/// ステートからバトルマネージャーを使うときに取得するための関数
	/// </summary>
	/// <returns></returns>
	BattleManager* GetBattleManager()const { return m_battleManager; }

	/// <summary>
	/// ステートからアニメーションコントローラーを使うときに取得するための関数
	/// </summary>
	/// <returns></returns>
	AnimationController& GetAnimationController()const { return *m_animationController; }

	/// <summary>
	/// 当たり判定を取得する関数
	/// </summary>
	/// <returns></returns>
	HitBoxSet& GetHitBoxes()const { return *m_hitBoxes; }

	/// <summary>
	/// モデルハンドルを取得する関数
	/// </summary>
	/// <returns></returns>
	int GetModelHandle()const { return m_modelHandle; }

	/// <summary>
	/// 攻撃を受けたときの処理
	/// </summary>
	/// <param name="attack">受けた技の性能</param>
	void OnHit(const AttackData& attack);
	/// <summary>
	/// 体力を取得する
	/// </summary>
	/// <returns></returns>
	int GetHp()const { return m_hp; }
	/// <summary>
	/// 体力がゼロになったかどうか
	/// </summary>
	/// <returns></returns>
	bool IsDead()const { return m_hp <= 0; }
	/// <summary>
	/// 投げられる状態かどうか
	/// </summary>
	/// <returns></returns>
	bool IsThrowable()const;
	/// <summary>
	/// 投げられる処理
	/// </summary>
	/// <param name="attack"></param>
	void OnThrowHit(const AttackData& attack);
	/// <summary>
	/// 投げる処理
	/// </summary>
	/// <param name="attack"></param>
	void OnThrowSuccess(const AttackData& attack);
	/// <summary>
	/// 投げられているかどうか
	/// </summary>
	/// <returns></returns>
	bool IsThrowing()const;

private:
	///モデルハンドル
	int m_modelHandle;
	//プレイヤー識別子
	PlayerID m_playerID;
	//アニメーションコントローラー
	std::unique_ptr<AnimationController>m_animationController;
	//バトルマネージャー
	BattleManager* m_battleManager;
	//状態
	std::unique_ptr<PlayerStateBase>m_state;
	//当たり判定
	std::unique_ptr<HitBoxSet>m_hitBoxes;
	//体力
	int m_hp;
};

