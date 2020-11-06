#pragma once
#include "Bullets.h"
#include <math.h>

#define BULLET_SPEED					0.25f

#define SMALL_BULLET_BBOX_WIDTH			8
#define SMALL_BULLET_BBOX_HEIGHT		6

#define BIG_BULLET_BBOX_WIDTH			22
#define BIG_BULLET_BBOX_HEIGHT			6

#define SMALL_BULLET_JASON_ANI_RIGHT	0
#define SMALL_BULLET_JASON_BANG_ANI		1
#define SMALL_BULLET_JASON_ANI_TOP		2

#define ANIMATION_SET_SMALL_JASON_BULLET		20
#define SMALL_JASON_BULLET_DELAY				860

class MainBullets : public Bullets
{
public:

	MainBullets();
	~MainBullets();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
};

