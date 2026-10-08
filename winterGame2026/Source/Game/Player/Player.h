#pragma once
#include"Game/Base/GameObject.h"
#include"Game/Common/PlayerID.h"
#include<string>
#include<memory>

class PlayerStateBase;
class AnimationController;
class BattleManager;
class Player : public GameObject
{
public:
	//アニメーション名たち
	static constexpr const wchar_t* kIdleAnimName = L"Armature|Idle";
	static constexpr const wchar_t* kWalkAnimName = L"Armature|Walk";

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
	/// Dxライブラリにトランスフォームを適用する
	/// </summary>
	void ApplyDxLibMatrix();

	/// <summary>
	/// プレイヤーIDを取得する関数
	/// </summary>
	/// <returns></returns>
	const PlayerID GetPlayerID()const { return m_playerID; }

	BattleManager* GetBattleManager()const { return m_battleManager; }

	AnimationController& GetAnimationController()const { return *m_animationController; }
	
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
};

