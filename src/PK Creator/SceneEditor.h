#pragma once

#include <QWidget>
#include <QTimer>
#include <QList>

namespace sf
{
	class RenderWindow;
	class RectangleShape;
}

class TextureMgr;
class ObjectItem;
class SceneItemWindow;
class SceneObject;

class SceneEditor : public QWidget
{
	Q_OBJECT

private:
	sf::RenderWindow		*m_pRenderer;

	QList<SceneObject*>		*m_pObjects;

	const ObjectItem		*m_pSelectedObj;

	SceneObject				*m_pCurrObj;

	TextureMgr				*m_pTexMgr;

	bool					m_drawGrid;
	int						m_snapX;
	int						m_snapY;

	QSize					m_windowSize;

	unsigned				m_bgColor;

	sf::RectangleShape		*m_hLine; // horizontal
	sf::RectangleShape		*m_vLine; // vertical

	QTimer					m_timer;

	friend					SceneItemWindow;

protected:
	void mouseMoveEvent(QMouseEvent *e)		override;
	void mousePressEvent(QMouseEvent *e)	override;
	void mouseReleaseEvent(QMouseEvent *e)	override;
	void resizeEvent(QResizeEvent *e)		override;

public:
	explicit SceneEditor(QWidget *parent);
	~SceneEditor();

	void SetObjectList(QList<void*> *list);

	void SetCurrObject(const ObjectItem *obj) { m_pSelectedObj = obj; }

	TextureMgr *GetTexMgr() const { return m_pTexMgr; }

public slots:
	void Render();
};

