#pragma once
#include "GameObject.h"
#include "Utils.h"

#include <map>

#define BULLET_BBOX_WIDTH	26
#define BULLET_BBOX_HEIGHT	8
#define DISTANCE_TO_CAR_WIDTH_RIGHT	18
#define DISTANCE_TO_CAR_WIDTH_LEFT	1
#define DISTANCE_TO_CAR_HEIGTH	1
#define DISTANCE_TO_CAR_FLIP_UP_WIDTH	11
#define DISTANCE_TO_CAR_FLIP_UP_HEIGHT	-14
#define DISTANCE_TO_FIRE	6
#define DISTANCE_TO_HUMAN_WIDTH_RIGHT	10
#define DISTANCE_TO_HUMAN_WIDTH_LEFT	1
#define DISTANCE_TO_HUMAN_HEIGTH	6

#define COLLISION_WALL_ANI	0
#define COLLISION_ENERMIES_ANI	1
#define DISTANCE_TO_BANG		4

#define EXPLOTION_TIME	500

class Bullets;
typedef Bullets* LPBULLETS;
class Bullets : public CGameObject
{
protected:
	bool IsCollisionBrick;
	bool IsCollisionEnermies;
	bool IsDone;
	bool isFinish, flag;
	int damage;
	int alpha;
	float TimeDelay, TimeDelayMax;
	DWORD timer;
public:
	bool IsTargetTop;
	int BulletDirection;
	Bullets();
	~Bullets();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	//virtual void Fire(int direct, bool isGunFlip, float posX, float posY) { direction = direct; IsTargetTop = isGunFlip; x = posX + DISTANCE_TO_CAR_WIDTH_RIGHT; y = posY + DISTANCE_TO_CAR_WIDTH_LEFT; alpha = 255; IsDone = false; IsCollisionBrick = 0; }
	void ResetDelay() { TimeDelay = 0; }
	bool GetIsDone() { return IsDone; }
	void SetIsDone(bool isdone) { IsDone = isdone; }

	float Get_width() { return width; }
	float Get_height() { return height; }
	bool Get_IsDead() { return isFinish; }
	void Set_IsDead(bool isfinish) { isFinish = isfinish; };
	bool Get_IsCollision();

	virtual bool Get_IsFinish() { return isFinish; }
	virtual void Set_IsFinish(bool isfinish) { isFinish = isfinish; };
	int GetDamage() { return damage; }

};

