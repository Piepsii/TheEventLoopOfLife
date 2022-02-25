#pragma once

class Agent;
#include "Event.h"

class Observer
{
public:
	virtual ~Observer() {};
	virtual void onNotify(const Agent& _agent, Event _event);
};

