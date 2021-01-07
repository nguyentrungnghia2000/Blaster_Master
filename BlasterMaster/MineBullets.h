#pragma once
#include "Bullets.h"
#include"Utils.h"
#include "Brick.h"
#include <math.h>

#define MINE_BULLET_SPEED 0.005f
#define MINE_JUMP_SPEED_Y 0.12f
#define MINE_GRAVITY 0.001f

#define MINE_BULLET_ANI 817
#define SPIDER_BULLET_EX_ANI	828

#define	MINE_BULLET_BBOX	4

#define MINE_BULLET_ANI	0
#define TIME_JUMP	300
class MineBullets :public Bullets
{
	LPANIMATION ani;
	LPANIMATION ani_ex;
	Timer* Time_jump;
public:
	MineBullets();
	MineBullets(int, int);
	~MineBullets();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();

	bool Get_IsDead() { return isFinish; }
	void Set_IsDead(bool isfinish) { isFinish = isfinish; };
	void SubDamage(int damage) { EnermiesHealth += damage; };
};

