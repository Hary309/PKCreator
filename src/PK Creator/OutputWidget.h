#pragma once

#include <VisualWidget.h>

class VisualNode;
class Widget;

class OutputWidget : public VisualWidget
{
private:

public:
	OutputWidget(VisualNode *parent, Widget *data, sf::Vector2f offset);
	~OutputWidget();

	void Render(sf::RenderWindow *renderer) override;
	void Event(sf::Event *e) override;

	void MoveTo(sf::Vector2f pos) override;
};

