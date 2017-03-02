#pragma once

#include <QDialog>

class SceneEditor : public QDialog
{
protected:
	void moveEvent(QMoveEvent *e) override;
	void closeEvent(QCloseEvent *e) override;

public:
	explicit SceneEditor(QWidget *parent);
	~SceneEditor();
};

