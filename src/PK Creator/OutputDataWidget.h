/*
*********************************************************************
* File          : OutputDataWidget.h
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

class OutputDataWidget : public VisualWidget
{
private:
	QVector<WireData*>		m_wires;

public:
	OutputDataWidget(VisualNode *parent, Widget *data, sf::Vector2f offset);
	~OutputDataWidget() = default;

	void ConnectedWith(VisualWidget *widget) override;
	Wire *ConnectWire() override;
	void Disconnect(Wire *wire) override;

	void Render(sf::RenderWindow *renderer) override;
	void Event(sf::Event *e) override;

	void MoveTo(sf::Vector2f pos) override;

	QVector<WireData*> *GetWires() { return &m_wires; }
};

