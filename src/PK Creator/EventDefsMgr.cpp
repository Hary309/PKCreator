/*
*********************************************************************
* File          : EventDefsMgr.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "EventDefsMgr.h"

#include <QDebug>

EventDefsMgr *EventDefsMgr::s_pInst;

EventDefsMgr::EventDefsMgr()
{
	s_pInst = this;
	QVector<Arg> basic;
	basic.push_back(Arg(DataType::DATA_ID, "this"));
	
	AddEvent(CREATE_EVENT, "Create Event", "createEvent", basic);
	AddEvent(DESTROY_EVENT, "Destroy Event", "destroyEvent", basic);
	AddEvent(PULSE_EVENT, "Pulse Event", "pulseEvent", basic);
	AddEvent(RENDER_EVENT, "Render Event", "renderEvent", basic);


	QVector<Arg> mouse;
	mouse.push_back(Arg(DataType::DATA_ID, "this"));
	mouse.push_back(Arg(DataType::DATA_NUMBER, "x"));
	mouse.push_back(Arg(DataType::DATA_NUMBER, "y"));

	AddEvent(MOUSE_MOVED_EVENT, "Mouse Moved Event", "mouseMovedEvent", mouse);
	mouse.push_back(Arg(DataType::DATA_INTEGER, "button"));
	AddEvent(MOUSE_PRESSED_EVENT, "Mouse Pressed Event", "mouseDownEvent", mouse);
	AddEvent(MOUSE_RELEASED_EVENT, "Mouse Released Event", "mouseUpEvent", mouse);


	QVector<Arg> keyboard;
	keyboard.push_back(Arg(DataType::DATA_ID, "this"));
	keyboard.push_back(Arg(DataType::DATA_INTEGER, "key"));

	AddEvent(KEY_PRESSED_EVENT, "Key Pressed Event", "keyDownEvent", keyboard);
	AddEvent(KEY_RELEASED_EVENT, "Key Released Event", "keyUpEvent", keyboard);

	qInfo() << "Loaded events";
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
