#include "Arrows.h"

Arrows::Arrows(float w, float h)
{
	this->W = w;
	this->H = h;
	this->IsDead = false;
	this->EnermiesHealth = ENERMIES_HEALTH;
}

void Arrows::Render()
{
	RenderBoundingBox();
}

void Arrows::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (EnermiesHealth <= 0) {
		IsDead = true;
	}
}

void Arrows::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + this->W;
	b = y + this->H;
}
