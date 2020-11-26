#pragma once
#include "Bullets.h"
#include <math.h>

#define BULLET_SPEED	0.25f

#define BULLET_BBOX_WIDTH	4
#define BULLET_BBOX_HEIGHT	4

#define HUMAN_BULLET_ANI	0
#define HUMAN_BULLET_EXPLOTION_ANI	1

#define BULLETS_DAMAGE 1

#define ANIMATION_SET_HUMAN_BULLET		21
#define HUMAN_BULLET_DELAY				860

class HumanBullets : public Bullets
{
public:
	int temp;
	HumanBullets();
	~HumanBullets();

	bool Get_IsFinish() { return isFinish; }
	void Set_IsFinish(bool isfinish) { isFinish = isfinish; };
	void SubDamage(int damage) { EnermiesHealth += damage; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
};

