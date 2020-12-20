#pragma once
#include "GameObject.h"

#define ANI_SET_INTRO	3
#define ANI_INTRO	0

#define INTRO_TIME	16000

class Intro : public CGameObject
{
	LPANIMATION_SET ani_set;
	DWORD timer;
public:
	Intro();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects=NULL);
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }
};

