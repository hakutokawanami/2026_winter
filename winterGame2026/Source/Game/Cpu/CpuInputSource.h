#pragma once
#include "Engine/Input/IInputSource.h"
#include<array>
#include<random>

class Player;
class CpuInputSource : public IInputSource
{
public:
	/// <summary>
	/// CPUは自分の情報を知りたいので自分のプレイヤーの参照を受け取る
	/// </summary>
	/// <param name="self">自分のプレイヤーの参照</param>
	explicit CpuInputSource(const Player& self);
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	bool IsTriggerd(Button button)const override;
	StickDirection GetStickDirection()const override;
	bool IsStickDoubleTap(StickDirection dir)const override;

private:
	/// <summary>
	/// CPUの行動
	/// </summary>
	enum class Action 
	{
		wait,
		forward,
		back,
		guard,
		attack,
		combo
	};
	/// <summary>
	/// 次の行動を決める
	/// </summary>
	void Think();
	/// <summary>
	/// 行動を始める
	/// </summary>
	/// <param name="action">行動</param>
	/// <param name="frame">何フレーム続けるか</param>
	void StartAction(Action action,int frame);
	/// <summary>
	/// プレイヤーの向いている方向に応じて入力を反転させる関数
	/// </summary>
	/// <param name="dir"></param>
	/// <returns></returns>
	StickDirection ToScreen(StickDirection dir)const;
	/// <summary>
	/// 0～99の乱数を返す
	/// </summary>
	/// <returns></returns>
	int Random100();

private:
	//自分のプレイヤーの参照
	const Player& m_self;
	//今の行動
	Action m_action;
	//今の行動を続ける残りフレーム
	int m_actionTimer;
	//コンボで連打するボタン
	Button m_attackButton;
	//反応するまでの残りフレーム
	int m_reactionTimer;
	//前のフレームに相手が攻撃していたか
	bool m_wasOpponentAttacking;
	//前のフレームに相手が投げ攻撃していたか
	bool m_wasOpponentThrowing;
	//このフレームの入力
	StickDirection m_stick;
	std::array<bool, static_cast<size_t>(Button::count)>m_triggered;
	StickDirection m_doubleTap;
	//乱数
	std::mt19937 m_random;
};

