#include "ModeSelectScene.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"SceneController.h"
#include"Engine/Input/InputManager.h"
#include"Engine/Core/Application.h"
#include"DxLib.h"

namespace
{
	//フェードにかかる時間
	constexpr int kFadeInterval = 60;

	struct ModeItem
	{
		const wchar_t* name;
		GameMode mode;
	};
	constexpr ModeItem kModeItems[] =
	{
		{ L"対CPU",       GameMode::vsCpu },
		{ L"対人",        GameMode::vsPlayer },
		{ L"トレーニング", GameMode::training },
	};
	constexpr int kItemCount = static_cast<int>(std::size(kModeItems));
}

ModeSelectScene::ModeSelectScene(SceneController& controller) :
	SceneBase(controller),
	m_cursor(0),
	m_isBack(false)
{
	//フェードイン
	m_update = &ModeSelectScene::FadeInUpdate;
	//フェード用描画
	m_draw = &ModeSelectScene::FadeDraw;
	//フェード用のフレームカウンター初期化
	m_frame = kFadeInterval;
}

ModeSelectScene::~ModeSelectScene()
{
}

void ModeSelectScene::Update()
{
	(this->*m_update)();
}

void ModeSelectScene::Draw()
{
	(this->*m_draw)();
}

void ModeSelectScene::FadeInUpdate()
{
	if (--m_frame <= 0)
	{
		m_update = &ModeSelectScene::NormalUpdate;
		m_draw = &ModeSelectScene::NormalDraw;
		return;
	}
}

void ModeSelectScene::NormalUpdate()
{
	auto& input = InputManager::GetInstance();
	input.Update();

	if (input.IsTriggered(PlayerID::P1, "up"))
	{
		m_cursor = (m_cursor + kItemCount - 1) % kItemCount;
	}

	if (input.IsTriggered(PlayerID::P1, "down"))
	{
		m_cursor = (m_cursor + 1) % kItemCount;
	}

	if (input.IsTriggered(PlayerID::P1,"A"))
	{
		m_matchSetting.gameMode = kModeItems[m_cursor].mode;
		m_update = &ModeSelectScene::FadeOutUpdate;
		m_draw = &ModeSelectScene::FadeDraw;
	}

	if (input.IsTriggered(PlayerID::P1, "B"))
	{
		m_isBack = true;
		m_update = &ModeSelectScene::FadeOutUpdate;
		m_draw = &ModeSelectScene::FadeDraw;
	}
}

void ModeSelectScene::FadeOutUpdate()
{
	if (++m_frame >= kFadeInterval)
	{
		if (m_isBack)
		{
			m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
			return;
		}
		else
		{
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller, m_matchSetting));
		}
	}
}

void ModeSelectScene::NormalDraw()
{
	auto& wsize = Application::GetInstance().GetWindowSize();

	//見出しの高さ(画面の高さに対する割合)
	constexpr float kTitleRate = 0.25f;
	//1つ目の項目の高さ(画面の高さに対する割合)
	constexpr float kItemStartRate = 0.45f;
	//項目同士の間隔
	constexpr int kItemSpace = 40;
	//カーソルの印と項目の間
	constexpr int kCursorOffset = 30;
	//選ばれている項目の色
	constexpr unsigned int kSelectColor = 0xffd040;
	//選ばれていない項目の色
	constexpr unsigned int kNormalColor = 0x808080;

	//見出し(画面の中央にそろえる)
	const wchar_t* title = L"MODE SELECT";
	int titleWidth = GetDrawStringWidth(title, static_cast<int>(wcslen(title)));
	DrawString((wsize.w - titleWidth) / 2, static_cast<int>(wsize.h * kTitleRate), title, 0xffffff);

	//項目
	for (int i = 0; i < kItemCount; i++)
	{
		const wchar_t* name = kModeItems[i].name;
		int width = GetDrawStringWidth(name, static_cast<int>(wcslen(name)));
		int x = (wsize.w - width) / 2;
		int y = static_cast<int>(wsize.h * kItemStartRate) + i * kItemSpace;

		if (i == m_cursor)
		{
			//選ばれている項目は色を変えて、左に印を付ける
			DrawString(x - kCursorOffset, y, L"▶", kSelectColor);
			DrawString(x, y, name, kSelectColor);
		}
		else
		{
			DrawString(x, y, name, kNormalColor);
		}
	}

	//操作説明
	const wchar_t* guide = L"↑↓：選択　A：決定　B：戻る";
	int guideWidth = GetDrawStringWidth(guide, static_cast<int>(wcslen(guide)));
	DrawString((wsize.w - guideWidth) / 2, wsize.h - 60, guide, 0xffffff);
}

void ModeSelectScene::FadeDraw()
{
	NormalDraw();

	auto& wsize = Application::GetInstance().GetWindowSize();
	//フェード処理
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast <int>(255 * rate));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, static_cast <int>(255 * rate));
}