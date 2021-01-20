#include "Ladder.h"

Ladder::Ladder(float w, float h)
{
	this->W = w;
	this->H = h;
}

void Ladder::Render()
{
	//RenderBoundingBox();
}

void Ladder::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + this->W;
	b = y + this->H;
}
