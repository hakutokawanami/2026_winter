#pragma once
#include"Game/Base/GameObject.h"
#include"Game/Common/PlayerID.h"
#include<string>
#include<memory>

/// <summary>
/// プレイヤーアニメーションの名前空間
/// </summary>
namespace PlayerAnim
{
	const std::wstring idle = L"Armature|Idle";
	const std::wstring walk = L"Armature|Walk";
}

class AnimationController;
class Player : public GameObject
{
public:
	Player(PlayerID id, int modelHandle);
	~Player();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

private:
	///モデルハンドル
	int m_modelHandle;
	//プレイヤー識別子
	PlayerID m_playerID;
	//アニメーションコントローラー
	std::unique_ptr<AnimationController>m_animationController;
};

