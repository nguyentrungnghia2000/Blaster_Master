#include "Lava.h"

Lava::Lava(float w, float h)
{
	this->W = w;
	this->H = h;
}

void Lava::Render()
{
	//RenderBoundingBox();
}

void Lava::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + this->W;
	b = y + this->H;
}
