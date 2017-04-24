/*
*********************************************************************
* File          : CodeGenerator.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "CodeGenerator.h"

#include <QDir>
#include <QTextStream>

#include <ResourceView.h>
#include <ObjectItem.h>
#include <SpriteItem.h>
#include <SceneItem.h>

CodeGenerator::CodeGenerator(const QString &path)
	: m_path(path)
{
	QDir dir(m_path);
	dir.mkpath(m_path);

	m_globalVars += "var canvas = document.getElementById(\"canvas\");\n";
	m_globalVars += "var ctx = canvas.getContext(\"2d\");\n\n";;
}

CodeGenerator::~CodeGenerator()
{
}

void CodeGenerator::GenerateHTML(const QString &title, int width, int height)
{
	// head
	m_finalHTMLCode = "<!DOCTYPE html>\r<html>\r<head>\r<title>" + title + "</title>\r</head>\r<body>\r";

	// body
	m_finalHTMLCode += "<canvas id =\"canvas\" width=\"" + QString::number(width) + "\" height=\"" + QString::number(height) + "\" style=\"border:1px solid black;\"></canvas>\r";

	// scripts
	m_finalHTMLCode += "<script src=\"lib.js\"></script>\r<script src=\"script.js\"></script>\r</body>\r</html>";
}

void CodeGenerator::GenerateObject(ObjectItem *obj)
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

void CodeGenerator::GenerateSceneObject(SceneObject *obj)
{
	if (!obj)
		return;

	if (!obj->pObj)
		return;

	QString objName = "gameObj" + obj->pObj->GetName();

	QString vector = "new Vector(" + QString::number(obj->pos.x()) + "," + QString::number(obj->pos.y()) + ")";

	m_init += "sceneObjects.push(new SceneObject(" + objName + ", "+vector+"));\n";
}

void CodeGenerator::SaveHTML()
{
	QFile file(m_path + "\\index.html");
	file.open(QFile::WriteOnly);

	QTextStream stream(&file);
	stream << m_finalHTMLCode;

	QFile lib("libs\\js\\lib.js");
	lib.copy(m_path + "\\lib.js");
}

void CodeGenerator::SaveJS()
{
	QFile file(m_path + "\\script.js");
	file.open(QFile::WriteOnly);

	QTextStream stream(&file);

	// global var
	stream << m_globalVars << "\nvar sceneObjects = [];\n";

	// init
	stream << "\nfunction init()\n{\n" << m_init << "}";

	// render
	stream << "\n\nfunction render()\n{\nctx.fillStyle=\"#9E9E9E\";\nctx.fillRect(0, 0, 800, 600);\nfor (i = 0; i < sceneObjects.length; i++)\nsceneObjects[i].draw();\n setTimeout(render, 10);\n}";

	stream << "\ninit();\nrender();\n";
}
