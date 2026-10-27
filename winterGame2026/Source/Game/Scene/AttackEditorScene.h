#pragma once
#include "SceneBase.h"
#include "Game/Battle/BattleCollision.h"
#include<memory>
#include<vector>
#include<string>
#include<array>

class Player;
class BattleManager;
struct AttackDef;

/// <summary>
/// 技のアニメーション速度や発生・持続・硬直を画面で見ながら調整するシーン
/// </summary>
class AttackEditorScene : public SceneBase
{
public:
	AttackEditorScene(SceneController& controller);
	~AttackEditorScene();

	void Update()override;
	void Draw()override;

private:
	/// <summary>
	/// 編集できる項目
	/// </summary>
	enum class Param
	{
		anim,
		animSpeed,
		blendTime,
		bone,
		length,
		radius,
		offsetY,
		startUp,
		active,
		recovery,
		damage,
		hitStun,
		blockStun,
		distance,
		count
	};

	/// <summary>
	/// キーの押されている時間を更新する
	/// </summary>
	void UpdateKeyState();
	/// <summary>
	/// キーが今押されたか
	/// </summary>
	bool IsKeyTriggered(int key)const;
	/// <summary>
	/// キーが押され続けているときに一定間隔でtrueになる
	/// </summary>
	bool IsKeyRepeated(int key)const;
	/// <summary>
	/// キーが押されているか
	/// </summary>
	bool IsKeyPressed(int key)const;

	/// <summary>
	/// 新しい技の名前入力中の更新
	/// </summary>
	void UpdateNameInput();
	/// <summary>
	/// 項目の編集やファイルの保存などの操作
	/// </summary>
	void UpdateEdit();
	/// <summary>
	/// 再生・コマ送り・タイムラインのクリック
	/// </summary>
	void UpdatePlayback();

	/// <summary>
	/// 選んでいる項目の値を変える
	/// </summary>
	/// <param name="dir">+1 か -1</param>
	/// <param name="isFast">大きく変えるかどうか</param>
	void ChangeParam(int dir, bool isFast);
	/// <summary>
	/// 編集する技を切り替える
	/// </summary>
	/// <param name="index">技の番号</param>
	void SelectAttack(size_t index);
	/// <summary>
	/// 攻撃側と相手を入れ替える
	/// </summary>
	void SwapPlayers();
	/// <summary>
	/// 攻撃側に技のアニメーションをセットする
	/// </summary>
	void SetupAttackAnimation();
	/// <summary>
	/// 相手との距離に合わせて二人を配置する
	/// </summary>
	void PlacePlayers();
	/// <summary>
	/// 今のフレームのポーズと判定を反映する
	/// </summary>
	void ApplyFrame();
	/// <summary>
	/// 攻撃判定のボーンが相手の方向にどれだけ伸びているかをフレームごとに調べる
	/// </summary>
	void BuildReachCurve();
	/// <summary>
	/// アニメーションが終わるフレームを取得する
	/// </summary>
	int GetAnimEndFrame()const;
	/// <summary>
	/// タイムラインに表示するフレーム数を取得する
	/// </summary>
	int GetTimelineLength()const;
	/// <summary>
	/// 画面に一定時間メッセージを出す
	/// </summary>
	void ShowMessage(const std::wstring& message);
	/// <summary>
	/// 編集中の技
	/// </summary>
	AttackDef& Current();
	const AttackDef& Current()const;

	/// <summary>
	/// 項目の一覧の描画
	/// </summary>
	void DrawParams();
	/// <summary>
	/// タイムラインとボーンの伸びのグラフの描画
	/// </summary>
	void DrawTimeline();
	/// <summary>
	/// 操作説明の描画
	/// </summary>
	void DrawHelp();
	/// <summary>
	/// 床の描画
	/// </summary>
	void DrawGrid();

private:
	//バトルマネージャー(プレイヤーの向きを決めるのに使う)
	std::unique_ptr<BattleManager>m_battleManager;
	//技を出す側
	std::unique_ptr<Player>m_attacker;
	//技を受ける側
	std::unique_ptr<Player>m_dummy;
	//当たっているかを調べる
	BattleCollision m_collision;

	//モデルに入っているアニメーションの名前
	std::vector<std::wstring>m_animNames;
	//編集中の技の番号
	size_t m_attackIndex;
	//選んでいる項目
	int m_paramIndex;

	//今表示しているフレーム(技を出して何フレーム目か)
	int m_frame;
	//再生中かどうか
	bool m_isPlaying;
	//スロー再生の段階(0:等倍 1:1/2 2:1/4)
	int m_slowLevel;
	//スロー再生用のカウンター
	int m_slowCounter;
	//相手との距離
	float m_distance;
	//攻撃が当たっているかどうか
	bool m_isHit;
	//保存していない変更があるかどうか
	bool m_isDirty;
	//攻撃判定のボーンの伸び(フレームごと)
	std::vector<float>m_reachCurve;

	//メッセージ
	std::wstring m_message;
	//メッセージを表示する残り時間
	int m_messageTimer;

	//名前入力のハンドル(入力していないときは-1)
	int m_nameInputHandle;

	//キーが押され続けているフレーム数
	std::array<int, 256>m_keyHoldFrame;
	//前のフレームにマウスが押されていたか
	bool m_isPrevMouseDown;
	//タイムラインをドラッグしているかどうか
	bool m_isScrubbing;
};
