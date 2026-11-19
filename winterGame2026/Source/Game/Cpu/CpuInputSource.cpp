#include "CpuInputSource.h"
#include"Game/Player/Player.h"
#include"Game/Battle/BattleManager.h"

namespace
{
	//コンボや投げが届く距離
	constexpr float kNearDistance = 180.0f;	
	//これより遠いと近づく
	constexpr float kFarDistance = 450.0f;	

	//相手の攻撃を見てからガードするまでのフレーム数
	constexpr int kReactionFrame = 8;
	//相手の攻撃をガードしようとする確率(%)
	constexpr int kGuardRate = 70;
	//相手の投げを抜けようとする確率(%)
	constexpr int kGrabRate = 70;
	//コンボを最後まで出そうとする確率(%)
	constexpr int kComboRate = 70;
	//コンボ中にボタンを押す間隔
	constexpr int kComboPressInterval = 6;
	//ガードし続けるフレーム
	constexpr int kGuardActionFrame = 20;
}

CpuInputSource::CpuInputSource(const Player& self):
	m_self(self),
	m_action(Action::wait),
	m_actionTimer(0),
	m_attackButton(Button::X),
	m_reactionTimer(-1),
	m_wasOpponentAttacking(false),
	m_wasOpponentThrowing(false),
	m_stick(StickDirection::neutral),
	m_triggered{},
	m_doubleTap(StickDirection::neutral),
	m_random(std::random_device{}())
{
}

void CpuInputSource::Update()
{
	//ボタンとダブルタップは「押した1フレームだけ」なので毎フレーム消す
	m_triggered.fill(false);
	m_doubleTap = StickDirection::neutral;

	//相手がいなかったらreturn
	const Player* opponent = m_self.GetBattleManager()->GetOpponentPlayer(m_self.GetPlayerID());
	if (!opponent)
	{
		return;
	}

	//攻撃中かどうか
	const bool isOpponentAttacking = opponent->IsAttacking();
	//相手が攻撃を出した瞬間にガードするかどうかを決める
	if (isOpponentAttacking && !m_wasOpponentAttacking && Random100() < kGuardRate)
	{
		m_reactionTimer = kReactionFrame;
	}
	//前のフレームに攻撃していたかどうかを保存
	m_wasOpponentAttacking = isOpponentAttacking;

	//反応が間に合ったらガードに切り替える
	if (m_reactionTimer >= 0 && --m_reactionTimer < 0)
	{
		StartAction(Action::guard, kGuardActionFrame);
	}

	//投げ攻撃中かどうか
	const bool isOpponentThrowing = opponent->IsThrowing();
	//相手が投げ攻撃を出した瞬間に投げ抜けするかどうかを決める
	if (isOpponentThrowing && !m_wasOpponentThrowing && Random100() < kGrabRate)
	{
		m_reactionTimer = kReactionFrame;
	}
	//前のフレームに攻撃していたかどうかを保存
	m_wasOpponentThrowing = isOpponentThrowing;

	//反応が間に合ったら投げに切り替える
	if (m_reactionTimer >= 0 && --m_reactionTimer < 0)
	{
		//投げ
		m_triggered[static_cast<size_t>(Button::B)] = true;
		StartAction(Action::attack, 1);
	}

	//行動が終わったら次の行動を決める
	if (--m_actionTimer <= 0)
	{
		Think();
	}

	switch (m_action)
	{
	case Action::wait:
		m_stick = StickDirection::neutral;
		break;
	case Action::forward:
		m_stick = ToScreen(StickDirection::right);
		break;
	case Action::back:
		m_stick = ToScreen(StickDirection::left);
		break;
	case Action::guard:
		m_stick = ToScreen(StickDirection::left);
		break;
	case Action::attack:
		m_stick = StickDirection::neutral;
		break;
	case Action::combo:
		//一定間隔でボタンを押して、先行入力でコンボをつなげる
		if (m_actionTimer % kComboPressInterval == 0)
		{
			m_triggered[static_cast<size_t>(m_attackButton)] = true;
		}
		break;
	}
}

bool CpuInputSource::IsTriggerd(Button button) const
{
	return m_triggered[static_cast<size_t>(button)];
}

StickDirection CpuInputSource::GetStickDirection() const
{
	 return m_stick;
}

bool CpuInputSource::IsStickDoubleTap(StickDirection dir) const
{
	return dir != StickDirection::neutral && dir == m_doubleTap;
}

void CpuInputSource::Think()
{
	const float distance = m_self.GetBattleManager()->GetDistance();
	const int random = Random100();

	//相手との距離が遠いとき
	if (distance > kFarDistance)
	{
		if (random < 20)
		{
			//ステップ
			m_doubleTap = ToScreen(StickDirection::right);
			StartAction(Action::wait, 20);
		}
		else
		{
			//前に進む
			StartAction(Action::forward, 30);
		}

		return;
	}
	//相手との距離が中距離のとき
	if (distance > kNearDistance)
	{
		if (random < 25)
		{
			//踏み込み攻撃
			m_attackButton = Button::Y;
			StartAction(Action::attack, 1);
			m_triggered[static_cast<size_t>(Button::Y)] = true;
		}
		else if (random < 65)
		{
			//前に進む
			StartAction(Action::forward, 20);
		}
		else if (random < 80)
		{
			//後ろに下がる
			StartAction(Action::back, 15);
		}
		else
		{
			//その場で待つ
			StartAction(Action::wait, 20);
		}
		return;
	}

	//相手との距離が近いとき
	if (random < 40)
	{
		//コンボ攻撃
		m_attackButton = Button::X;
		m_triggered[static_cast<size_t>(Button::X)] = true;
		//最後まで出すときは、コンボ全体の長さだけボタンを押し続ける
		const bool fullCombo = Random100() < kComboRate;
		StartAction(fullCombo ? Action::combo : Action::attack, fullCombo ? 60 : 1);
	}
	else if (random < 55)
	{
		//キック
		m_triggered[static_cast<size_t>(Button::A)] = true;
		StartAction(Action::attack, 1);
	}
	else if (random < 70)
	{
		//投げ
		m_triggered[static_cast<size_t>(Button::B)] = true;
		StartAction(Action::attack, 1);
	}
	else if (random < 85)
	{
		//後ろに下がる
		StartAction(Action::back, 20);
	}
	else
	{
		//その場で待つ
		StartAction(Action::wait, 15);
	}
}

void CpuInputSource::StartAction(Action action, int frame)
{
	m_action = action;
	m_actionTimer = frame;
}

StickDirection CpuInputSource::ToScreen(StickDirection dir) const
{
	//相手が右にいるなら右が前、左にいるなら左右を反転する
	const bool isRight = m_self.GetBattleManager()->IsOpponentOnRight(m_self.GetPlayerID());
	return isRight ? dir : Mirror(dir);
}

int CpuInputSource::Random100()
{
	return std::uniform_int_distribution<int>(0, 99)(m_random);
}
