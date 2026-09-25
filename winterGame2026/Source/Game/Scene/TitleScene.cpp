#include "TitleScene.h"
#include"SceneController.h"
#include"Engine/Core/Application.h"
#include"DxLib.h"

namespace
{
	//フェードにかかる時間
	constexpr int kFadeInterval = 60;
}

TitleScene::TitleScene(SceneController& controller) :
	SceneBase(controller)
{
	//フェードイン
	m_update = &TitleScene::FadeInUpdate;
	//フェード用描画
	m_draw = &TitleScene::FadeDraw;
	//フェード用のフレームカウンター初期化
	m_frame = kFadeInterval;
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	(this->*m_update)();
}

void TitleScene::Draw()
{
	(this->*m_draw)();
}

void TitleScene::FadeInUpdate()
{
	if (--m_frame <= 0)
	{
		m_update = &TitleScene::NormalUpdate;
		m_draw = &TitleScene::NormalDraw;
		return;
	}
}

void TitleScene::NormalUpdate()
{
	
}

void TitleScene::FadeOutUpdate()
{
	if (++m_frame >= kFadeInterval)
	{
		//シーンの切り替え
//		m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));
		return;
	}
}

void TitleScene::NormalDraw()
{
	auto& wsize = Application::GetInstance().GetWindowSize();
#ifdef _DEBUG
	DrawFormatString(static_cast<int>(wsize.w / 2.0f), static_cast<int>(wsize.h / 2.0f), 0xffffff, L"TITLESCENE");
#endif // _DEBUG
}

void TitleScene::FadeDraw()
{
	NormalDraw();

	auto& wsize = Application::GetInstance().GetWindowSize();
	//フェード処理
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast <int>(255 * rate));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, static_cast <int>(255 * rate));
}
