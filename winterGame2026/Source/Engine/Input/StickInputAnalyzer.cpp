#include "StickInputAnalyzer.h"
#include<DxLib.h>

namespace
{
	//デバッグ表示する履歴の最大行数
	constexpr int kDebugDrawMaxLine = 20;
	//デバッグ表示の行の高さ
	constexpr int kDebugDrawLineHeight = 18;

	//デバッグ表示の矢印の半分の長さ
	constexpr float kArrowHalfLength = 7.0f;
	//デバッグ表示の矢印の先端の大きさ
	constexpr float kArrowHeadSize = 5.0f;
	//矢印の右に表示する文字のずらし幅
	constexpr int kArrowTextOffset = 24;

	/// <summary>
	/// 方向を画面上の向き(右がx+、下がy+)に変換する
	/// </summary>
	/// <returns>ニュートラルならfalse</returns>
	bool ToScreenVector(StickDirection direction, float& x, float& y)
	{
		//斜めの長さを1にするための値
		constexpr float d = 0.7071f;

		switch (direction)
		{
		case StickDirection::up:		x = 0.0f;	y = -1.0f;	return true;
		case StickDirection::down:		x = 0.0f;	y = 1.0f;	return true;
		case StickDirection::left:		x = -1.0f;	y = 0.0f;	return true;
		case StickDirection::right:		x = 1.0f;	y = 0.0f;	return true;
		case StickDirection::upLeft:	x = -d;		y = -d;		return true;
		case StickDirection::upRight:	x = d;		y = -d;		return true;
		case StickDirection::downLeft:	x = -d;		y = d;		return true;
		case StickDirection::downRight:	x = d;		y = d;		return true;
		default:													return false;
		}
	}

	/// <summary>
	/// 方向を矢印で描画する(ニュートラルは丸)
	/// </summary>
	/// <param name="cx">中心X</param>
	/// <param name="cy">中心Y</param>
	void DrawDirectionArrow(float cx, float cy, StickDirection direction, unsigned int color)
	{
		float vx = 0.0f;
		float vy = 0.0f;
		if (!ToScreenVector(direction, vx, vy))
		{
			//ニュートラルは小さい丸
			DrawCircleAA(cx, cy, 3.0f, 16, color, true);
			return;
		}
		//矢印の根元と先端
		float tailX = cx - vx * kArrowHalfLength;
		float tailY = cy - vy * kArrowHalfLength;
		float tipX = cx + vx * kArrowHalfLength;
		float tipY = cy + vy * kArrowHalfLength;
		//先端の三角形の付け根(向きに垂直な方向に広げる)
		float baseX = tipX - vx * kArrowHeadSize;
		float baseY = tipY - vy * kArrowHeadSize;
		float px = -vy * kArrowHeadSize;
		float py = vx * kArrowHeadSize;

		DrawLineAA(tailX, tailY, baseX, baseY, color, 2.0f);
		DrawTriangleAA(tipX, tipY, baseX + px, baseY + py, baseX - px, baseY - py, color, true);
	}

	//履歴の最大サイズ
	constexpr size_t kMaxHistorySize = 30;
	//一瞬とみなす最大フレーム数
	constexpr int kQuickInputMaxFrame = 8;
	//連続で倒したとみなす、間のニュートラル最大フレーム
	constexpr int kDoubleTapNeutralMaxFrame = 10;
	//連続で倒したとみなす、一回目の入力の最大フレーム
	constexpr int kDoubleTapFirstMaxFrame = 10;
}

void StickInputAnalyzer::Update(StickDirection direction)
{
	//フレームを進める
	++m_currentFrame;
	//前のフレームと同じ方向なら継続
	if (direction == m_currentDirection)
	{
		if (!m_history.empty())
		{
			m_history.back().endFrame = m_currentFrame;
		}

		return;
	}
	//現在の入力
	m_currentDirection = direction;
	//履歴に追加
	AddRecord(direction);
	//古い履歴削除
	RemoveOldRecords();
}

void StickInputAnalyzer::Clear()
{
	m_history.clear();
	m_currentDirection = StickDirection::neutral;
	m_currentFrame = 0;
}

bool StickInputAnalyzer::IsQuickInput(StickDirection direction) const
{
	//履歴が2つ以上無ければfalse
	if (m_history.size() < 2)
	{
		return false;
	}
	//一番新しい入力
	const auto& current = m_history[m_history.size() - 1];
	//ひとつ前の入力
	const auto& prev = m_history[m_history.size() - 2];
	
	//ニュートラルに戻った瞬間かどうかを判定
	if (current.direction != StickDirection::neutral)
	{
		return false;
	}
	if (current.startFrame != m_currentFrame)
	{
		return false;
	}
	//ひとつ前の入力でその方向に倒しているか
	if (prev.direction != direction)
	{
		return false;
	}
	//一瞬の範囲内で倒していたか
	return prev.GetDuration() <= kQuickInputMaxFrame;
}

bool StickInputAnalyzer::IsDoubleTap(StickDirection direction) const
{
	//履歴が3つ以上無ければfalse
	if (m_history.size() < 3)
	{
		return false;
	}
	//一番新しい入力
	const auto& Third = m_history[m_history.size() - 1];
	//ひとつ前の入力
	const auto& second = m_history[m_history.size() - 2];
	//さらにひとつ前の入力
	const auto& first = m_history[m_history.size() - 3];

	//一番新しい入力が指定の方向に今倒されたか
	if (Third.direction != direction)
	{
		return false;
	}
	if (Third.startFrame != m_currentFrame)
	{
		return false;
	}
	//間の入力に短いニュートラルがあるか
	if (second.direction != StickDirection::neutral)
	{
		return false;
	}
	if (second.GetDuration() > kDoubleTapNeutralMaxFrame)
	{
		return false;
	}
	//一回目の入力が指定の方向に倒されているか
	if (first.direction != direction)
	{
		return false;
	}
	//一回目の入力が短く倒されているか
	return first.GetDuration() <= kDoubleTapFirstMaxFrame;
}

void StickInputAnalyzer::DebugDraw(int x, int y) const
{
	DrawFormatString(x, y, 0xffffff, L"frame : %d", m_currentFrame);

	int line = 1;
	//新しい入力から順に表示
	for (auto it = m_history.rbegin(); it != m_history.rend(); ++it)
	{
		if (line > kDebugDrawMaxLine)
		{
			break;
		}
		//今入力中のものは黄色、それ以外は白
		unsigned int color = (it == m_history.rbegin()) ? 0xffff00 : 0xffffff;

		int lineY = y + kDebugDrawLineHeight * line;
		//方向の矢印(文字の高さの中央に合わせる)
		DrawDirectionArrow(static_cast<float>(x + 8), static_cast<float>(lineY + 8), it->direction, color);
		//継続フレームと開始～終了フレーム
		DrawFormatString(x + kArrowTextOffset, lineY, color,
			L"%dF (%d-%d)",
			it->GetDuration(), it->startFrame, it->endFrame);

		++line;
	}
}

void StickInputAnalyzer::AddRecord(StickDirection direction)
{
	StickInputRecord record;

	record.direction = direction;
	record.startFrame = m_currentFrame;
	record.endFrame = m_currentFrame;

	m_history.push_back(record);
}

void StickInputAnalyzer::RemoveOldRecords()
{
	while (m_history.size() > kMaxHistorySize)
	{
		m_history.pop_front();
	}
}
