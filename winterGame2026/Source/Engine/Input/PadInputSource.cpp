#include "PadInputSource.h"
#include"InputManager.h"

PadInputSource::PadInputSource(PlayerID id):
	m_id(id)
{
}

bool PadInputSource::IsTriggerd(Button button) const
{
	return InputManager::GetInstance().IsTriggered(m_id,ToName(button));
}

StickDirection PadInputSource::GetStickDirection() const
{
	return InputManager::GetInstance().GetStickDirection(m_id);
}

bool PadInputSource::IsStickDoubleTap(StickDirection dir) const
{
	return InputManager::GetInstance().IsStickDoubleTapInput(m_id,dir);
}
