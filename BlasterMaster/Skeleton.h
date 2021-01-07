#include"GameObject.h"
#include"Utils.h"
#include"Brick.h"
#include "SkeletonBullet.h"

#define SKELETON_BBOX_WALKING 18
#define SKELETON_BBOX_ATTACK 21

#define SKELETON_WALKING_SPEED 0.05f

#define SKELETON_STATE_WALKING	100
#define SKELETON_STATE_ATTACK	200

#define SKELETON_ANI_WALKING_LEFT	0
#define SKELETON_ANI_WALKING_RIGHT	1
#define SKELETON_ANI_ATTACK_LEFT	2
#define SKELETON_ANI_ATTACK_RIGHT	3

#define SKELETON_ATTACK_TIME 5000
#define SKELETON_BULLET_DIS	300

class Skeleton : public CGameObject
{
	//int randomX;
	Timer* time_attack;
	Timer* time_dis;
	int LastDis;
	vector<SkeletonBullet*> Bullet;
	LPGAMEOBJECT Target;
	bool IsAttack;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Skeleton(LPGAMEOBJECT);
	virtual void SetState(int state);
	//bool GetAttack() { return IsAttack; }
	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	bool Get_IsDead() { return this->IsDead; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	void SubDamage(int damage) { EnermiesHealth += damage; };
};