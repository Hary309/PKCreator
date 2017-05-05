/*
*********************************************************************
* File          : OutputWidget.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QVector>

#include <VisualWidget.h>

class VisualNode;
class Widget;
class WireData;

class OutputWidget : public VisualWidget
{
private:
	QVector<WireData*>		m_wires;

public:
	OutputWidget(VisualNode *parent, Widget *data, sf::Vector2f offset);
	~OutputWidget();

	void ConnectedWith(VisualWidget *widget) override;
	WireData *ConnectWire() override;
	void Disconnect(Wire *wire) override;

	void Render(sf::RenderWindow *renderer) override;
	void Event(sf::Event *e) override;

	void MoveTo(sf::Vector2f pos) override;

	QVector<WireData*> *GetWires() { return &m_wires; }
};

