#pragma once
#include "GameObject.h"
#include "Utils.h"
#include <map>

#define BULLET_BBOX_WIDTH	26
#define BULLET_BBOX_HEIGHT	8
#define DISTANCE_TO_PLAYER_WIDTH_RIGHT	18
#define DISTANCE_TO_PLAYER_WIDTH_LEFT	1
#define DISTANCE_TO_PLAYER_HEIGTH	1
#define DISTANCE_TO_FIRE	6

#define COLLISION_WALL_ANI	0
#define COLLISION_ENERMIES_ANI	1
#define DISTANCE_TO_BANG		4

#define EXPLOTION_TIME	300

class Bullets;
typedef Bullets* LPBULLETS;
class Bullets : public CGameObject
{
protected:
	bool IsCollision;
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

	virtual void Fire(int direct, bool isGunFlip, float posX, float posY) { direction = direct; IsTargetTop = isGunFlip; x = posX + DISTANCE_TO_PLAYER_WIDTH_RIGHT; y = posY + DISTANCE_TO_PLAYER_HEIGTH; alpha = 255; IsDone = false; IsCollision = 0; }
	void ResetDelay() { TimeDelay = 0; }
	bool GetIsDone() { return IsDone; }
	void SetIsDone(bool isdone) { IsDone = isdone; }
	bool Get_IsFinish() { return isFinish; }
	void Set_IsFinish(bool isfinish) { isFinish = isfinish; }
	int GetDamage() { return damage; }

};

