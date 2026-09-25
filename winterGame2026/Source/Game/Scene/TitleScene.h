#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase
{
public:
	TitleScene(SceneController& controller);
	~TitleScene();
	void Update()override;
	void Draw()override;

private:
	//フェードイン更新
	void FadeInUpdate();
	//通常更新
	void NormalUpdate();
	//フェードアウト更新
	void FadeOutUpdate();
	//Updateメンバ関数を代入できるメンバ関数ポインタ
	using UpdateFunc_t = void (TitleScene::*)();
	UpdateFunc_t m_update;

	//通常Draw
	void NormalDraw();
	//フェードDraw
	void FadeDraw();
	//Drawメンバ関数を代入できるメンバ関数ポインタ
	using DrawFunc_t = void(TitleScene::*)();
	DrawFunc_t m_draw;

	//フェード用のフレームカウンター
	int m_frame;
};