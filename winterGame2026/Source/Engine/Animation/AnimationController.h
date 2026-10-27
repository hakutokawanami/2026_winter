#pragma once
#include<string>
#include<map>

class AnimationController
{
public:
	//コンストラクタでモデルハンドルを受け取る
	AnimationController(int modelHandle);
	//更新
	void Update();
	/// <summary>
	/// アニメーション再生
	/// </summary>
	/// <param name="name">アニメーション名</param>
	/// <param name="loop">ループフラグ(デフォルトはtrue)</param>
	/// <param name="blendTime">blendにかかるフレーム数(デフォルトは15.0f)</param>
	void Play(const std::wstring& name, bool loop = true, float blendTime = 15.0f,float speed = 0.5f);
	/// <summary>
	/// アニメーション登録
	/// </summary>
	/// <param name="name">アニメーション名</param>
	void AddAnimation(const std::wstring& name);
	/// <summary>
	/// ループしないアニメーションが終わったかどうか
	/// </summary>
	/// <returns></returns>
	bool IsEnd() { return m_isEnd; }
	/// <summary>
	/// 再生時間を直接指定する(攻撃エディタのコマ送り用)
	/// </summary>
	/// <param name="time">アニメーション時間</param>
	void SetTime(float time);
	/// <summary>
	/// 再生中のアニメーション全体の時間を取得する
	/// </summary>
	/// <returns></returns>
	float GetTotalTime()const;
private:
	//モデルハンドル
	int m_modelHandle;

	//アニメーション名とアニメーション番号
	std::map<std::wstring, int>m_animTable;
	//現在のアニメーション番号
	int m_currentAnimNo;
	//現在のアニメーションのアタッチ番号
	int m_currentAttachNo;
	//アニメーションの再生時間
	float m_currentTime;
	//ループするかどうか
	bool m_isLoop;
	//アニメーションが終わったかどうか
	bool m_isEnd;

	//前のアニメーション番号
	int m_prevAnimNo;
	//前のアニメーションのアタッチ番号
	int m_prevAttachNo;
	//前のアニメーションの再生時間
	float m_prevTime;

	//ブレンドにかける時間
	float m_blendTime;
	//経過時間
	float m_blendTimer;

	//再生速度
	float m_speed;
	//前のアニメーションの再生速度
	float m_prevSpeed;
};
