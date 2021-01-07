#pragma once
#include "Bullets.h"
#include "Brick.h"
#include <math.h>


#define EYEYBALL_BULLET_SPEED 0.2f
#define EYEBALL_GRAVITY 0.001f

#define	EYEBALL_BULLET_BBOX 8

#define EYEBALL_BULLET_EX_ANI	1005
#define EYEBALL_BULLET_ANI	826

class EyeballBullets :public Bullets
{
	LPANIMATION ani;
	LPANIMATION ani_ex;
	LPGAMEOBJECT Target;
	Timer* Delay_time;
	//Timer* Time_Ex;
	int Type;
public:
	EyeballBullets();
	EyeballBullets(LPGAMEOBJECT, int, int);
	~EyeballBullets();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	float CountVx(LPGAMEOBJECT, int, int);
	void Attack();

	bool Get_IsDead() { return isFinish; }
	void Set_IsDead(bool isfinish) { isFinish = isfinish; };
	void SubDamage(int damage) { EnermiesHealth += damage; };
};

