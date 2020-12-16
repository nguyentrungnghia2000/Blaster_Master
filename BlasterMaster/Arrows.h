#pragma once
#include "GameObject.h"

class Arrows : public CGameObject
{
	float W, H;
	bool IsFinish;
public:
	Arrows(float w, float h);
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	float Get_width() { return W; }
	float Get_height() { return H; }
	void Set_IsDead(bool value) { this->IsFinish = value; }
	bool Get_IsDead() { return this->IsFinish; }
	void SubDamage(int damage) { EnermiesHealth += damage; }
};

