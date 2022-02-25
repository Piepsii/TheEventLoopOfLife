#pragma once

#include <list>

#include "Observer.h"
#include "Event.h"
#include "Agent.h"

class Subject {
public:
	void addObserver(Observer* _observer);
	void removeObserver(Observer* _observer);
	void removeAll();

protected:
	virtual void notify(const Agent* _agent, Event _event);

private:
	std::list<Observer*> observers = std::list<Observer*>();

};
