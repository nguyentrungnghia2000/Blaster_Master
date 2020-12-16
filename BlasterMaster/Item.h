#include "GameObject.h"

#define ITEM_BBOX_WIDTH		20
#define ITEM_BBOX_HEIGHT	20

#define ITEM_STATE_ACTIVE	100
#define	ITEM_STATE_TIMEUP	200

#define P_ITEM_ANI_ACTIVE	0
#define P_ITEM_ANI_TIMEUP	1
#define H_ITEM_ANI_ACTIVE	2
#define H_ITEM_ANI_TIMEUP	3
#define LIGHTNING_ANI_ACTIVE	4
#define LIGHTNING_ANI_TIMEUP	5
#define ROCKET_ANI_ACTIVE	6
#define ROCKET_ANI_TIMEUP	7
#define HROCKET_ANI_ACTIVE	8
#define HROCKET_ANI_TIMEUP	9

#define ITEM_TIME_ACTIVE	10000
#define	ITEM_TIME_TIMEUP	8000	

class Item :public CGameObject
{
	int ID;
	Timer* Time_Active;
	
public:
	Item();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();

	float Get_width() { return width; }
	float Get_height() { return height; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }

	virtual void SetState(int state);
};
