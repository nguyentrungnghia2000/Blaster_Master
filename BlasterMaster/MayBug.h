#include "GameObject.h"
#include "Brick.h"

#define MAYBUG_SPEED_X 0.09f
#define MAYBUG_SPEED_Y 0.01f

#define BEE_BBOX_WIDTH 18
#define BEE_BBOX_HEIGHT 18

#define MAYBUG_FLYING_TIME 5000
#define MAYBUG_ROLL_TIME_MAX 1000
#define MAYBUG_ROLL_TIME_MIN 500

#define	MAYBUG_STATE_UNACTIVE 0
#define MAYBUG_STATE_FLYING 100
#define MAYBUG_STATE_ROLLING 200

#define MAYBUG_ANI_FLYING_LEFT 0
#define MAYBUG_ANI_FLYING_RIGHT 1
#define MAYBUG_ANI_ROLLING 2

#define DISTANCE 100

class MayBug :public CGameObject
{
	Timer* time;
	bool IsFlyingLeft;
	bool IsActive;
	//bool IsFinish;
	int RandomUpDown;
	LPGAMEOBJECT target;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	MayBug(LPGAMEOBJECT);

	float Get_width() { return width; }
	float Get_height() { return height; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }

	virtual void SetState(int state);
};

