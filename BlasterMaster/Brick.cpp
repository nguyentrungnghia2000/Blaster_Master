#include "Brick.h"

CBrick::CBrick(float width, float height)
{
	this->width = width;
	this->height = height;
	//this->IsFinish = false;
}

void CBrick::Render()
{
	//animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + this->width;
	b = y + this->height;
}