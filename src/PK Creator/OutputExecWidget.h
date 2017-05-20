/*
*********************************************************************
* File          : OutputExecWidget.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once
#include <VisualWidget.h>

class WireExec;

class OutputExecWidget : public VisualWidget
{
private:
	WireExec *m_pWire;

	bool m_waitingForWire;

public:
	OutputExecWidget(VisualNode *parent, Widget *data, sf::Vector2f offset);
	~OutputExecWidget() = default;

	bool ConnectedWith(VisualNode *node);
	void ConnectedWith(VisualWidget *widget) override { }
	Wire *ConnectWire() override;
	void Disconnect(Wire *wire) override;

	void Render(sf::RenderWindow *renderer) override;
	void Event(sf::Event *e) override;

	void MoveTo(sf::Vector2f pos) override;

	WireExec *GetWire() const { return m_pWire; }
};

