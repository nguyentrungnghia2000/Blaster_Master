#include"GameObject.h"
#include"Utils.h"
#include"Brick.h"

#define SPIDER_BBOX_WIDTH 18
#define SPIDER_BBOX_HEIGHT 16

#define SPIDER_STATE_WALKING	100

#define SPIDER_ANI_WALKING_LEFT 0
#define SPIDER_ANI_WALKING_RIGHT 1

class Spider : public CGameObject
{
	//int randomX;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Spider(LPGAMEOBJECT);
	virtual void SetState(int state);
};