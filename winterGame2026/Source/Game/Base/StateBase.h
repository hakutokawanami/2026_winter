#pragma once
class StateBase
{
public:
	virtual ~StateBase() = default;

	virtual void Enter()abstract;
	virtual void Update()abstract;
	virtual void Exit()abstract;
};

