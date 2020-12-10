#include"GameObject.h"
#include"Utils.h"
#include"Brick.h"
#include "SpiderBullets.h"

#define SPIDER_BBOX_WIDTH 18
#define SPIDER_BBOX_HEIGHT 16

#define SPIDER_STATE_WALKING	100

#define SPIDER_ANI_WALKING_LEFT 0
#define SPIDER_ANI_WALKING_RIGHT 1

#define SPIDER_ATTACK_TIME 3000
#define SPIDER_BULLET_DIS	300

class Spider : public CGameObject
{
	//int randomX;
	Timer* time_attack;
	Timer* time_dis;
	vector<SpiderBullets*> Bullet;
	LPGAMEOBJECT Target;
	bool IsAttack;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Spider(LPGAMEOBJECT);
	virtual void SetState(int state);
	bool GetAttack() { return IsAttack; }

	bool Get_IsDead() { return this->IsDead; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	void SubDamage(int damage) { EnermiesHealth += damage; };
};