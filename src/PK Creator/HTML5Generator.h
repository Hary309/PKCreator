#pragma once

#include <CodeGenerator.h>

class HTML5Generator : public CodeGenerator
{
private:
	QString	m_htmlCode;

	// javascript
	QString m_globalVars;
	QString m_init;
	QString m_render;

public:
	explicit HTML5Generator(const QString &path);
	~HTML5Generator();

	void GenerateCanvas(const QString &title, const QSize &windowSize) override;

	void GenerateObject(ObjectItem *obj) override;
	void GenerateSceneObject(SceneObject *obj) override;

	void Save() override;
};

