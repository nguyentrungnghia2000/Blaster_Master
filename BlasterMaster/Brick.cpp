#include "Brick.h"

void CBrick::Render()
{
	RenderBoundingBox();
	if(istemp)animation_set->at(0)->Render(x, y);
	istemp = 0;
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void CBrick::SetRender()
{
	istemp = true;
}
