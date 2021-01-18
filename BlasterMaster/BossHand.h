#pragma once
#include"Utils.h"
#include"Brick.h"
#include"Bullets.h"

#define BOSSHAND_SPEED 0.07f

#define BOSSHAND_BBOX_HEIGHT	32
#define BOSSHAND_BBOX_WIDTH		18

#define BOSSHAND_STATE_MOVE	0

#define BOSSHAND_ANI_LEFT	0
#define BOSSHAND_ANI_RIGHT	1

#define BOSSHAND_TIME	1000
class BossHand :public Bullets
{
	int change_move;
	int type_move;
	int type_hand;
	LPANIMATION ani;
	Timer* time_move_type;
	Timer* time_delay;
	float FollowBossX, FollowBossY;
public:
	BossHand(int, int, int);	
	virtual void SetState(int state);
	float ReturnX();
	float ReturnY();
	int Get_ChangeMove() { return change_move; }
	void Set_ChangeMove(int change) { change_move = change; }
	void Set_FollowBossX(float dx_boss) { FollowBossX = dx_boss; }
	void Set_FollowBossY(float dy_boss) { FollowBossY = dy_boss; }
	//bool GetAttack() { return IsAttack; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	//bool TimeUp();
	bool Get_IsDead() { return isFinish; }
	void Set_IsDead(bool isfinish) { isFinish = isfinish; };
	void SubDamage(int damage) { EnermiesHealth += damage; };
};

