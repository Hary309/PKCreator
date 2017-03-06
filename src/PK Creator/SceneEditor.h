#pragma once

#include <SFML/Graphics.hpp>

#include <QDialog>
#include <QTimer>

namespace sf
{
	class RenderWindow;
	class Sprite;
}

class Scene;

class TextureMgr;

class SceneEditor : public QDialog
{
protected:
	sf::RenderWindow	*m_pWindow = nullptr;

	sf::Sprite			*m_pCurrObject;
	
	TextureMgr			*m_pTexMgr;

	QSize				m_windowSize;

	unsigned			m_bgColor;

	bool				m_drawGrid;
	int					m_snapX;
	int					m_snapY;

	sf::RectangleShape	*m_hLine; // horizontal
	sf::RectangleShape	*m_vLine; // vertical
	
	QTimer				m_timer;

	friend Scene;

protected:
	void mouseMoveEvent(QMouseEvent *e) override;
	void moveEvent(QMoveEvent *e)		override;
	void showEvent(QShowEvent *e)		override;
	void closeEvent(QCloseEvent *e)		override;
	void paintEvent(QPaintEvent *e)		override;

private:
	void Render();
	void Pulse();

public:
	explicit SceneEditor(QWidget *parent);
	~SceneEditor();

	void SetCurrObject(const QString &sprName);

	TextureMgr *GetTexMgr() const { return m_pTexMgr; }
};

