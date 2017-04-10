#pragma once

#include <VisualWidget.h>

class VisualNode;
class Widget;

class InputWidget : public VisualWidget
{


public:
	InputWidget(VisualNode *parent, Widget *data, sf::Vector2f offset);
	~InputWidget();

	void Render(sf::RenderWindow *renderer) override;
	void Event(sf::Event *e) override;

	void MoveTo(sf::Vector2f pos) override;
};

