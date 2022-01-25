#pragma once
class Agent
{
public:
	virtual void Sense() = 0;
	virtual void Decide() = 0;
	virtual void Act() = 0;
};

