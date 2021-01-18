#pragma once
#include"GameObject.h"
#include"Utils.h"
#include"Brick.h"
#include "BossHand.h"
#include "FlangeHand.h"
#include "BossBullets.h"

#define BOSS_SPEED 0.01f

#define BOSS_STATE_MOVE	0

#define BOSS_BBOX_WIDTH 60
#define BOSS_BBOX_HEIGHT 64

#define BOSS_TIME_MOVE	5000

#define TIME_DELAY_ATTACK	2000
#define TIME_ONESHOT	500

#define SCREEN_WIDTH	300
#define SCREEN_HEIGHT	240
class Boss:public CGameObject
{
	BossHand* RightHand;
	BossHand* LeftHand;
	vector <FlangeHand*> Flange;
	vector <FlangeHand*> Flange2;
	Timer *Time_move;
	Timer* Time_delay_attack;
	LPGAMEOBJECT Target;
	vector<BossBullets*>Bullets;
	//Timer* Time_shot;
	int where;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Boss(LPGAMEOBJECT,int ,int);
	virtual void SetState(int state);
	void Attack(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	//bool GetAttack() { return IsAttack; }

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	bool Get_IsDead() { return this->IsDead; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	void SubDamage(int damage) { EnermiesHealth += damage; };
	float GetDx() { return dx; }
	float GetDy() { return dy; }
};

