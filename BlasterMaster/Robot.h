#include"GameObject.h"
#include"Utils.h"
#include"Brick.h"
#include"math.h"

#define ROBOT_WALKING_SPEED 0.04f
#define ROBOT_JUMP_SPEED_Y 0.3f
#define ROBOT_GRAVITY 0.001f
#define ROBOT_HUNTING_TIME 2500
#define ROBOT_SAVE_TIME 4000

#define ROBOT_BBOX_WIDTH 14
#define ROBOT_BBOX_HEIGHT 25

#define ROBOT_STATE_WALKING	100
#define ROBOT_STATE_JUMP 200
#define ROBOT_STATE_HUNTING 300
#define ROBOT_STATE_UNACTIVE 0

#define ROBOT_ANI_WALKING_LEFT 0
#define ROBOT_ANI_WALKING_RIGHT 1
#define ROBOT_ANI_JUMP_LEFT	2
#define ROBOT_ANI_JUMP_RIGHT	3

#define DISTANCE 50

class Robot : public CGameObject
{
	Timer* time;
	LPGAMEOBJECT target;
	bool IsJump;
	bool IsHunting;
	bool IsActive;
	int count = 0;
	int randomJump;
	//int jump;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Robot(LPGAMEOBJECT);
	virtual void SetState(int state);
	int Get_health() { return EnermiesHealth; }

	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }
};
