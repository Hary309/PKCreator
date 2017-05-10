/*
*********************************************************************
* File          : SceneEditor.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QSharedPointer>

namespace sf
{
	class RenderWindow;
	class RectangleShape;
	class Sprite;
}

class TextureMgr;
class ObjectItem;
class SceneItemWindow;
class SceneObject; // dec in SceneItem.h
class SceneItem;

class SceneEditor : public QWidget
{
	Q_OBJECT

private:
	QSharedPointer<sf::RenderWindow>		m_pRenderer;

	QVector<QSharedPointer<SceneObject>>	*m_pObjects;
	SceneItem								*m_pSceneItem;

	QSharedPointer<sf::Sprite>				*m_pBackground;

	const ObjectItem						*m_pSelectedObj;

	SceneObject								*m_pCurrObj;

	QSharedPointer<TextureMgr>				m_pTexMgr;

	bool									m_drawGrid;
	int										m_snapX;
	int										m_snapY;

	QSize									m_windowSize;

	QSharedPointer<sf::RectangleShape>		m_hLine; // horizontal
	QSharedPointer<sf::RectangleShape>		m_vLine; // vertical

	QTimer									m_timer;

	friend									SceneItemWindow;

protected:
	void mouseMoveEvent(QMouseEvent *e)		override;
	void mousePressEvent(QMouseEvent *e)	override;
	void mouseReleaseEvent(QMouseEvent *e)	override;
	void resizeEvent(QResizeEvent *e)		override;

public:
	explicit SceneEditor(QWidget *parent);
	~SceneEditor();

	void SetSource(SceneItem *sceneItem);

	void SetCurrObject(const ObjectItem *obj) { m_pSelectedObj = obj; }

	TextureMgr *GetTexMgr() const { return m_pTexMgr.data(); }

public slots:
	void Render();
};

