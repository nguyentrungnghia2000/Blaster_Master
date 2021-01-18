#pragma once
#include "Bullets.h"
#include "Brick.h"
#include <math.h>

#define BOSS_BULLET_SPEED 0.25f

#define BOSS_BULLET_ANI 855

#define	BOSS_BULLET_BBOX	7

class BossBullets :public Bullets
{
	LPANIMATION ani;
	LPGAMEOBJECT Target;
public:
	//SpiderBullets();
	BossBullets(int, int, float, LPGAMEOBJECT);
	~BossBullets();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();

	float CountVx(LPGAMEOBJECT, int, int);

	bool Get_IsDead() { return isFinish; }
	void Set_IsDead(bool isfinish) { isFinish = isfinish; };
	void SubDamage(int damage) { EnermiesHealth += damage; };
};

