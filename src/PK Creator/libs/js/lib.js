/*
@project: PK Creator
@author: Piotr Krupa
*/

function Object(imagePath)
{
	this.img = new Image();
	this.img.src = imagePath;

	this.origin = new Vector(0, 0);
}

function SceneObject(object, pos)
{
	this.object = object;
	this.pos = pos;
	this.angle = 0;

	this.draw = function() {
		var rad = (this.angle) * Math.PI / 180;
		
		ctx.save();
		ctx.translate(this.pos.x + this.object.origin.x, this.pos.y + this.object.origin.y)
		ctx.rotate(rad);
		ctx.drawImage(this.object.img,-this.object.origin.x * 2, -this.object.origin.y * 2, this.object.img.width, this.object.img.height);
		ctx.restore();
	}
}

function Scene(bgColor)
{
	this.bgColor = bgColor;

	this.objects = [];
}

function Vector(x, y)
{
	this.x = x;
	this.y = y;
}

function PointDir(x1, y1, x2, y2)
{
	return Math.atan2((y1 - y2), (x1 - x2)) * 180 / Math.PI + 270;
}

// TBD