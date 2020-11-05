#include "Bullets.h"

Bullets::Bullets()
{
	IsDone = true;
	damage = 1;
}

Bullets::~Bullets()
{
}

void Bullets::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void Bullets::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void Bullets::Render()
{
}
