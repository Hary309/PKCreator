/*
*********************************************************************
* File          : SceneItem.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Item.h>

#include <QPoint>

class SceneEditor;
class SceneItemWindow;

class ObjectItem;

class BackgroundItem;

namespace sf
{
	class Sprite;
}

struct SceneObject
{
	const ObjectItem			*pObj;
	QSharedPointer<sf::Sprite>	pSpr;
	QPoint						pos;
};

class CodeGenerator;
class HTML5Generator;

class SceneItem : public Item
{
protected:
	QVector<QSharedPointer<SceneObject>>	m_objects;

	BackgroundItem							*m_pBackground;
	bool									m_tileVer;
	bool									m_tileHor;

	unsigned								m_bgColor;

	friend SceneItemWindow;
	friend SceneEditor;
	friend HTML5Generator;

public:
	SceneItem(QStandardItem *item, const QString &itemName);
	~SceneItem();

	void SetName(const QString &name) override;

	void Load(QDataStream *const dataStream) override;
	void Save(QDataStream *const dataStream) override;

	unsigned GetBgColor() const { return m_bgColor; }
	auto GetObjects() const { return &m_objects; }

	void Show(QWidget *wndParent) override;
	void Close() override;
};
