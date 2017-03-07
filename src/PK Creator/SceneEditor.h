#pragma once

#include <SFML/Graphics.hpp>

#include <QDialog>
#include <QTimer>
#include <QList>

namespace sf
{
	class RenderWindow;
	class Sprite;
}

class Scene;
class Object;

class TextureMgr;

class SceneEditor : public QDialog
{
private:
	struct SceneObject
	{
		const Object		*pObj;
		sf::Sprite			*pSpr;
	};

protected:
	sf::RenderWindow		*m_pWindow = nullptr;

	QList <SceneObject*>	m_objects;

	const Object			*m_pSelectedObj;
	
	SceneObject				*m_pCurrObj;

	TextureMgr				*m_pTexMgr;

	QSize					m_windowSize;

	unsigned				m_bgColor;

	bool					m_drawGrid;
	int						m_snapX;
	int						m_snapY;

	sf::RectangleShape		*m_hLine; // horizontal
	sf::RectangleShape		*m_vLine; // vertical
	
	QTimer					m_timer;

	friend Scene;

protected:
	void mouseMoveEvent(QMouseEvent *e)		override;
	void mousePressEvent(QMouseEvent *e)	override;
	void mouseReleaseEvent(QMouseEvent *e)	override;
	void moveEvent(QMoveEvent *e)			override;
	void showEvent(QShowEvent *e)			override;
	void closeEvent(QCloseEvent *e)			override;
	void paintEvent(QPaintEvent *e)			override;

private:
	void Render();
	void Pulse();

public:
	explicit SceneEditor(QWidget *parent);
	~SceneEditor();

	void SetCurrObject(const Object *obj);

	TextureMgr *GetTexMgr() const { return m_pTexMgr; }
};

