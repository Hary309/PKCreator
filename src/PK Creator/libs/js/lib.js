/*
@project: PK Creator
@author: Piotr Krupa
*/

function Object(imagePath)
{
	this.img = new Image();
	this.img.src = imagePath;

	this.centerX = centerX;
	this.centerY = centerY;
}

function SceneObject(object, pos)
{
	this.object = object;
	this.x = x;
	this.y = y;
	this.angle = 0;

	this.draw = function() {
		var rad = (this.angle) * Math.PI / 180;
		
		ctx.save();
		ctx.translate(this.x + this.sprite.centerX, this.y + this.sprite.centerY)
		ctx.rotate(rad);
		ctx.drawImage(this.sprite.img,-this.sprite.centerX * 2, -this.sprite.centerY * 2, this.sprite.img.width, this.sprite.img.height);
		ctx.restore();
	}
}

function Scene(bgColor)
{
	this.bgColor = bgColor;

	this.objects = [];
}

function PointDir(x1, y1, x2, y2)
{
	return Math.atan2((y1 - y2), (x1 - x2)) * 180 / Math.PI + 270;
}

// TBD