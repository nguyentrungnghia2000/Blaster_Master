#pragma once
#include"GameObject.h"
#include"Utils.h"
#include"Brick.h"
#include "EyeballBullets.h"

#define EYEBALL_SPEED_Y_TYPE1 0.01f
#define EYEBALL_GRAVITY_TYPE2 0.001f
#define EYEBALL_GRAVITY_TYPE3 0.0005f
#define EYEBALL_JUMP_TYPE2 0.23f
#define EYEBALL_JUMP_TYPE3 0.15f
#define EYEBALL_SPEED_TYPE3 0.05f

#define EYEBALL_BBOX 16

#define EYEBALL_STATE_NORMAL_TYPE1 0
#define EYEBALL_STATE_NORMAL_TYPE2	100
#define EYEBALL_STATE_NORMAL_TYPE3	200
#define EYEBALL_STATE_ATTACK	300

#define EYEBALL_NORMAL_ANI	0
#define EYEBALL_ATTACK_ANI	1

#define TIME_HUNTING	2000
#define TIME_DELAY_HUNTING	3000

#define TIME_DELAY_JUMP	3000

#define EYEBALL_DELAY_ATTACK 3000

#define TIME_DIS 300

// Loại 1: Follow target
// Loại 2: Phóng ra từ top và left right
// Loại 3: Nhảy cầu vồng
class Eyeball :public CGameObject
{
	int type;
	int type_2;//1 trên, 2 trái, 3 phải
	int type_3;//1 trên, 2 trái, 3 phải
	Timer* time_hunting_type1;
	Timer* time_delay_type1;
	Timer* time_delay_type2;
	Timer* time_delay_type3;
	Timer* Delay_time;
	Timer* Time_dis;
	vector<EyeballBullets*> Bullet;
	int count_jump;
	LPGAMEOBJECT Target;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Eyeball(LPGAMEOBJECT, int, int);
	virtual void SetState(int state);
	float CountVx(LPGAMEOBJECT, int, int);
	void Update_Type_1(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Update_Type_2(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Update_Type_3(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Attack(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	bool Get_IsDead() { return this->IsDead; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	void SubDamage(int damage) { EnermiesHealth += damage; };
};

