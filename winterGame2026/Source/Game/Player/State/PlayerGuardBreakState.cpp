#include "PlayerGuardBreakState.h"
#include "PlayerIdleState.h"

namespace
{
	//ガードブレイクで動けないフレーム数(弾かれる+ふらふらの合計)
	//(2つのアニメーションをほぼ等速で再生しきる長さ。短くするとふらふらが速く再生される)
	constexpr int kGuardBreakFrame = 170;
	//弾かれるアニメーションのフレーム数(ここまでが1段目、ここからふらふら)
	constexpr int kBreakAnimFrame = 51;

	//弾かれるアニメーションのブレンドにかけるフレーム数(ガードの構えからすぐ切り替える)
	constexpr float kBreakBlendTime = 5.0f;
	//ふらふらに切り替えるときのブレンドにかけるフレーム数
	constexpr float kStunBlendTime = 8.0f;
}

PlayerGuardBreakState::PlayerGuardBreakState(Player& player) :
	PlayerStateBase(player),
	m_frame(0)
{
}

void PlayerGuardBreakState::Enter()
{
	//1段目：弾かれるアニメーションを最初から再生
	m_animationController.Play(Player::kGuardBreakAnimName, false, kBreakBlendTime, 0.5f, true);
	//kBreakAnimFrameぴったりで終わる速さにする
	m_animationController.SetSpeed(m_animationController.GetTotalTime() / kBreakAnimFrame);

	m_animationController.RootLocked();
}

void PlayerGuardBreakState::Update()
{
	m_frame++;

	//2段目：弾かれ終わったらふらふらに切り替える(1回だけ)
	if (m_frame == kBreakAnimFrame)
	{
		m_animationController.Play(Player::kStunAnimName, false, kStunBlendTime, 0.5f, true);
		//残りの時間ぴったりで終わる速さにする
		m_animationController.SetSpeed(m_animationController.GetTotalTime() / (kGuardBreakFrame - kBreakAnimFrame));
	}

	if (m_frame >= kGuardBreakFrame)
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player));
		return;
	}
}

void PlayerGuardBreakState::Exit()
{
	//ブレイクが終わったらゲージを満タンに戻す
	//(時間で終わったときも、ブレイク中に攻撃を食らったときも必ず通る)
	m_player.ResetGuardHp();
}
