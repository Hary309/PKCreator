/*
@project: PK Creator
@author: Piotr Krupa
*/

var allObjects = [];
var allScenes = [];

// CurrentScene object
var currentScene;

var currentId = 0;

// Common
function GetID()
{
	currentId++;
	return currentId;
}

// Sprite
function Sprite(imagePath, centerX, centerY)
{
	this.img = new Image();
	this.img.src = imagePath;

	this.centerX = centerX;
	this.centerY = centerY;
}

// Object
function Object(id, sprite)
{
	this.id = id;
	this.sprite = sprite;
	this.vars = [];

	this.draw = function() {
		var rad = (this.angle) * Math.PI / 180;
		
		ctx.save();
		ctx.translate(this.x + this.sprite.centerX, this.y + this.sprite.centerY)
		ctx.rotate(rad);
		ctx.drawImage(this.sprite.img,-this.sprite.centerX * 2, -this.sprite.centerY * 2, this.sprite.img.width, this.sprite.img.height);
		ctx.restore();
	}
}

function AddObject(object)
{
	allObjects.push(object);
}

// CurrentScene && Instance
function CurrentScene(bgColor)
{
	this.bgColor = bgColor;
	this.instances = [];
}

function Instance(object, x, y)
{
	this.id = GetID();
	this.object = object;
	this.sprite = object.sprite;
	this.vars = object.vars;

	this.x = x;
	this.y = y;
	this.angle = 0;

	this.draw = object.draw;
}

function CreateInstance(objectID, x, y)
{
	for (var i = 0; i < allObjects.length; ++i)
	{
		if (allObjects[i].id == objectID)
		{
			var instance = new Instance(allObjects[i], x, y);

			currentScene.instances.push(instance);
			return instance;
		}
	}
}

function DestroyInstance(instanceID)
{
	for (var i = 0; i < currentScene.instances.length; ++i)
	{
		if (currentScene.instances[i].id == instanceID)
		{
			currentScene.instances.splice(i, 1);
		}
	}
}

// Scene
function Scene(id, bgColor)
{
	this.id = id;
	this.bgColor = bgColor;

	this.sceneObjects = [];
}

function SceneObject(objectID, x, y)
{
	this.objectID = objectID;
	this.x = x;
	this.y = y;
}

function AddScene(scene)
{
	allScenes.push(scene);
}

function UnloadCurrentScene()
{
	if (currentScene)
		currentScene.instances = [];
}

function LoadScene(sceneID)
{
	UnloadCurrentScene();

	for (var i = 0; i < allScenes.length; i++)
	{
		if (allScenes[i].id == sceneID)
		{
			currentScene = new CurrentScene(allScenes[i].bgColor);

			for (var j = 0; j < allScenes[i].sceneObjects.length; j++)
			{
				var sceneObject = allScenes[i].sceneObjects[j];

				CreateInstance(sceneObject.objectID, sceneObject.x, sceneObject.y);
			}
		}
	}
}

function PointDir(x1, y1, x2, y2)
{
	return Math.atan2((y1 - y2), (x1 - x2)) * 180 / Math.PI + 270;
}

// TBD