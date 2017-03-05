#pragma once

#include <SFML/Graphics.hpp>

#include <QDialog>
#include <QTimer>

namespace sf
{
	class RenderWindow;
	class Sprite;
}

class TextureMgr;

class SceneEditor : public QDialog
{
private:
	sf::RenderWindow	*m_pWindow = nullptr;

	sf::Sprite			*m_pCurrObject;
	
	TextureMgr			*m_pTexMgr;

	QTimer				m_timer;


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

	void SetCurrObject(const QString &sprName) const;

	TextureMgr *GetTexMgr() const { return m_pTexMgr; }
};

