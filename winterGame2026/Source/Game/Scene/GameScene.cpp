#include "GameScene.h"
#include"SceneController.h"
#include"DxLib.h"
#include"Engine/Core/Application.h"

namespace
{
	//フェードにかかる時間
	constexpr int kFadeInterval = 60;
}

GameScene::GameScene(SceneController& controller) :
	SceneBase(controller)
{
	//フェードイン
	m_update = &GameScene::FadeInUpdate;
	//フェード用描画
	m_draw = &GameScene::FadeDraw;
	//フェード用のフレームカウンター初期化
	m_frame = kFadeInterval;
}

GameScene::~GameScene()
{
}

void GameScene::Update()
{
	(this->*m_update)();
}

void GameScene::Draw()
{
	(this->*m_draw)();
}

void GameScene::FadeInUpdate()
{
	if (--m_frame <= 0)
	{
		m_update = &GameScene::NormalUpdate;
		m_draw = &GameScene::NormalDraw;
		return;
	}
}

void GameScene::NormalUpdate()
{

}

void GameScene::FadeOutUpdate()
{
	if (++m_frame >= kFadeInterval)
	{
		//シーンの切り替え
//		m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));
		return;
	}
}

void GameScene::NormalDraw()
{
	auto& wsize = Application::GetInstance().GetWindowSize();
#ifdef _DEBUG
	DrawFormatString(static_cast<int>(wsize.w / 2.0f), static_cast<int>(wsize.h / 2.0f), 0xffffff, L"GAMESCENE");
#endif // _DEBUG
}

void GameScene::FadeDraw()
{
	NormalDraw();

	auto& wsize = Application::GetInstance().GetWindowSize();
	//フェード処理
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast <int>(255 * rate));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, static_cast <int>(255 * rate));
}
