#pragma once
#include"GameObject.h"
#include"Utils.h"
#include"Brick.h"
#include "EyeballBullets.h"

#define TELE_BBOX_WIDTH 24
#define TELE_BBOX_HEIGHT 32

#define TELE_ANI_UNDEAD	0
#define TELE_ANI_TELE	1

#define TELE_STATE_UNDEAD	0
#define TELE_STATE_TELE		100

#define TELE_DELAY_ATTACK 6000
#define TELE_TIME_DIS	300

#define TIME_TELE	3000
#define TIME_DELAY_TELE	700
#define TIME_UNDEAD	3000

#define SCREEN_WIDTH	300
#define SCREEN_HEIGHT	240

class Tele :public CGameObject
{
	Timer* Time_tele;
	Timer* Time_delay_tele;
	Timer* Time_undead;
	vector<EyeballBullets*> Bullet;
	Timer* Delay_time;
	Timer* Time_dis;
	LPGAMEOBJECT Target;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Tele(LPGAMEOBJECT);
	virtual void SetState(int state);
	void Attack(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	bool Get_IsDead() { return this->IsDead; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	void SubDamage(int damage) { EnermiesHealth += damage; };
};

