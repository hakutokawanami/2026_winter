#pragma once
#include "SceneBase.h"
#include"Game/Common/MatchSetting.h"

class ModeSelectScene : public SceneBase
{
public:
	ModeSelectScene(SceneController& controller);
	~ModeSelectScene();
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
	using UpdateFunc_t = void (ModeSelectScene::*)();
	UpdateFunc_t m_update;

	//通常Draw
	void NormalDraw();
	//フェードDraw
	void FadeDraw();
	//Drawメンバ関数を代入できるメンバ関数ポインタ
	using DrawFunc_t = void(ModeSelectScene::*)();
	DrawFunc_t m_draw;

	//フェード用のフレームカウンター
	int m_frame;
	//選んでいる項目の番号
	int m_cursor;
	//次のシーンに渡す設定
	MatchSetting m_matchSetting;
	//戻るかどうか
	bool m_isBack;
};

