/*
*********************************************************************
* File          : EventDefsMgr.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/


#include "EventDefsMgr.h"

EventDefsMgr *EventDefsMgr::s_pInst;

EventDefsMgr::EventDefsMgr()
{
	s_pInst = this;

	AddEvent(CREATE_EVENT, "Create Event", "createEvent", QVector<Arg>());
	AddEvent(DESTROY_EVENT, "Destroy Event", "destroyEvent", QVector<Arg>());
	AddEvent(PULSE_EVENT, "Pulse Event", "pulseEvent", QVector<Arg>());
	AddEvent(RENDER_EVENT, "Render Event", "renderEvent", QVector<Arg>());

	QVector<Arg> mouse;
	mouse.push_back(Arg(DataType::DATA_NUMBER, "x"));
	mouse.push_back(Arg(DataType::DATA_NUMBER, "y"));

	AddEvent(MOUSE_MOVED_EVENT, "Mouse Moved Event", "mouseMovedEvent", mouse);
	AddEvent(MOUSE_PRESSED_EVENT, "Mouse Pressed Event", "mouseDownEvent", mouse);
	AddEvent(MOUSE_RELEASED_EVENT, "Mouse Released Event", "mouseUpEvent", mouse);

	QVector<Arg> keyboard;
	keyboard.push_back(Arg(DataType::DATA_INTEGER, "key"));

	AddEvent(KEY_PRESSED_EVENT, "Key Pressed Event", "keyDownEvent", keyboard);
	AddEvent(KEY_RELEASED_EVENT, "Key Released Event", "keyUpEvent", keyboard);
}

EventDefsMgr::~EventDefsMgr()
{

}

EventDefsMgr::Event *EventDefsMgr::GetEvent(Type type)
{
	for (auto e : m_eventsDef)
	{
		if (e->type == type)
			return e.data();
	}

	return nullptr;
}

void EventDefsMgr::AddEvent(Type type, const QString &name, const QString &functionName, QVector<Arg> args)
{
	auto e = QSharedPointer<Event>(new Event());
	e->type = type;
	e->name = name;
	e->functionName = functionName;
	e->args = args;

	m_eventsDef.push_back(e);
}
