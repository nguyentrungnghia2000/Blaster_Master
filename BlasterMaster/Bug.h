#include "GameObject.h"
#include "Brick.h"

#define BUG_WALKING_SPEED 0.04f
#define BUG_JUMP_SPEED_Y 0.2f
#define BUG_GRAVITY 0.001f

#define BUG_BBOX_WIDTH 18
#define BUG_BBOX_HEIGHT 10

#define BUG_STATE_UNACTIVE 0
#define BUG_STATE_WALKING	100
#define BUG_STATE_JUMPING 200
#define BUG_STATE_HUNTING 300

#define BUG_ANI_WALKING_LEFT 0
#define BUG_ANI_WALKING_RIGHT 1


#define DISTANCE_TURNAROUND 30
class Bug :public CGameObject
{
	LPGAMEOBJECT target;
	bool IsJump;
	bool IsActive;
	//bool IsFinish;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Bug(LPGAMEOBJECT);

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }

	virtual void SetState(int state);
};