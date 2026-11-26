#pragma once
#include "SceneBase.h"
#include"Game/Common/MatchSetting.h"
#include"Game/Common/PlayerID.h"

//一人分の選択状態
struct Selector
{
	int cursor = 0;
	bool isDecided = false;
};

class CharacterSelectScene : public SceneBase
{
public:
	CharacterSelectScene(SceneController& controller,const MatchSetting& matchSetting);
	~CharacterSelectScene();
	void Update()override;
	void Draw()override;

	//一人分の更新
	bool UpdateSelector(Selector& selector, PlayerID id);

private:
	//フェードイン更新
	void FadeInUpdate();
	//通常更新
	void NormalUpdate();
	//フェードアウト更新
	void FadeOutUpdate();
	//Updateメンバ関数を代入できるメンバ関数ポインタ
	using UpdateFunc_t = void (CharacterSelectScene::*)();
	UpdateFunc_t m_update;

	//通常Draw
	void NormalDraw();
	//フェードDraw
	void FadeDraw();
	//Drawメンバ関数を代入できるメンバ関数ポインタ
	using DrawFunc_t = void(CharacterSelectScene::*)();
	DrawFunc_t m_draw;

	//フェード用のフレームカウンター
	int m_frame;
	//次のシーンに渡す設定
	MatchSetting m_matchSetting;
	//戻るかどうか
	bool m_isBack;
	//二人分
	Selector m_selectors[2];
};

