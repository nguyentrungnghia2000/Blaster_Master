#pragma once
#include"Utils.h"
#include"Brick.h"
#include"Bullets.h"
#include"BossHand.h"

#define FLANGEHAND_BBOX_HEIGHT	17
#define FLANGEHAND_BBOX_WIDTH		16

#define FLANGEHAND_STATE_MOVE	0
#define FLANGEHAND_STATE_STOP	1

#define FLANGEHAND_ANI_LEFT	0
#define FLANGEHAND_ANI_RIGHT	1

class FlangeHand:public Bullets
{
	LPANIMATION ani;
	BossHand* FollowHand;
	FlangeHand* FollowFlange;
	int number_hand;
	int change_move;
	int type_move;
	float FollowBossX, FollowBossY;
public:
	FlangeHand(BossHand*,int ,int ,int);
	FlangeHand(FlangeHand*, int, int, int);
	virtual void SetState(int state);
	//bool GetAttack() { return IsAttack; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update_Right(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Update_Left(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Update_Right1(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Update_Right2(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Update_Left1(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Update_Left2(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);

	int GetFl_ChangeMove() { return change_move; }
	void SetFl_ChangeMove(int change) { change_move = change; }

	void Set_FollowBossX(float dx_boss) { FollowBossX = dx_boss; }
	void Set_FollowBossY(float dy_boss) { FollowBossY = dy_boss; }

	void Render();
	//bool TimeUp();
	bool Get_IsDead() { return isFinish; }
	void Set_IsDead(bool isfinish) { isFinish = isfinish; };
	void SubDamage(int damage) { EnermiesHealth += damage; };
};

