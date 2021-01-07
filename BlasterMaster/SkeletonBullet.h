#pragma once
#include "Bullets.h"
#include "Brick.h"
#include <math.h>

#define SKELETON_BULLET_SPEED 0.2f
#define SKELETON_GRAVITY 0.001f


#define SKELETON_BULLET_EX_ANI	1005

#define	SKELETON_BULLET_BBOX	10

#define SKELETON_BULLET_ANI	822

#define SKELETON_BULLET_TIME 1500


class SkeletonBullet :public Bullets
{
	LPANIMATION ani;
	LPANIMATION ani_ex;
	Timer* Time_Ex;
public:
	SkeletonBullet();
	SkeletonBullet(int, int);
	~SkeletonBullet();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	bool TimeUp();
	bool Get_IsDead() { return isFinish; }
	void Set_IsDead(bool isfinish) { isFinish = isfinish; };
	void SubDamage(int damage) { EnermiesHealth += damage; };
};