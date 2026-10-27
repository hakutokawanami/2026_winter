#include "Player.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Math/Vector3.h"
#include"Engine/Input/InputManager.h"
#include"Game/Battle/BattleManager.h"
#include"State/PlayerStateBase.h"
#include"State/PlayerIdleState.h"
#include"Game/Battle/HitBoxSet.h"
#include<cassert>
#include<DxLib.h>
namespace
{
	//プレイヤーの初期位置
	constexpr Vector3 kPlayer1InitPos = { -200.0f,0.0f,0.0f };
	constexpr Vector3 kPlayer2InitPos = { 200.0f,0.0f,0.0f };
	//プレイヤーのスケール
	constexpr float kPlayer1Scale = 2.057f;
	constexpr float kPlayer2Scale = 0.252f;

	//押し出し判定のカプセルの半径
	constexpr float kPushRadius = 70.0f;
	//押し出し判定のカプセルの高さ
	constexpr float kPushHeight = 400.0f;

	//やられ判定
	constexpr BoneCapsuleDef kHurtBoxes[] =
	{
		//頭
		{ L"mixamorig:Head",         20.0f, 30.0f, 20.0f },
		//胴体
		{ L"mixamorig:Spine2",       40.0f, 50.0f },
		{ L"mixamorig:Hips",         30.0f, 50.0f },
		//腕(肘と手)
		{ L"mixamorig:LeftForeArm",  20.0f, 20.0f },
		{ L"mixamorig:LeftHand",     10.0f, 18.0f },
		{ L"mixamorig:RightForeArm", 20.0f, 20.0f },
		{ L"mixamorig:RightHand",    10.0f, 18.0f },
		//脚(太もも・膝・足首)
		{ L"mixamorig:LeftUpLeg",    40.0f, 30.0f, -40.0f },
		{ L"mixamorig:LeftLeg",      30.0f, 25.0f },
		{ L"mixamorig:LeftFoot",     10.0f, 22.0f },
		{ L"mixamorig:RightUpLeg",   40.0f, 30.0f, -40.0f },
		{ L"mixamorig:RightLeg",     30.0f, 25.0f },
		{ L"mixamorig:RightFoot",    10.0f, 22.0f },
	};
}

Player::Player(PlayerID id, int modelHandle, BattleManager* battleManager) :
	m_playerID(id),
	m_modelHandle(-1),
	m_battleManager(battleManager)
{
	//モデル複製
	m_modelHandle = MV1DuplicateModel(modelHandle);
	//アニメーションコントローラー生成
	m_animationController = std::make_unique<AnimationController>(m_modelHandle);
	//================アニメーション登録======================
	//モデルに入っているアニメーションをすべて登録する
	//(技を追加したときにここを書き足さなくていいように)
	const int animNum = MV1GetAnimNum(m_modelHandle);
	for (int i = 0; i < animNum; i++)
	{
		m_animationController->AddAnimation(MV1GetAnimName(m_modelHandle, i));
	}
	//========================================================
	
	//初期アニメーション再生
	m_animationController->Play(kIdleAnimName, true);

	//ステート初期化
	m_state = std::make_unique<PlayerIdleState>(*this);

	//当たり判定生成
	m_hitBoxes = std::make_unique<HitBoxSet>(m_playerID,m_modelHandle);
	//押し出し判定を設定
	m_hitBoxes->SetPushBox(kPushRadius, kPushHeight);
	//やられ判定を設定
	for (const auto& def : kHurtBoxes)
	{
		m_hitBoxes->AddHurtBox(def);
	}
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Init()
{
	if (m_playerID == PlayerID::P1)
	{
		// 1Pの初期化処理
		m_transform.SetPosition(kPlayer1InitPos);
		m_transform.SetScale({ kPlayer1Scale, kPlayer1Scale, kPlayer1Scale });
		MV1SetMatrix(m_modelHandle, m_transform.GetWorldMatrix().ChangeDxMat());
	}
	else
	{
		// 2Pの初期化処理
		m_transform.SetPosition(kPlayer2InitPos);
		m_transform.SetScale({ kPlayer2Scale, kPlayer2Scale, kPlayer2Scale });
		MV1SetMatrix(m_modelHandle, m_transform.GetWorldMatrix().ChangeDxMat());
	}
}

void Player::End()
{
}

void Player::Update()
{
	assert(m_state);

	m_state->Update();

	OpponentRotate();

	ApplyDxLibMatrix();

	m_animationController->Update();

#ifdef _DEBUG
	auto& input = InputManager::GetInstance();
	StickDirection direction = input.GetStickDirection(m_playerID);

	const int x = 100 + static_cast<int>(m_playerID) * 200;
	const int y = 500;

	const int normalColor = GetColor(255, 255, 255);
	const int activeColor = GetColor(255, 0, 0);

	// 現在の方向と一致しているか
	auto GetColorByDirection =
		[&](StickDirection target)
		{
			return direction == target ? activeColor : normalColor;
		};

	// 上段
	DrawFormatString(
		x, y,
		GetColorByDirection(StickDirection::upLeft),
		L"↖"
	);

	DrawFormatString(
		x + 20, y,
		GetColorByDirection(StickDirection::up),
		L"↑"
	);

	DrawFormatString(
		x + 40, y,
		GetColorByDirection(StickDirection::upRight),
		L"↗"
	);

	// 中段
	DrawFormatString(
		x, y + 20,
		GetColorByDirection(StickDirection::left),
		L"←"
	);

	DrawFormatString(
		x + 20, y + 20,
		GetColorByDirection(StickDirection::neutral),
		L"○"
	);

	DrawFormatString(
		x + 40, y + 20,
		GetColorByDirection(StickDirection::right),
		L"→"
	);

	// 下段
	DrawFormatString(
		x, y + 40,
		GetColorByDirection(StickDirection::downLeft),
		L"↙"
	);

	DrawFormatString(
		x + 20, y + 40,
		GetColorByDirection(StickDirection::down),
		L"↓"
	);

	DrawFormatString(
		x + 40, y + 40,
		GetColorByDirection(StickDirection::downRight),
		L"↘"
	);

#endif // _DEBUG
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	//プレイヤーのポジションのデバッグ表示
	DrawFormatString(10, 50 + static_cast<int>(m_playerID) * 20, 0xffffff, L"Player %d Position: (%.2f, %.2f, %.2f)", static_cast<int>(m_playerID), m_transform.GetPosition().x, m_transform.GetPosition().y, m_transform.GetPosition().z);
	//当たり判定のデバッグ表示
	m_hitBoxes->Draw();
#endif // _DEBUG

}

void Player::ChangeState(std::unique_ptr<PlayerStateBase> newState)
{
	if (m_state)
	{
		m_state->Exit();
	}

	m_state = std::move(newState);

	m_state->Enter();
}

void Player::OpponentRotate()
{
	m_transform.SetRotate(
		Quaternion::LookRotation(m_battleManager->GetOpponentDirection(m_playerID),
			Vector3::Up()) * Quaternion::AngleAxis(DX_PI_F, Vector3::Up()));
}

void Player::ApplyDxLibMatrix()
{
	MV1SetMatrix(m_modelHandle, m_transform.GetWorldMatrix().ChangeDxMat());
}
