/*
*********************************************************************
* File          : InputWidget.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <VisualWidget.h>

class VisualNode;
class Widget;

class WireData;

class InputWidget : public VisualWidget
{
private:
	WireData		*m_pWire;

public:
	InputWidget(VisualNode *parent, Widget *data, sf::Vector2f offset);
	~InputWidget();

	void ConnectedWith(VisualWidget *widget) override;
	WireData *ConnectWire() override;
	void Disconnect(Wire *wire) override;

	void Render(sf::RenderWindow *renderer) override;
	void Event(sf::Event *e) override;

	void MoveTo(sf::Vector2f pos) override;

	WireData *GetWire() const { return m_pWire; }
};
