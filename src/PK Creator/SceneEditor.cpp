#include "SceneEditor.h"

#include <QApplication>
#include <QDesktopWidget>

#include <QMoveEvent>

#include <Scene.h>

SceneEditor::SceneEditor(QWidget *parent)
	: QDialog(parent)
{
	const int width = 800,
		height = 600;

	resize(width, height);
	setMinimumSize(QSize(width, height));
	setMaximumSize(QSize(16777215, 16777215));

	move(QApplication::desktop()->width() / 2 - width / 2, QApplication::desktop()->height() / 2 - height / 2);

	// (HWND)QWidget::winId();
}

SceneEditor::~SceneEditor()
{
}

void SceneEditor::moveEvent(QMoveEvent *e)
{
	QDialog::moveEvent(e);

	if (parent())
	{
		Scene *scene = (Scene*)parent();

		QRect rect = scene->geometry();

		const int offsetX = 16;

		scene->move(pos().x() - rect.width() - offsetX, pos().y());
	}
}

void SceneEditor::closeEvent(QCloseEvent *e)
{
	if (parent())
	{
		Scene *scene = (Scene*)parent();
		scene->hide();
	}

}
