/*
*********************************************************************
* File          : HTML5Generator.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "HTML5Generator.h"

#include <QDir>
#include <QTextStream>

#include <ResourceView.h>
#include <SpriteItem.h>
#include <BackgroundItem.h>
#include <FontItem.h>
#include <ObjectItem.h>
#include <SceneItem.h>
#include <Var.h>
#include <EventObjectItem.h>
#include <Node.h>
#include <Widget.h>
#include <FunctionDefsMgr.h>
#include <GlobalVariablesWindow.h>
#include <Config.h>

#include <SFML/Graphics.hpp>

HTML5Generator::HTML5Generator(const QString &path)
	: CodeGenerator()
{
	m_path = path + "\\HTML5";

	QDir dir(m_path);
	dir.mkpath(m_path);
}


HTML5Generator::~HTML5Generator()
{
}

void HTML5Generator::GenerateCanvas(const QString &title, const QSize &windowSize)
{
	m_windowSize = windowSize;

	// head
	m_htmlCode = "<!DOCTYPE html>\r<html>\r<head>\r<title>" + title + "</title>\r</head>\r<body style='margin:0;padding:0;'>\r";

	// body
	m_htmlCode += "<canvas id ='canvas' width='" + QString::number(windowSize.width()) + "' height='" + QString::number(windowSize.height()) + "' style='border:1px solid black;'></canvas>\r";

	// scripts
	m_htmlCode += "<script src='lib.js'></script>\r<script src='script.js'></script>\r</body>\r</html>";
}

void HTML5Generator::GenerateGlobalVars(const GlobalVariablesWindow *globalVarsWnd)
{
	auto vars = globalVarsWnd->m_vars;

	for (auto var : vars)
	{
		m_global += "var globalVar" + var->m_name + " = ";

		switch (var->m_type)
		{
		case DATA_INTEGER:
			m_global += QString::number(var->m_data.integer);
			break;
		case DATA_NUMBER:
			m_global += QString::number(var->m_data.number, 'f', 10);
			break;
		case DATA_STRING:
			m_global += "'" + *var->m_data.string + "'";
			break;
		case DATA_BOOLEAN:
			m_global += var->m_data.boolean ? "true" : "false";
			break;
		default:;
		}

		m_global += ";\n";
	}
}

void HTML5Generator::GenerateFont(FontItem *fontItem)
{
	QString fontName = "gameFont" + fontItem->GetName();

	QString strColor = "rgb(" + QString::number(fontItem->GetColor().red()) + "," + QString::number(fontItem->GetColor().green()) + "," + QString::number(fontItem->GetColor().blue()) + ")";

	m_init += "AddFont(new Font(" + QString::number(fontItem->GetID()) + ",'" + fontItem->GetFontFamily() +
		"'," + QString::number(fontItem->GetSize()) + ",'" + strColor + "','";

	auto textAlign = fontItem->GetTextAlign();

	if (textAlign == FontItem::LEFT)
		m_init += "left";
	else if (textAlign == FontItem::CENTER)
		m_init += "center";
	else if (textAlign == FontItem::RIGHT)
		m_init += "right";

	m_init += "'));\n";
}

void HTML5Generator::GenerateSprite(SpriteItem *sprite)
{
	if (!sprite)
		return;

	QString spriteName = "gameSprite" + sprite->GetName();

	QString newTexturePath = spriteName + "." + sprite->GetTexPath().split(".").last();

	QFile tex(ResourceView::Get()->GetMainDir() + sprite->GetTexPath());
	tex.copy(m_path + "\\" + newTexturePath);

	m_init += "var " + spriteName + " = new Sprite('" + newTexturePath + "'," + QString::number(sprite->GetCenter().x()) + "," + QString::number(sprite->GetCenter().y()) + ");\n";
}

void HTML5Generator::GenerateObject(ObjectItem *object)
{
	if (!object)
		return;

	if (!object->GetSprite())
		return;

	QString objectName = "gameObject" + object->GetName();

	m_init += "var " + objectName + " = new Object(" + QString::number(object->GetID()) + ", gameSprite" + object->GetSprite()->GetName() + ", " + (object->IsSolid() ? "true" : "false") + ", " + (object->IsVisible() ? "true" : "false") + ");\n";

	for (auto var : *object->GetVars())
	{
		m_init += objectName + ".vars." + var->m_name + " = ";

		switch (var->m_type)
		{
		case DATA_INTEGER:
			m_init += QString::number(var->m_data.integer);
			break;
		case DATA_NUMBER:
			m_init += QString::number(var->m_data.number, 'f', 10);
			break;
		case DATA_STRING:
			m_init += "'" + *var->m_data.string + "'";
			break;
		case DATA_BOOLEAN:
			m_init += var->m_data.boolean ? "true" : "false";
			break;
		default: ;
		}

		m_init += ";\n";
	}

	auto events = object->GetEvents();

	if (!events->isEmpty())
	{
		printf("\tGenerating events logic...\n");

		for (auto sharedEvent : *events)
		{
			if (sharedEvent)
			{
				EventObjectItem *e = sharedEvent.data();
				
				if (!e)
					continue;
				
				auto firstNode = e->GetFirstNode();

				if (e->GetType() == EventDefsMgr::COLLISION_EVENT)
				{
					auto collideWith = ResourceView::Get()->GetItem(e->m_idCollideWith);

					if (!collideWith)
						continue;

					printf("\t\tCollision with %s Event...\n", collideWith->GetName().toStdString().c_str());

					if (firstNode->m_type != Node::EVENT)
						continue;

					QString declaration = objectName + ".addCollisionListener(" + QString::number(e->m_idCollideWith) + ", function(";

					for (int i = 0; i < firstNode->m_outputs.size(); ++i)
					{
						if (i > 0)
							declaration += ",";

						declaration += "v" + QString::number(firstNode->m_outputs[i]->m_id);
					}

					declaration += ") {";

					auto nextNode = e->GetNode(firstNode->m_idExecTo);

					declaration += GenerateFunction(e, nextNode, "");

					declaration += "});\n";

					m_init += declaration;
				}
				else
				{
					printf("\t\t%s...\n", EventDefsMgr::Get()->GetEvent(e->GetType())->name.toStdString().c_str());

					if (firstNode->m_type != Node::EVENT)
						continue;

					auto eventDef = EventDefsMgr::Get()->GetEvent(e->GetType());

					if (!eventDef)
						continue;

					QString declaration = objectName + ".events." + eventDef->functionName + " = function(";

					for (int i = 0; i < firstNode->m_outputs.size(); ++i)
					{
						if (i > 0)
							declaration += ",";

						declaration += "v" + QString::number(firstNode->m_outputs[i]->m_id);
					}

					declaration += ") {";

					auto nextNode = e->GetNode(firstNode->m_idExecTo);

					declaration += GenerateFunction(e, nextNode, "");

					declaration += "}\n";

					m_init += declaration;
				}
			}
		}
	}

	m_init += "AddObject(" + objectName + ");\n";
}

QString HTML5Generator::GenerateFunction(EventObjectItem *e, Node *node, QString code)
{
	QString result = code;

	if (!node)
		return result;

	if (node->m_type == Node::CONDITION)
	{
		if (node->m_outputs.size() == 2 && (node->m_inputs.size() == 1 || node->m_inputs.size() == 2))
		{
			if (!node->m_outputs[0]->m_connected.isEmpty() || !node->m_outputs[1]->m_connected.isEmpty())
			{
				if (node->m_inputs.size() == 1)
				{
					result += "if (";
					result += GetVar(e, node->m_inputs[0]->m_connected.first());
					result += ")";
				}
				else if (node->m_inputs.size() == 2)
				{
					result += "if (";
					result += GetVar(e, node->m_inputs[0]->m_connected.first());
					result += node->m_additionalData;
					result += GetVar(e, node->m_inputs[1]->m_connected.first());
					result += ")";
				}

				// if true
				if (!node->m_outputs[0]->m_connected.isEmpty())
				{
					result += "{";
					result += GenerateFunction(e, e->GetNode(node->m_outputs[0]->m_connected.first()), "");
					result += "}";
				}

				// if false
				if (!node->m_outputs[1]->m_connected.isEmpty())
				{
					result += "else{";
					result += GenerateFunction(e, e->GetNode(node->m_outputs[1]->m_connected.first()), "");
					result += "}";
				}
			}
		}

		if (node->m_idExecTo)
			return GenerateFunction(e, e->GetNode(node->m_idExecTo), result);

		return result;
	}
	else
	{
		if (node->m_outputs.size())
			result += "var v" + QString::number(node->m_outputs.first()->m_id) + " = ";

		result += node->m_name + "(";

		for (int i = 0; i < node->m_inputs.size(); ++i)
		{
			if (i > 0)
				result += ",";

			if (node->m_inputs[i]->m_connected.isEmpty())
			{
				result += "0";
				continue;
			}

			qint64 id = node->m_inputs[i]->m_connected.first();

			result += GetVar(e, id);
		}

		result += ");";

		for (auto output : node->m_outputs)
		{
			for (auto connID : output->m_connected)
			{
				auto var = e->GetParent()->GetVarByWidget(connID, e->GetType());

				auto globalVarNode = e->GetParent()->GetNodeByWidget(connID, e->GetType());
				Var *globalVar = nullptr;

				if (globalVarNode)
					globalVar = GlobalVariablesWindow::Get()->GetVar(globalVarNode->GetID());

				if (var)
					result += "gameObject" + e->GetParent()->GetName() + ".vars." + var->m_name + " = v" + QString::number(node->m_outputs.first()->m_id) + ";";
				else if (globalVar)
					result += "globalVar" + globalVar->m_name + " = v" + QString::number(node->m_outputs.first()->m_id) + ";";

			}
		}

		if (!node->m_idExecTo)
			return result;

		return GenerateFunction(e, e->GetNode(node->m_idExecTo), result);
	}
}

QString HTML5Generator::GetVar(EventObjectItem *e, qint64 id)
{
	QString result;

	auto var = e->GetParent()->GetVarByWidget(id, e->GetType());
	auto inlineVar = e->GetParent()->GetInlineVarValue(id, e->GetType());

	auto globalVarNode = e->GetParent()->GetNodeByWidget(id, e->GetType());
	Var *globalVar = nullptr;

	if (globalVarNode)
		globalVar = GlobalVariablesWindow::Get()->GetVar(globalVarNode->GetID());

	if (var)
		result += "gameObject" + e->GetParent()->GetName() + ".vars." + var->m_name;
	else if (!inlineVar.isEmpty())
		result += inlineVar;
	else if (globalVar)
		result += "globalVar" + globalVar->m_name;
	else
		result += "v" + QString::number(id);

	return result;
}

void HTML5Generator::GenerateScene(SceneItem *scene)
{
	if (!scene)
		return;
	
	QString sceneName = "gameScene" + scene->GetName();

	sf::Color color = sf::Color(scene->GetBgColor());

	auto objects = scene->GetObjects();

	QString textureName = "";

	if (scene->m_pBackground)
	{
		textureName = sceneName + "Background." + scene->m_pBackground->GetTexPath().split(".").last();;
		 
		QFile tex(ResourceView::Get()->GetMainDir() + scene->m_pBackground->GetTexPath());
		tex.copy(m_path + "\\" + textureName);
	}

	int bgTile = (scene->m_tileHor ? 1 : 0) + (scene->m_tileVer ? 2 : 0);

	m_init +=	"var " + sceneName + " = new Scene(" + QString::number(scene->GetID()) + 
				", 'rgb(" + QString::number(color.r) + "," + QString::number(color.g) + "," + QString::number(color.b) + ")'" +
				", '" + textureName + "'" +
				", " + QString::number(bgTile) + ");\n";


	int i = 0;

	for (auto sceneObject : *objects)
	{
		QString sceneObjectName = sceneName + "Object" + QString::number(i);

		m_init += "var " + sceneObjectName + " = new SceneObject(" + QString::number(sceneObject->pObj->GetID()) + ", " + QString::number(sceneObject->pos.x()) + ", " + QString::number(sceneObject->pos.y()) + "); \n";

		m_init += sceneName + ".sceneObjects.push("+ sceneObjectName +");\n";

		i++;
	}

	m_init += "AddScene(" + sceneName + ");\n";
}

void HTML5Generator::Save()
{
	QFile htmlFile(m_path + "\\index.html");
	htmlFile.open(QFile::WriteOnly);

	QTextStream stream(&htmlFile);

	QDateTime date = QDateTime::currentDateTime();

	// Header 
	stream << "<!--=================================================-->\n";
	stream << "<!-- Generated by PK Creator							-->\n";
	stream << "<!-- PK Creator by Piotr Krupa						-->\n";
	stream << "<!-- Source: https://github.com/Harry09/PKCreator	-->\n";
	stream << "<!-- Date: " + date.toString("HH:mm dd.MM.yyyy") + "	-->\n";
	stream << "<!--=================================================-->\n";


	stream << m_htmlCode;
	htmlFile.close();

	QFile jsFile(m_path + "\\script.js");
	jsFile.open(QFile::WriteOnly);

	stream.setDevice(&jsFile);

	// Header
	stream << "/*=================================================*/\n";
	stream << " // Generated by PK Creator\n";
	stream << " // PK Creator by Piotr Krupa\n";
	stream << " // Source: https://github.com/Harry09/PKCreator\n";
	stream << " // Date: " + date.toString("HH:mm dd.MM.yyyy") + " \n";
	stream << "/*=================================================*/\n\n";

	auto startingSceneId = ResourceView::Get()->GetConfig()->GetStartingSceneId();

	QString startingSceneStr;

	if (startingSceneId)
		startingSceneStr = QString::number(startingSceneId);
	else
		startingSceneStr = QString("allScenes[0].id");

	// init
	stream << m_global << "\nfunction init()\n{\n" << m_init << "LoadScene(" + startingSceneStr + ")\n" << "}";

	// render
	stream <<
		"\n\nfunction render()\n"
		"{\n"
		"ctx.fillStyle=currentScene.bgColor;\n"
		"ctx.fillRect(0, 0, canvas.width, canvas.height);\n"
		"currentScene.scene.draw();\n"
		"for (i = 0; i < currentScene.instances.length; i++)\n"
			"\tcurrentScene.instances[i].pulse();\n"
		"for (i = 0; i < currentScene.instances.length; i++)\n"
			"\tcurrentScene.instances[i].draw();\n"
		"}";

	stream << "\ninit();\nsetInterval(render, 20);\n\n";

	auto functionsDefs = ResourceView::Get()->GetFunctionDefsMgr()->GetFunctionsDef();

	for (auto sharedFuncDef : *functionsDefs)
	{
		FunctionDefsMgr::FunctionDef *funcDef = sharedFuncDef.data();
		
		if (funcDef->jsCode.isEmpty())
			continue;

		QString funcBody = "function ";
		
		funcBody += funcDef->name + "(";

		for (int i = 0; i < funcDef->args.size(); ++i)
		{
			if (i > 0)
				funcBody += ",";
			
			funcBody += funcDef->args[i].name;
		}

		funcBody += ")\n{\n";

		funcBody += funcDef->jsCode + "\n";

		funcBody += "}\n\n";

		stream << funcBody;
	}

	// copy lib
	QFile lib("libs\\js\\lib.js");
	lib.copy(m_path + "\\lib.js");
}
