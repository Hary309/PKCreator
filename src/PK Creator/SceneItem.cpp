#include "SceneItem.h"

#include <QDataStream>

#include <Item.h>
#include <SceneItemWindow.h>


SceneItem::SceneItem(QStandardItem *item, const QString &itemName)
	: Item(item, itemName)
{
	SceneItem::SetName(itemName);

	m_type = SCENE;

	m_pItemWnd = nullptr;
}

SceneItem::~SceneItem()
{
	if (m_pItemWnd)
	{
		delete m_pItemWnd;
		m_pItemWnd = nullptr;
	}
}

void SceneItem::SetName(const QString &name)
{
	m_itemName = name;
	m_pTreeItem->setText(name);
}

void SceneItem::Load(QDataStream *const dataStream)
{
	Item::Load(dataStream);
	/**dataStream >> m_pSceneEditor->m_bgColor;

	QColor color(m_pSceneEditor->m_bgColor >> 8);

	QString newColor = QString::asprintf("background-color: rgb(%d, %d, %d);", color.red(), color.green(), color.blue());
	
	ui.bgColorButton->setStyleSheet(newColor);*/
}

void SceneItem::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	//*dataStream << m_pSceneEditor->m_bgColor;
}

void SceneItem::Show(QWidget * wndParent)
{
	if (!m_pItemWnd)
	{
		m_pItemWnd = new SceneItemWindow(wndParent);
		m_pItemWnd->FillData(this);
		m_pItemWnd->show();
	}
}

void SceneItem::Close()
{
	if (m_pItemWnd)
	{
		delete m_pItemWnd;
		m_pItemWnd = nullptr;
	}
}
