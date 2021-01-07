#pragma once
#include "Bullets.h"
#include "Brick.h"
#include <math.h>

#define GUNNER_BULLET_SPEED 0.2f
#define GUNNER_GRAVITY 0.001f

#define	GUNNER_BULLET_BBOX 8


#define GUNNER_BULLET_EX_ANI	1005
#define GUNNER_BULLET_ANI	826

#define GUNNER_BULLET_TIME 1500
class GunnerBullets :public Bullets
{
	LPANIMATION ani;
	LPANIMATION ani_ex;
	//Timer* Time_Ex;
	int Type;
public:
	GunnerBullets();
	GunnerBullets(int, int, int);
	~GunnerBullets();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();

	bool Get_IsDead() { return isFinish; }
	void Set_IsDead(bool isfinish) { isFinish = isfinish; };
	void SubDamage(int damage) { EnermiesHealth += damage; };
};

