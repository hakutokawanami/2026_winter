#pragma once
#include "IInputSource.h"
#include"Game/Common/PlayerID.h"

class PadInputSource : public IInputSource
{
public:
	explicit PadInputSource(PlayerID id);

	bool IsTriggerd(Button button)const override;

	StickDirection GetStickDirection()const override;

	bool IsStickDoubleTap(StickDirection dir)const override;

private:
	PlayerID m_id;
};

