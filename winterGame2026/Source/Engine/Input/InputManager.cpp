#include "InputManager.h"

namespace
{
	//GetJoypadXInputStateで取得した値を-1.0～1.0の範囲に変換するために使う
	constexpr float kStickInitializeNum = 32767.0f;
	//GetJoypadDirectInputStateで取得した値を-1.0～1.0の範囲に変換するために使う
	constexpr float kDirectInputStickMax = 1000.0f;
	//スティックのデッドゾーン
	constexpr float kDeadZone = 0.4f;
	//入力を保存するフレーム数
	constexpr int kStickHistorySize = 30;
	//入力履歴にデバッグ表示するボタン(この順番で並べて表示する)
	constexpr const char* kHistoryButtonNames[] = { "A","B","X","Y","START","SELECT" };
}

InputManager::InputManager() :
	m_inputData{},
	m_lastInputData{},
	m_inputTable{},
	m_stickDirection{},
	m_leftStick{},
	m_stickAnalyzer{},
	m_rawLeftStick{},
	m_rawRightStick{}
{
	//イベント名を添え字にして、
	// 右辺に実際の入力種別と入力コードの配列を置く

	m_inputTable["left"] = { {PeripheralType::keyBoard,KEY_INPUT_LEFT},
							{PeripheralType::pad,PAD_INPUT_LEFT} };
	m_inputTable["up"] = { {PeripheralType::keyBoard,KEY_INPUT_UP},
							{PeripheralType::pad,PAD_INPUT_UP} };
	m_inputTable["right"] = { {PeripheralType::keyBoard,KEY_INPUT_RIGHT},
							{PeripheralType::pad,PAD_INPUT_RIGHT} };
	m_inputTable["down"] = { {PeripheralType::keyBoard,KEY_INPUT_DOWN},
							{PeripheralType::pad,PAD_INPUT_DOWN} };

	m_inputTable["A"] = { {PeripheralType::keyBoard,KEY_INPUT_Z},
							{PeripheralType::pad,PAD_INPUT_A} };
	m_inputTable["B"] = { {PeripheralType::keyBoard,KEY_INPUT_X},
							{PeripheralType::pad,PAD_INPUT_B} };
	m_inputTable["X"] = { {PeripheralType::keyBoard,KEY_INPUT_C},
							{PeripheralType::pad,PAD_INPUT_C} };
	m_inputTable["Y"] = { {PeripheralType::keyBoard,KEY_INPUT_V},
							{PeripheralType::pad,PAD_INPUT_X} };
	m_inputTable["START"] = { {PeripheralType::keyBoard,KEY_INPUT_RETURN},
							{PeripheralType::pad,PAD_INPUT_R} };
	m_inputTable["SELECT"] = { {PeripheralType::keyBoard,KEY_INPUT_Z},
							{PeripheralType::pad,PAD_INPUT_L} };

	//あらかじめ入力データの枠を開けておく
	//ここで枠を開けておかないと、
	//チェックの際にAt関数でクラッシュする可能性がある
	for (const auto& inputInfo : m_inputTable)
	{
		for (int i = 0; i < m_inputData.size();i++)
		{
			m_inputData[i][inputInfo.first] = false;
			m_lastInputData[i][inputInfo.first] = false;
		}
	}
}

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

void InputManager::Update()
{
	char keyState[256];
	GetHitKeyStateAll(keyState);//生のキーボード情報

	for (int i = 0; i < static_cast<int>(PlayerID::PlayerCount); i++)
	{
		PlayerID player = static_cast<PlayerID>(i);
		//何番目のゲームパッドを使うか取得
		int padNo = GetPadNo(player);
		//ボタン入力を取得
		int padState = GetJoypadInputState(padNo);
		//直前のフレームを更新(前のフレーム情報をコピー)
		m_lastInputData[i] = m_inputData[i];

		//スティックの入力を取得
		ReadStick(i, padNo);
		//左スティックの入力を変換
		ConvertLeftStick(i);
		//左スティックの入力を8方向+ニュートラルに変換
		ConvertStickDirection(i);

		//すべての入力イベントをチェック
		//ここでinputData_が更新される
		//inputTable_を回して各イベントの入力をチェック
		for (const auto inputInfo : m_inputTable)
		{
			//inputInfo.firstには"ok"等が入っている
			auto& input = m_inputData[i][inputInfo.first];

			input = false;//初期化

			//InputStateのベクタを回す
			for (const auto& state : inputInfo.second)
			{
				//入力種別をチェック
				switch (state.type)
				{
				case PeripheralType::keyBoard://キーボードの場合
					if (i == 0)
					{
						input = keyState[state.id];
					}
					break;
				case PeripheralType::pad:
					input = (padState & state.id);
					break;
				}
				//ここでbreakしないと最後のチェックで押されていないとfalseになる
				if (input)
				{
					break;
				}
			}
		}

		//押されているボタンを集める(履歴のデバッグ表示用)
		std::vector<const char*> pressedButtons;
		for (const char* name : kHistoryButtonNames)
		{
			if (m_inputData[i].at(name))
			{
				pressedButtons.push_back(name);
			}
		}
		//入力履歴に追加
		m_stickAnalyzer[i].Update(m_stickDirection[i], pressedButtons);
	}

#ifdef _DEBUG
	DrawFormatString(
		16, 180,
		0xffffff,
		L"P1 A : %d -> %d",
		m_lastInputData[0].at("A"),
		m_inputData[0].at("A"));

	DrawFormatString(
		16, 196,
		0xffffff,
		L"P2 A : %d -> %d",
		m_lastInputData[1].at("A"),
		m_inputData[1].at("A"));

	DrawFormatString(
		20, 100,
		0xffffff,
		L"P1 PadState : %d",
		GetJoypadInputState(DX_INPUT_PAD1)
	);

	DrawFormatString(
		20, 120,
		0xffffff,
		L"P2 PadState : %d",
		GetJoypadInputState(DX_INPUT_PAD2)
	);

#endif
}

void InputManager::ReadStick(int id, int padNo)
{
	if (CheckJoypadXInput(padNo))
	{
		//Xboxコントローラーなど、XInput対応のパッド
		XINPUT_STATE state{};
		GetJoypadXInputState(padNo, &state);

		m_rawLeftStick[id] = { state.ThumbLX / kStickInitializeNum, state.ThumbLY / kStickInitializeNum };
		m_rawRightStick[id] = { state.ThumbRX / kStickInitializeNum, state.ThumbRY / kStickInitializeNum };
	}
	else
	{
		//DUALSHOCKなど、DirectInputのパッド
		DINPUT_JOYSTATE state{};
		GetJoypadDirectInputState(padNo, &state);

		//DirectInputは下方向が+なので、XInputに合わせてYを反転する
		m_rawLeftStick[id] = { state.X / kDirectInputStickMax, -state.Y / kDirectInputStickMax };
		//右スティックはZ軸とZ回転に割り当てられていることが多い
		m_rawRightStick[id] = { state.Z / kDirectInputStickMax, -state.Rz / kDirectInputStickMax };
	}
}

void InputManager::ConvertLeftStick(int id)
{
	Vector2 stick = m_rawLeftStick[id];

	if (stick.Length() < kDeadZone)
	{
		m_leftStick[id] = { 0.0f,0.0f };
	}
	else
	{
		m_leftStick[id] = stick;
	}
}

void InputManager::ConvertStickDirection(int id)
{
	float angle = 0.0f;
	int direction = 0;

	if (m_leftStick[id].Length() < kDeadZone)
	{
		//動かしていないならニュートラル
		m_stickDirection[id] = StickDirection::neutral;
	}
	else
	{
		//スティックの角度を求める
		angle = atan2f(m_leftStick[id].y, m_leftStick[id].x);
		//8方向の境界を22.5どずらす
		angle += DX_PI_F / 8.0f;
		//角度を0から2πにする
		if (angle < 0.0f)
		{
			angle += DX_PI_F * 2.0f;
		}
		//45度ごとに8方向に分類
		direction = static_cast<int>(angle / (DX_PI_F / 4.0f));

		switch (direction)
		{
		case 0:
			m_stickDirection[id] = StickDirection::right;
			break;
		case 1:
			m_stickDirection[id] = StickDirection::upRight;
			break;
		case 2:
			m_stickDirection[id] = StickDirection::up;
			break;
		case 3:
			m_stickDirection[id] = StickDirection::upLeft;
			break;
		case 4:
			m_stickDirection[id] = StickDirection::left;
			break;
		case 5:
			m_stickDirection[id] = StickDirection::downLeft;
			break;
		case 6:
			m_stickDirection[id] = StickDirection::down;
			break;
		case 7:
			m_stickDirection[id] = StickDirection::downRight;
			break;
		}
	}

#ifdef _DEBUG
	DrawFormatString(16, 300 + 20 * id, 0xffffff, L"stickAngle : %f", angle);
	DrawFormatString(16, 340 + 20 * id, 0xffffff, L"stickDirection : %d", direction);
#endif // _DEBUG

}

bool InputManager::IsPressed(PlayerID player,const char* name) const
{
	int index = static_cast<int>(player);

	return m_inputData[index].at(name);
}

bool InputManager::IsTriggered(PlayerID player,const char* name) const
{
	int index = static_cast<int>(player);

	return m_inputData[index].at(name) && !m_lastInputData[index].at(name);
}

bool InputManager::IsReleased(PlayerID player,const char* name) const
{
	int index = static_cast<int>(player);

	return !m_inputData[index].at(name) && m_lastInputData[index].at(name);
}

Vector2 InputManager::GetLeftStick(PlayerID player) const
{
	return m_leftStick[static_cast<int>(player)];
}

Vector2 InputManager::GetRightStick(PlayerID player) const
{
	int index = static_cast<int>(player);

	Vector2 stick = m_rawRightStick[index];
	float x = stick.x;
	float y = stick.y;

	//デッドゾーン処理
	float len = sqrtf(x * x + y * y);
	if (len < kDeadZone)
	{
		return { 0.0f,0.0f };
	}
	//正規化
	if (len > 1.0f)
	{
		len = 1.0f;
	}
	stick.x /= len;
	stick.y /= len;

	return stick;
}

bool InputManager::AnyPressButtonInput(PlayerID player) const
{
	int index = static_cast<int>(player);

	for (const auto& input : m_inputData[index])
	{
		if (input.first == "A" || input.first == "B" ||
			input.first == "X" || input.first == "Y" ||
			input.first == "START")
		{
			if (input.second)
			{
				return true;
			}
		}
	}
	return false;
}

int InputManager::GetPadNo(PlayerID player) const
{
	switch (player)
	{
	case PlayerID::P1:
		return DX_INPUT_PAD1;
		break;
		
	case PlayerID::P2:
		return DX_INPUT_PAD2;
		break;
	}

	return DX_INPUT_PAD1;
}

StickDirection InputManager::GetStickDirection(PlayerID player) const
{
	return m_stickDirection[static_cast<int>(player)];
}

bool InputManager::IsStickQuickInput(PlayerID player, StickDirection direction)
{
	return m_stickAnalyzer[static_cast<int>(player)].IsQuickInput(direction);
}

bool InputManager::IsStickDoubleTapInput(PlayerID player, StickDirection direction)
{
	return m_stickAnalyzer[static_cast<int>(player)].IsDoubleTap(direction);
}

void InputManager::DebugDrawStickHistory(PlayerID player, int x, int y) const
{
	m_stickAnalyzer[static_cast<int>(player)].DebugDraw(x, y);
}
