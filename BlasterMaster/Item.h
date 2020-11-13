#include "GameObject.h"

#define ITEM_BBOX_WIDTH		16
#define ITEM_BBOX_HEIGHT	16

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
	//bool IsActive;
	Timer* Time_Active;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
public:
	Item();
	virtual void SetState(int state);
};
