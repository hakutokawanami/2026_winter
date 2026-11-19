#include "PlayerDownState.h"
#include"PlayerGetUpState.h"
namespace
{
	//ダウンの最大フレーム数
	constexpr int kDownFrame = 30;

	//先行入力を受け付けるフレーム数(この数以内に押されていたら技を出す)
	constexpr int kBufferFrame = 10;
}

void PlayerDownState::Enter()
{
	m_animationController.Play(Player::kDownAnimName);
}

void PlayerDownState::Update()
{
	m_frame++;

	if (m_frame >= kDownFrame)
	{
		if (IsBufferd(Button::A, kBufferFrame) ||
			IsBufferd(Button::B, kBufferFrame) ||
			IsBufferd(Button::X, kBufferFrame) ||
			IsBufferd(Button::Y, kBufferFrame))
		{
			m_player.ChangeState(std::make_unique<PlayerGetUpState>(m_player, GetUp::attack));
			return;
		}

		if (GetStickDirection() == ToScreenDirection(StickDirection::left) ||
			GetStickDirection() == ToScreenDirection(StickDirection::downLeft))
		{
			m_player.ChangeState(std::make_unique<PlayerGetUpState>(m_player, GetUp::rollBack));
			return;
		}

		m_player.ChangeState(std::make_unique<PlayerGetUpState>(m_player,GetUp::normal));
		return;
	}
}

void PlayerDownState::Exit()
{
}
