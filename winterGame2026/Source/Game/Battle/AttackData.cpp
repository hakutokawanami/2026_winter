#include "AttackData.h"
#include<DxLib.h>
#include<fstream>
#include<sstream>
#include<vector>
#include<filesystem>
#include<cassert>

namespace
{
	//技の情報のファイル
	constexpr const char* kAttackDataPath = "Data/Attack/AttackData.csv";
	//1行に必要な項目数
	constexpr size_t kColumnCount = 14;
	//ファイルの先頭に書く説明
	constexpr const char* kHeader =
		"# name,anim,animSpeed,blendTime,bone,length,radius,offsetY,startUp,active,recovery,damage,hitStun,blockStun";

	/// <summary>
	/// UTF-8の文字列をワイド文字列に変換する
	/// </summary>
	std::wstring ToWide(const std::string& str)
	{
		if (str.empty())
		{
			return {};
		}
		int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
		std::wstring result(size, L'\0');
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), result.data(), size);
		return result;
	}

	/// <summary>
	/// ワイド文字列をUTF-8の文字列に変換する
	/// </summary>
	std::string ToUtf8(const std::wstring& str)
	{
		if (str.empty())
		{
			return {};
		}
		int size = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0, nullptr, nullptr);
		std::string result(size, '\0');
		WideCharToMultiByte(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), result.data(), size, nullptr, nullptr);
		return result;
	}

	/// <summary>
	/// 1行をカンマで区切る
	/// </summary>
	std::vector<std::string> Split(const std::string& line)
	{
		std::vector<std::string> result;
		std::stringstream stream(line);
		std::string item;
		while (std::getline(stream, item, ','))
		{
			result.push_back(item);
		}
		return result;
	}
}

AttackTable& AttackTable::GetInstance()
{
	static AttackTable instance;
	return instance;
}

bool AttackTable::Load()
{
	std::ifstream file(kAttackDataPath);
	if (!file)
	{
		return false;
	}

	m_attacks.clear();

	std::string line;
	while (std::getline(file, line))
	{
		//Windowsの改行の\rを消す
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}
		//空行と#から始まる行は飛ばす
		if (line.empty() || line[0] == '#')
		{
			continue;
		}

		auto items = Split(line);
		if (items.size() < kColumnCount)
		{
			assert(false && "AttackData.csvの項目が足りない行があります");
			continue;
		}

		AttackDef def;
		def.name = items[0];
		def.animName = ToWide(items[1]);
		def.animSpeed = std::stof(items[2]);
		def.blendTime = std::stof(items[3]);
		def.bone = ToWide(items[4]);
		def.length = std::stof(items[5]);
		def.radius = std::stof(items[6]);
		def.offsetY = std::stof(items[7]);
		def.data.startUp = std::stoi(items[8]);
		def.data.active = std::stoi(items[9]);
		def.data.recovery = std::stoi(items[10]);
		def.data.damage = std::stoi(items[11]);
		def.data.hitStun = std::stoi(items[12]);
		def.data.blockStun = std::stoi(items[13]);
		m_attacks.push_back(def);
	}
	return true;
}

bool AttackTable::Save() const
{
	//フォルダが無ければ作る
	std::filesystem::create_directories(std::filesystem::path(kAttackDataPath).parent_path());

	std::ofstream file(kAttackDataPath);
	if (!file)
	{
		return false;
	}

	file << kHeader << "\n";
	for (const auto& def : m_attacks)
	{
		char line[512];
		snprintf(line, sizeof(line), "%s,%s,%.2f,%.0f,%s,%.1f,%.1f,%.1f,%d,%d,%d,%d,%d,%d",
			def.name.c_str(),
			ToUtf8(def.animName).c_str(),
			def.animSpeed,
			def.blendTime,
			ToUtf8(def.bone).c_str(),
			def.length,
			def.radius,
			def.offsetY,
			def.data.startUp,
			def.data.active,
			def.data.recovery,
			def.data.damage,
			def.data.hitStun,
			def.data.blockStun);
		file << line << "\n";
	}
	return true;
}

const AttackDef& AttackTable::Get(const std::string& name) const
{
	for (const auto& def : m_attacks)
	{
		if (def.name == name)
		{
			return def;
		}
	}
	assert(false && "技が見つかりません。AttackData.csvを確認してください");
	//見つからなかったときは何もしない技を返す
	static const AttackDef kEmpty = {};
	return kEmpty;
}

bool AttackTable::Contains(const std::string& name) const
{
	for (const auto& def : m_attacks)
	{
		if (def.name == name)
		{
			return true;
		}
	}
	return false;
}
