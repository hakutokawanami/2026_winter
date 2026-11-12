#include "BattleUI.h"
#include"Engine/Core/Application.h"
#include"Game/Player/Player.h"
#include"RoundManager.h"
#include<DxLib.h>
#include<algorithm>

namespace
{
	//HPバー(勝利ラウンドの四角の位置にも使う)
	constexpr int kBarWidth = 500;
	constexpr int kBarHeight = 30;
	constexpr int kMarginX = 40;
	constexpr int kBarY = 40;

	//勝利ラウンドの四角
	constexpr int kWinBoxWidth = 28;
	constexpr int kWinBoxHeight = 14;
	//四角同士の間隔
	constexpr int kWinBoxGap = 8;
	//HPバーとの間隔
	constexpr int kWinBoxMarginY = 6;
	//点灯している色・消えている色
	constexpr unsigned int kWinOnColor = 0xffd040;
	constexpr unsigned int kWinOffColor = 0x303030;

	//フェーズの文字
	//最初の大きさ・最後の大きさ
	constexpr double kTextStartScale = 1.0;
	constexpr double kTextEndScale = 5.0;
	//大きくなりきるまでのフレーム数
	constexpr int kTextGrowFrame = 20;
	//消えていくフレーム数
	constexpr int kTextFadeFrame = 15;
	//「FIGHT!」を出しておくフレーム数
	constexpr int kFightTextFrame = 60;

	/// <summary>
	/// 文字を画面中央に、だんだん大きくしながら描画する
	/// </summary>
	/// <param name="text">文字</param>
	/// <param name="frame">表示を始めてからのフレーム数</param>
	/// <param name="color">文字の色</param>
	/// <param name="showFrame">表示するフレーム数(0以下ならずっと表示)</param>
	void DrawGrowText(const wchar_t* text, int frame, unsigned int color, int showFrame = 0)
	{
		if (showFrame > 0 && frame >= showFrame)
		{
			return;
		}

		//最初は速く、最後はゆっくり大きくする(イーズアウト)
		const double t = std::clamp(static_cast<double>(frame) / kTextGrowFrame, 0.0, 1.0);
		const double ease = 1.0 - (1.0 - t) * (1.0 - t);
		const double scale = kTextStartScale + (kTextEndScale - kTextStartScale) * ease;

		//表示の終わりに近づいたら薄くする
		int alpha = 255;
		if (showFrame > 0)
		{
			const int remain = showFrame - frame;
			alpha = std::clamp(255 * remain / kTextFadeFrame, 0, 255);
		}

		//大きさに合わせて中央にそろえる
		auto& wsize = Application::GetInstance().GetWindowSize();
		const int width = static_cast<int>(GetDrawStringWidth(text, static_cast<int>(wcslen(text))) * scale);
		const int height = static_cast<int>(GetFontSize() * scale);
		const int x = (wsize.w - width) / 2;
		const int y = wsize.h / 3 - height / 2;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawExtendString(x, y, scale, scale, text, color, 0x000000);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void BattleUI::Draw(const RoundManager& round, const Player& p1, const Player& p2) const
{
	DrawHpBar(p1);
	DrawHpBar(p2);

	DrawWins(round.GetWins(PlayerID::P1), p1);
	DrawWins(round.GetWins(PlayerID::P2), p2);

	DrawTimer(round.GetTimer());

	DrawPhaseText(round);
}

void BattleUI::DrawHpBar(const Player& player) const
{
	auto& wsize = Application::GetInstance().GetWindowSize();

	const float rate = static_cast<float>(player.GetHp()) / Player::kMaxHp;
	const int fill = static_cast<int>(kBarWidth * rate);
	int x = 0;

	if (player.GetPlayerID() == PlayerID::P1)
	{
		x = kMarginX;
	}
	else if(player.GetPlayerID() == PlayerID::P2)
	{
		x = wsize.w - kMarginX - kBarWidth;
	}

	//背景(減った部分)
	DrawBox(x, kBarY, x + kBarWidth, kBarY + kBarHeight, 0x502020, true);
	//残りHP(画面の中央側から減っていく)
	if (player.GetPlayerID() == PlayerID::P1)
	{
		DrawBox(x, kBarY, x + fill, kBarY + kBarHeight, 0xffd040, true);
	}
	else if (player.GetPlayerID() == PlayerID::P2)
	{
		DrawBox(x + kBarWidth - fill, kBarY, x + kBarWidth, kBarY + kBarHeight, 0xffd040, true);
	}
	//枠
	DrawBox(x, kBarY, x + kBarWidth, kBarY + kBarHeight, 0xffffff, false);
}

void BattleUI::DrawTimer(int seconds) const
{
	auto& wsize = Application::GetInstance().GetWindowSize();
	constexpr int kTimerY = 30;
	constexpr int kExX = 3;
	constexpr int kExY = 3;
	DrawExtendFormatString(wsize.w / 2, kTimerY, kExX, kExY, 0xffffff, L"%d", seconds);
}

void BattleUI::DrawWins(int wins, const Player& player) const
{
	auto& wsize = Application::GetInstance().GetWindowSize();
	const bool isP1 = player.GetPlayerID() == PlayerID::P1;

	//HPバーの上に、画面の中央側から並べる
	const int top = kBarY - kWinBoxMarginY - kWinBoxHeight;
	//中央側の端(P1はバーの右端、P2はバーの左端)
	const int innerX = isP1 ? kMarginX + kBarWidth : wsize.w - kMarginX - kBarWidth;

	for (int i = 0; i < RoundManager::kWinRounds; ++i)
	{
		//中央から外側へ i 個目の四角
		const int offset = i * (kWinBoxWidth + kWinBoxGap);
		const int left = isP1 ? innerX - offset - kWinBoxWidth : innerX + offset;

		//勝った数だけ中央側から点灯
		const unsigned int color = (i < wins) ? kWinOnColor : kWinOffColor;
		DrawBox(left, top, left + kWinBoxWidth, top + kWinBoxHeight, color, true);
		//枠
		DrawBox(left, top, left + kWinBoxWidth, top + kWinBoxHeight, 0xffffff, false);
	}
}

void BattleUI::DrawPhaseText(const RoundManager& round) const
{
	const int frame = round.GetPhaseFrame();

	switch (round.GetPhase())
	{
	case RoundManager::Phase::roundStart:
	{
		wchar_t text[32];
		swprintf_s(text, L"ROUND %d", round.GetRound());
		DrawGrowText(text, frame, 0xffffff);
		break;
	}
	case RoundManager::Phase::fight:
		//始まってすぐだけ出す
		DrawGrowText(L"FIGHT!", frame, 0xff4040, kFightTextFrame);
		break;
	case RoundManager::Phase::ko:
		DrawGrowText(L"K.O.", frame, 0xff4040);
		break;
	case RoundManager::Phase::timeOver:
		DrawGrowText(L"TIME OVER", frame, 0xffd040);
		break;
	default:
		break;
	}
}
