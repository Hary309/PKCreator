/*
*********************************************************************
* File          : WireData.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Wire.h>
#include <VisualWidget.h>

class WireData : public Wire
{
public:
	VisualWidget *m_pStartWidget;
	VisualWidget *m_pEndWidget;

public:
	WireData() : Wire()
	{
		m_wireType = Wire::DATA;

		m_pStartWidget = m_pEndWidget = nullptr;
	}

	virtual ~WireData() { }

	void Connected() override
	{
		if (m_pStartWidget && m_pEndWidget)
		{
			m_pStartWidget->ConnectedWith(m_pEndWidget);
			m_pEndWidget->ConnectedWith(m_pStartWidget);
		}
	}

	void DisconnectAll() override
	{
		if (m_pStartWidget)
			m_pStartWidget->Disconnect(this);

		if (m_pEndWidget)
			m_pEndWidget->Disconnect(this);
	}
};

