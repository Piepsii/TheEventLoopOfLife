#include "Subject.h"

void Subject::addObserver(Observer* _observer)
{
	observers.push_back(_observer);
}

void Subject::removeObserver(Observer* _observer)
{
	for (auto current = observers.begin(); current != observers.end(); ++current) {
		if (*current = _observer)
			observers.erase(current);
	}
}

void Subject::removeAll()
{
	observers.clear();
}

void Subject::notify(const Agent* _agent, Event _event)
{
	for (auto observer = observers.begin(); observer != observers.end(); ++observer) {
		(*observer)->onNotify(*_agent, _event);
	}
}
