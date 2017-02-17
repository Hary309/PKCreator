#pragma once

#include <QDialog>

class SceneEditor : public QDialog
{
protected:
	virtual void moveEvent(QMoveEvent *event) override;
	virtual void closeEvent(QCloseEvent *event) override;

public:
	SceneEditor(QWidget *parent);
	~SceneEditor();
};

