/*
*********************************************************************
* File          : HTML5Generator.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "HTML5Generator.h"

#include <QFile>
#include <QDir>
#include <QTextStream>

#include <ResourceView.h>
#include <ObjectItem.h>
#include <SpriteItem.h>
#include <SceneItem.h>
#include <SFML/Graphics/Color.hpp>

HTML5Generator::HTML5Generator(const QString &path)
	: CodeGenerator()
{
	m_path = path + "\\HTML5";

	QDir dir(m_path);
	dir.mkpath(m_path);

	m_globalVars += "var canvas = document.getElementById(\"canvas\");\n";
	m_globalVars += "var ctx = canvas.getContext(\"2d\");\n\n";;
}


HTML5Generator::~HTML5Generator()
{
}

void HTML5Generator::GenerateCanvas(const QString & title, const QSize &windowSize)
{
	// head
	m_htmlCode = "<!DOCTYPE html>\r<html>\r<head>\r<title>" + title + "</title>\r</head>\r<body>\r";

	// body
	m_htmlCode += "<canvas id =\"canvas\" width=\"" + QString::number(windowSize.width()) + "\" height=\"" + QString::number(windowSize.height()) + "\" style=\"border:1px solid black;\"></canvas>\r";

	// scripts
	m_htmlCode += "<script src=\"lib.js\"></script>\r<script src=\"script.js\"></script>\r</body>\r</html>";
}

void HTML5Generator::GenerateObject(ObjectItem *obj)
{
	if (!obj)
		return;

	if (!obj->GetSprite())
		return;

	QString objName = "gameObj" + obj->GetName();

	QString spriteName = objName + "." + obj->GetSprite()->GetTexPath().split(".").last();

	QFile string(ResourceView::Get()->GetMainDir() + obj->GetSprite()->GetTexPath());
	string.copy(m_path + "\\" + spriteName);

	m_globalVars += "var " + objName + ";\n";
	m_init += objName + " = new Object(\"" + spriteName + "\");\n";
	m_init += objName + ".origin = new Vector(" + QString::number(obj->GetSprite()->GetCenter().x()) + "," + QString::number(obj->GetSprite()->GetCenter().y()) + ");\n";
}

void HTML5Generator::GenerateScene(const QString &name, unsigned bgColor, QVector<SceneObject*> *bbjects)
{
	if (!bbjects)
		return;
	
	QString sceneName = "gameScene" + name;

	sf::Color color = sf::Color(bgColor);

	m_globalVars += "var " + sceneName + " = new Scene(\"rgb(" + QString::number(color.r) + "," + QString::number(color.g) + "," +QString::number(color.b) + ")\");\n";

	for (auto sceneObject : *bbjects)
	{
		QString objName = "gameObj" + sceneObject->pObj->GetName();

		QString vector = "new Vector(" + QString::number(sceneObject->pos.x()) + "," + QString::number(sceneObject->pos.y()) + ")";

		m_init += sceneName + ".objects.push(new SceneObject(" + objName + ", " + vector + "));\n";
	}

	// only for debug
	m_init += "currentScene = " + sceneName + ";\n";
}

void HTML5Generator::Save()
{
	QFile htmlFile(m_path + "\\index.html");
	htmlFile.open(QFile::WriteOnly);

	QTextStream stream(&htmlFile);
	stream << m_htmlCode;
	htmlFile.close();

	QFile jsFile(m_path + "\\script.js");
	jsFile.open(QFile::WriteOnly);

	stream.setDevice(&jsFile);

	// global var
	stream << m_globalVars << "var currentScene;";

	// init
	stream << "\nfunction init()\n{\n" << m_init << "}";

	// render
	stream << 
		"\n\nfunction render()\n"
		"{\n"
		"ctx.fillStyle=currentScene.bgColor;\n"
		"ctx.fillRect(0, 0, 800, 600);\n"
		"for (i = 0; i < currentScene.objects.length; i++)\n"
			"currentScene.objects[i].draw();\n"
		"setTimeout(render, 10);\n"
		"}";

	stream << "\ninit();\nrender();\n";


	// copy lib
	QFile lib("libs\\js\\lib.js");
	lib.copy(m_path + "\\lib.js");
}
