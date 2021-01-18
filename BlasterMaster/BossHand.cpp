#include "BossHand.h"

void BossHand::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOSSHAND_BBOX_WIDTH;
	bottom = y + BOSSHAND_BBOX_HEIGHT;
}

void BossHand::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += FollowBossX;
	y += FollowBossY;
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (time_move_type->GetStartTime() == 0)
	{
		time_move_type->Start();
	}
	else if (time_move_type->IsTimeUp())
	{
		if (type_move == 7)
		{
			type_move = 1;
			change_move = type_move;
			this->SetState(BOSSHAND_STATE_MOVE);
		}
		else
		{
			type_move ++;
			change_move = type_move;
			this->SetState(BOSSHAND_STATE_MOVE);
		}
		time_move_type->Reset();
	}
}

void BossHand::Render()
{
	//int ani = BOSSHAND_ANI_RIGHT;
	//animation_set->at(ani)->Render(x, y);
	ani->Render(x, y);
	//DebugOut(L"%d \ntoa do: ", this->x);
	RenderBoundingBox();
}

BossHand::BossHand(int X, int Y, int type)
{
	this->x = X;
	this->y = Y;
	type_move = 1;
	change_move = 1;
	type_hand = type;
	if (type_hand == 1)
	{
		ani = CAnimations::GetInstance()->Get(851);
	}
	else
	{
		ani = CAnimations::GetInstance()->Get(852);
	}
	this->SetState(BOSSHAND_STATE_MOVE);
	time_move_type = new Timer(BOSSHAND_TIME);
}

void BossHand::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSSHAND_STATE_MOVE:
		vx = ReturnX();
		vy = ReturnY();
		break;
	}
}

float BossHand::ReturnX()
{
	switch (type_move)
	{
	case 1:
		if (type_hand == 1)
		{
			vx = (float)BOSSHAND_SPEED;
		}
		else
		{
			vx = -(float)BOSSHAND_SPEED;
		}
		break;
	case 2:
		if (type_hand == 1)
		{
			vx = (float)-2 / 3 * BOSSHAND_SPEED;
		}
		else
		{
			vx = (float)2 / 3 * BOSSHAND_SPEED;
		}
		break;
	case 3:
		vx = 0;
		break;
	case 4:
		if (type_hand == 1)
		{
			vx = (float)-1 / 3 * BOSSHAND_SPEED;
		}
		else
		{
			vx = (float)1 / 3 * BOSSHAND_SPEED;
		}
		break;
	case 5:
		vx = 0;
		break;
	case 6:
		if (type_hand == 1)
		{
			vx = BOSSHAND_SPEED;
		}
		else
		{
			vx = -BOSSHAND_SPEED;
		}
		break;
	case 7:
		if (type_hand == 1)
		{
			vx = -BOSSHAND_SPEED;
		}
		else
		{
			vx = BOSSHAND_SPEED;
		}
		break;
	}
	return vx;
}

float BossHand::ReturnY()
{
	switch (type_move)
	{
	case 1:
		vy = BOSSHAND_SPEED;
		break;
	case 2:
		vy = 0;
		break;
	case 3:
		vy = -3 / 2 * BOSSHAND_SPEED;
		break;
	case 4:
		vy = 0;
		break;
	case 5:
		vy = 3 / 2 * BOSSHAND_SPEED;
		break;
	case 6:
		vy = -BOSSHAND_SPEED;
		break;
	case 7:
		vy = 0;
		break;
	}
	return vy;
}
