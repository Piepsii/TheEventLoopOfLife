#pragma once

#include <list>

#include "Observer.h"
#include "Event.h"
#include "Agent.h"

// Trample

class Subject {
public:
	void addObserver(Observer* _observer);
	void removeObserver(Observer* _observer);

protected:
	virtual void notify(const Agent* _agent, Event _event);

private:
	std::list<Observer*> observers = std::list<Observer*>();

};
