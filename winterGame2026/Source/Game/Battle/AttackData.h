#pragma once
#include"HitBox.h"
#include<string>
#include<deque>

/// <summary>
/// 技の性能
/// </summary>
struct AttackData
{
	//発生フレーム
	int startUp;
	//持続フレーム
	int active;
	//硬直フレーム
	int recovery;
	//ダメージ
	int damage;
	//ヒット時に相手が動けないフレーム
	int hitStun;
	//ガード時に相手が動けないフレーム
	int blockStun;
};

/// <summary>
/// 技一つの情報
/// </summary>
struct AttackDef
{
	//技の名前(コードから技を探すときに使う)
	std::string name;
	//アニメーション名
	std::wstring animName;
	//アニメーションの再生速度(1フレームに進めるアニメーション時間)
	float animSpeed = 1.0f;
	//アニメーションのブレンドにかけるフレーム数
	float blendTime = 3.0f;
	//攻撃判定を付けるボーンの名前
	std::wstring bone;
	//攻撃判定の長さ
	float length = 0.0f;
	//攻撃判定の半径
	float radius = 25.0f;
	//攻撃判定のボーンの位置からのオフセット
	float offsetY = 0.0f;
	//技性能
	AttackData data = {};

	/// <summary>
	/// 攻撃判定のカプセルの情報を取得する
	/// (boneは中の文字列を指しているので、技の情報より長く持たないこと)
	/// </summary>
	/// <returns></returns>
	BoneCapsuleDef GetBox()const { return { bone.c_str(), length, radius, offsetY }; }

	/// <summary>
	/// 技の全体フレームを取得する
	/// </summary>
	/// <returns></returns>
	int GetTotalFrame()const { return data.startUp + data.active - 1 + data.recovery; }
};

/// <summary>
/// すべての技の情報をファイルから読み込んで持っておくクラス
/// </summary>
class AttackTable
{
private:
	AttackTable() = default;
	AttackTable(const AttackTable&) = delete;
	void operator=(const AttackTable&) = delete;

public:
	static AttackTable& GetInstance();

	/// <summary>
	/// ファイルから技の情報を読み込む
	/// </summary>
	/// <returns>読み込めた:true / ファイルが無い:false</returns>
	bool Load();
	/// <summary>
	/// 技の情報をファイルに書き出す
	/// </summary>
	/// <returns>書き出せた:true / 失敗:false</returns>
	bool Save()const;

	/// <summary>
	/// 名前から技を探す
	/// </summary>
	/// <param name="name">技の名前</param>
	/// <returns></returns>
	const AttackDef& Get(const std::string& name)const;
	/// <summary>
	/// 名前の技があるかどうか
	/// </summary>
	/// <param name="name">技の名前</param>
	/// <returns></returns>
	bool Contains(const std::string& name)const;

	/// <summary>
	/// 技の数を取得する(エディタ用)
	/// </summary>
	/// <returns></returns>
	size_t GetCount()const { return m_attacks.size(); }
	/// <summary>
	/// 番号から技を取得する(エディタ用)
	/// </summary>
	/// <param name="index">番号</param>
	/// <returns></returns>
	AttackDef& GetAt(size_t index) { return m_attacks[index]; }
	/// <summary>
	/// 技を追加する(エディタ用)
	/// </summary>
	/// <param name="def">追加する技</param>
	void Add(const AttackDef& def) { m_attacks.push_back(def); }

private:
	//技の一覧
	//(dequeは後ろに追加しても他の要素の場所が変わらないので、参照を持っていても安全)
	std::deque<AttackDef>m_attacks;
};
