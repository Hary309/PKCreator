/*
@project: PK Creator
@author: Piotr Krupa
*/

var canvas = document.getElementById('canvas');
var ctx = canvas.getContext('2d');

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

	this.events = [];

	this.draw = function() {
		if (this.events.pulseEvent)
			this.events.pulseEvent();

		var rad = (this.angle) * Math.PI / 180;
		
		ctx.save();
		ctx.translate(this.x + this.sprite.centerX, this.y + this.sprite.centerY)
		ctx.rotate(rad);
		ctx.drawImage(this.sprite.img,-this.sprite.centerX * 2, -this.sprite.centerY * 2, this.sprite.img.width, this.sprite.img.height);
		ctx.restore();

		if (this.events.renderEvent)
			this.events.renderEvent();
	}
}

function AddObject(object)
{
	allObjects.push(object);
}

// CurrentScene && Instance
function CurrentScene(scene)
{
	this.bgColor = scene.bgColor;
	this.scene = scene;

	this.instances = [];
}

function Instance(object, x, y)
{
	this.id = GetID();
	this.object = object;
	this.sprite = object.sprite;
	this.vars = object.vars;
	this.events = object.events;

	this.hspeed = 0;
	this.x = x;
	this.vspeed = 0;
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

			if (instance.events.createEvent)
				instance.events.createEvent(instance.id);

			currentScene.instances.push(instance);
			return instance.id;
		}
	}
}

function DestroyInstance(instanceID)
{
	console.log("Destroying " + instanceID);

	for (var i = 0; i < currentScene.instances.length; ++i)
	{
		var instance = currentScene.instances[i];
		if (instance.id == instanceID)
		{
			if (instance.events.destroyEvent)
				instance.events.destroyEvent(instance.id);

			currentScene.instances.splice(i, 1);
		}
	}
}

// Scene
function Scene(id, bgColor, bgImagePath, bgTile)
{
	this.id = id;
	this.bgColor = bgColor;
	this.bgImage = new Image();
	this.bgImage.src = bgImagePath;
	this.bgTile = bgTile;

	this.sceneObjects = [];

	this.draw = function() 
	{ 
		var maxX = 0;
		var maxY = 0;

		if (bgTile == 3 || bgTile == 1)
		{
			if (this.bgImage.width)
				maxX = canvas.width / this.bgImage.width;
		}

		if (bgTile == 3 || bgTile == 2)
		{
			if (this.bgImage.height)
				maxY = canvas.height / this.bgImage.height;
		}

		for (var i = 0; i <= maxX; i++)
		{
			for (var j = 0; j <= maxY; j++)
			{
				ctx.drawImage(this.bgImage, this.bgImage.width * i, this.bgImage.height * j);
			}
		}


	}
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
			currentScene = new CurrentScene(allScenes[i]);

			for (var j = 0; j < allScenes[i].sceneObjects.length; j++)
			{
				var sceneObject = allScenes[i].sceneObjects[j];

				CreateInstance(sceneObject.objectID, sceneObject.x, sceneObject.y);
			}
		}
	}
}



// Events
canvas.onmousemove = function(data) { 
	var x = data.x;
	var y = data.y;

	for (i = 0; i < currentScene.instances.length; i++)
	{
		var instance = currentScene.instances[i];

		if (instance.events.mouseMovedEvent)
			instance.events.mouseMovedEvent(instance.id,x,y);
	}
}

// Mouse pressed
canvas.onmousedown = function(data) { 
	var button = data.button;
	var x = data.x;
	var y = data.y;

	for (i = 0; i < currentScene.instances.length; i++)
	{
		var instance = currentScene.instances[i];

		if (instance.events.mouseDownEvent)
			instance.events.mouseDownEvent(instance.id,x,y,button);
	}
}

// Mouse released
canvas.onmouseup = function(data) { 
	var x = data.x;
	var y = data.y;

	for (i = 0; i < currentScene.instances.length; i++)
	{	
		var instance = currentScene.instances[i];

		if (instance.events.mouseUpEvent)
			instance.events.mouseUpEvent(instance.id,x,y,button);	
	}
}

window.onkeydown = function(data) { 
	var key = data.keyCode;

	for (i = 0; i < currentScene.instances.length; i++)
	{
		var instance = currentScene.instances[i];

		if (instance.events.keyDownEvent)
			instance.events.keyDownEvent(instance.id,key);
	}
}

window.onkeyup = function(data) { 
	var key = data.keyCode;

	for (i = 0; i < currentScene.instances.length; i++)
	{	
		var instance = currentScene.instances[i];

		if (instance.events.keyUpEvent)
			instance.events.keyUpEvent(instance.id,key);
	}
}
