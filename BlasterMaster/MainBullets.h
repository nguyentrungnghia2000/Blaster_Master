#pragma once
#include "Bullets.h"

#include <math.h>

#define BULLET_SPEED					0.25f

#define SMALL_BULLET_BBOX_WIDTH			8
#define SMALL_BULLET_BBOX_HEIGHT		6

#define BIG_BULLET_BBOX_WIDTH			22
#define BIG_BULLET_BBOX_HEIGHT			6

#define CAR_BULLET_ANI_RIGHT	0
#define CAR_BULLET_ANI_LEFT		1
#define CAR_BULLET_ANI_TOP		2
#define CAR_BULLET_EXPLOTION_ANI	3

#define BULLETS_DAMAGE 1

#define ANIMATION_SET_MAIN_BULLET		20
#define SMALL_JASON_BULLET_DELAY				860

class MainBullets : public Bullets
{
public:
	int temp;
	MainBullets();
	~MainBullets();

	bool Get_IsFinish() { return isFinish; }
	void Set_IsFinish(bool isfinish) { isFinish = isfinish; };
	void SubDamage(int damage) { EnermiesHealth += damage; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
};

