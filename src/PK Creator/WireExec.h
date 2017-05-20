/*
*********************************************************************
* File          : WireExec.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Wire.h>
#include <VisualNode.h>

class WireExec : public Wire
{
public:
	VisualNode *m_pStartNode;
	VisualNode *m_pEndNode;

public:
	WireExec() : Wire()
	{
		m_wireType = Wire::EXEC;

		m_pStartNode = m_pEndNode = nullptr;
	}

	virtual ~WireExec() = default;

	void Connected() override
	{
		if (m_pStartNode && m_pEndNode)
		{
			m_pStartNode->ConnectedWith(m_pEndNode, VisualNode::ExecTo);
			m_pEndNode->ConnectedWith(m_pStartNode, VisualNode::ExecFrom);
		}
	}

	void DisconnectAll() override
	{
		if (m_pStartNode)
			m_pStartNode->Disconnect(this);

		if (m_pEndNode)
			m_pEndNode->Disconnect(this);
	}
};

