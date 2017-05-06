/*
*********************************************************************
* File          : EventDefsMgr.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once
#include <Common.h>
#include <QVector>
#include <QSharedPointer>

class EventDefsMgr
{
public:
	enum Type
	{
		CREATE_EVENT = 0,
		DESTROY_EVENT,
		PULSE_EVENT,
		RENDER_EVENT,
		MOUSE_MOVED_EVENT,
		MOUSE_PRESSED_EVENT,
		MOUSE_RELEASED_EVENT,
		KEY_PRESSED_EVENT,
		KEY_RELEASED_EVENT
	};

	class Arg
	{
	public:
		DataType type;
		QString name;

		Arg() { type = static_cast<DataType>(-1); name = ""; }
		Arg(DataType t, const QString &n) : type(t), name(n) {  }
	};

	class Event
	{
	public:
		Type type;
		QString name;
		QString functionName;
		QVector<Arg> args;
	};

private:
	QVector<QSharedPointer<Event>> m_eventsDef;

	static EventDefsMgr *s_pInst;

private:
	void AddEvent(Type type, const QString &name, const QString &functionName, QVector<Arg> args);

public:
	EventDefsMgr();
	~EventDefsMgr();

	Event *GetEvent(Type type);
	QVector<QSharedPointer<Event>> *GetEvents() { return &m_eventsDef; }

	static EventDefsMgr *Get() { return s_pInst; }
};

