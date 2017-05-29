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

namespace sf
{
	class Sprite;
}

class SceneEditor;
class SceneItemWindow;

class ObjectItem;

class BackgroundItem;
class ResourceView;

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
	friend ResourceView;

protected:
	void RemoveSceneObject(const ObjectItem *object);

public:
	SceneItem(QStandardItem *item, const QString &itemName);
	~SceneItem() = default;

	void SetName(const QString &name) override;

	void Load(QDataStream *const dataStream) override;
	void Save(QDataStream *const dataStream) override;

	unsigned GetBgColor() const { return m_bgColor; }
	auto GetObjects() const { return &m_objects; }

	void Show(QWidget *wndParent) override;
	void Close() override;
};
