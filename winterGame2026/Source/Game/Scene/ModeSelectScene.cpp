#include "ModeSelectScene.h"
#include"GameScene.h"
#include"SceneController.h"
#include"Engine/Input/InputManager.h"
#include"Engine/Core/Application.h"
#include"DxLib.h"

namespace
{
	//フェードにかかる時間
	constexpr int kFadeInterval = 60;
}

ModeSelectScene::ModeSelectScene(SceneController& controller) :
	SceneBase(controller)
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

	if (input.AnyPressButtonInput())
	{
		m_update = &ModeSelectScene::FadeOutUpdate;
		m_draw = &ModeSelectScene::FadeDraw;
	}
}

void ModeSelectScene::FadeOutUpdate()
{
	if (++m_frame >= kFadeInterval)
	{
		//シーンの切り替え
		m_controller.ChangeScene(std::make_shared<GameScene>(m_controller, MatchSetting{}));
		return;
	}
}

void ModeSelectScene::NormalDraw()
{
	auto& wsize = Application::GetInstance().GetWindowSize();
#ifdef _DEBUG
	DrawFormatString(static_cast<int>(wsize.w / 2.0f), static_cast<int>(wsize.h / 2.0f), 0xffffff, L"TITLESCENE");
#endif // _DEBUG
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