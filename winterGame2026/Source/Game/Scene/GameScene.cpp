#include "GameScene.h"
#include"SceneController.h"
#include"Engine/Core/Application.h"
#include"Game/Player/Player.h"
#include<DxLib.h>

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

	m_player1 = std::make_shared<Player>();
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

	DrawGrid();

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

void GameScene::DrawGrid()
{
	// 直線の始点と終点
	VECTOR startPos;
	VECTOR endPos;

	for (int z = -500; z <= 500; z += 100)
	{
		startPos = VGet(-500.0f, 0.0f, static_cast<float>(z));
		endPos = VGet(500.0f, 0.0f, static_cast<float>(z));
		DrawLine3D(startPos, endPos, 0xff0000);
	}
	for (int x = -500; x <= 500; x += 100)
	{
		startPos = VGet(static_cast<float>(x), 0.0f, -500.0f);
		endPos = VGet(static_cast<float>(x), 0.0f, 500.0f);
		DrawLine3D(startPos, endPos, 0x0000ff);
	}
}
