#pragma once
#include "SceneBase.h"
#include<memory>
class Player;
class GameScene : public SceneBase
{
public:
	GameScene(SceneController& controller);
	~GameScene();
	void Update()override;
	void Draw()override;

	void DrawGrid();

private:
	//フェードイン更新
	void FadeInUpdate();
	//通常更新
	void NormalUpdate();
	//フェードアウト更新
	void FadeOutUpdate();
	//Updateメンバ関数を代入できるメンバ関数ポインタ
	using UpdateFunc_t = void (GameScene::*)();
	UpdateFunc_t m_update;

	//通常Draw
	void NormalDraw();
	//フェードDraw
	void FadeDraw();
	//Drawメンバ関数を代入できるメンバ関数ポインタ
	using DrawFunc_t = void(GameScene::*)();
	DrawFunc_t m_draw;

	//フェード用のフレームカウンター
	int m_frame;

	std::shared_ptr<Player>m_player1;
	std::shared_ptr<Player>m_player2;
};

