#pragma once
#include "GameObject.h"
#include"Utils.h"
#include"Brick.h"
#include"GunnerBullets.h"

#define GUNNER_BBOX 26

#define GUNNER_STATE_NOLMAL	100
#define GUNNER_STATE_ATTACK 200

#define GUNNER_ANI_NOLMAL 0
#define GUNNER_ANI_ATTACK_TD 1
#define GUNNER_ANI_ATTACK_LR 2

#define DELAY_TIME 1000
#define GUNNER_BULLET_DIS	300


class Gunner : public CGameObject
{
	int type;
	Timer* Time_Delay;
	vector<GunnerBullets*>Bullet;
	Timer* time_dis;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Gunner();
	virtual void SetState(int state);

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	bool Get_IsDead() { return this->IsDead; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	void SubDamage(int damage) { EnermiesHealth += damage; };
};

