#pragma once
#include<map>

/// <summary>
/// モデルの種類
/// </summary>
enum class ModelType
{
	player1,
	player2,
};
/// <summary>
/// グラフィックの種類
/// </summary>
enum class GraphType
{

};

enum class EffectType
{

};

class ResourceManager
{
private:
	//newも変数宣言もできなくする
	ResourceManager();
	//コピーコンストラクタも無効にする
	ResourceManager(const ResourceManager& app) = delete;
	//代入も無効にする
	void operator = (const ResourceManager& app) = delete;

public:
	~ResourceManager();

	static ResourceManager& GetInstance();
	//リソースのロード
	void LoadResources();
	//リソースの開放
	void ReleaseResources();
	//指定したモデルハンドルを取得する
	int GetModel(ModelType type)const;
	//指定した画像のハンドルを取得する
	int GetGraph(GraphType type)const;
	//指定したエフェクトのパスを取得する
	const wchar_t* GetEffectPath(EffectType type)const;

private:
	//モデルの種類と実際のハンドル
	std::map<ModelType, int>m_modelHandleTable;
	//画像の種類と実際のハンドル
	std::map<GraphType, int>m_graphHandleTable;
	//エフェクトの種類と実際のハンドル
	std::map<EffectType, const wchar_t*>m_effectHandleTable;

	//プレイヤーモデル
	int m_player1ModelHandle;
	int m_player2ModelHandle;
};

