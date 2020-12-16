#pragma once
#include "GameObject.h"

class Lava : public CGameObject
{
	float W, H;
	bool IsFinish;
public:
	Lava(float w, float h);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	float Get_width() { return W; }
	float Get_height() { return H; }

	void Set_IsDead(bool value) { this->IsFinish = value; }
	bool Get_IsDead() { return this->IsFinish; }
	void SubDamage(int damage) { EnermiesHealth += damage; }
};

