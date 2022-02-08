#pragma once
#include "Agent.h"
#include "Event.h"

// sheep, grass, wolf etc

class Observer
{
public:
	virtual ~Observer() {};
	virtual void onNotify(const Agent& _agent, Event _event);
};

