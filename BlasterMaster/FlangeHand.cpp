#include "FlangeHand.h"

FlangeHand::FlangeHand(BossHand *target,int X,int Y,int type)
{
	this->x = X;
	this->y = Y;
	if (type >= 1)
	{
		ani = CAnimations::GetInstance()->Get(853);
	}
	else
	{
		ani = CAnimations::GetInstance()->Get(854);
	}
	this->FollowHand = target;
	number_hand = type;
	change_move = 1;
	this->SetState(FLANGEHAND_STATE_STOP);
}

FlangeHand::FlangeHand(FlangeHand* target, int X, int Y, int type)
{
	this->x = X;
	this->y = Y;
	type_move = 1;
	change_move = 1;
	if (type >= 1)
	{
		ani = CAnimations::GetInstance()->Get(853);
	}
	else
	{
		ani = CAnimations::GetInstance()->Get(854);
	}
	this->FollowFlange = target;
	this->SetState(FLANGEHAND_STATE_STOP);
}

void FlangeHand::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLANGEHAND_STATE_MOVE:
		if (number_hand == 1)
		{
			this->vx = FollowHand->vx;
			this->vy = FollowHand->vy;
		}
		else
		{
			this->vx = FollowFlange->vx;
			this->vy = FollowFlange->vy;
		}
		break;
	case FLANGEHAND_STATE_STOP:
		this->vx = this->vy = 0;
		break;
	}
}

void FlangeHand::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 0;
	bottom = y + 0;
}

void FlangeHand::Update_Right(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (number_hand == 1)
	{
		Update_Right1(dt, colliable_objects);
	}
	else
	{
		Update_Right2(dt, colliable_objects);
	}
	x += FollowBossX;
	y += FollowBossY;
	//else if()
}

void FlangeHand::Update_Left(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (number_hand == 1)
	{
		Update_Left1(dt, colliable_objects);
	}
	else
	{
		Update_Left2(dt, colliable_objects);
	}
	x += FollowBossX;
	y += FollowBossY;
}

void FlangeHand::Update_Right1(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	//DebugOut(L"%d\nChangMove:", this->change_move);
	if (this->FollowHand->Get_ChangeMove() == 0)
	{
		//change_move = 0;
		this->SetState(FLANGEHAND_STATE_MOVE);
	}
	else if (this->FollowHand->Get_ChangeMove() == 1)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->x > this->x + FLANGEHAND_BBOX_WIDTH)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 1;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 2)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->x < this->x)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 2;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 3)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->y < this->y)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 3;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 4)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->x < this->x)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 4;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 5)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->y > this->y + FLANGEHAND_BBOX_HEIGHT)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 5;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 6)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->y < this->y)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 6;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 7)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->x < this->x)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 7;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	CGameObject::Update(dt, colliable_objects);
	x += dx ;
	y += dy ;
}

void FlangeHand::Update_Right2(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	//DebugOut(L"%d \nChange_move1:", FollowFlange->GetFl_ChangeMove());
	if (this->FollowFlange->GetFl_ChangeMove() == 0)
	{
		this->SetState(FLANGEHAND_STATE_MOVE);
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 1)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->x > this->x + FLANGEHAND_BBOX_WIDTH)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 1;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 2)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->x < this->x)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 2;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 3)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->y < this->y)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 3;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 4)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->x < this->x)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 4;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 5)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->y > this->y + FLANGEHAND_BBOX_HEIGHT)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 5;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 6)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->y < this->y)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 6;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 7)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->x < this->x)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 7;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	CGameObject::Update(dt, colliable_objects);
	x += dx ;
	y += dy ;
}

void FlangeHand::Update_Left1(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (this->FollowHand->Get_ChangeMove() == 0)
	{
		//change_move = 0;
		this->SetState(FLANGEHAND_STATE_MOVE);
	}
	else if (this->FollowHand->Get_ChangeMove() == 1)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->x < this->x - FLANGEHAND_BBOX_WIDTH)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 1;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 2)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->x > this->x)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 2;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 3)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->y < this->y)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 3;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 4)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->x > this->x)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 4;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 5)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->y > this->y + FLANGEHAND_BBOX_HEIGHT)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 5;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 6)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->y < this->y)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 6;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowHand->Get_ChangeMove() == 7)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowHand->x > this->x)
		{
			this->FollowHand->Set_ChangeMove(0);
			this->change_move = 7;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	CGameObject::Update(dt, colliable_objects);
	x += dx;
	y += dy;
}

void FlangeHand::Update_Left2(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (this->FollowFlange->GetFl_ChangeMove() == 0)
	{
		this->SetState(FLANGEHAND_STATE_MOVE);
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 1)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->x < this->x - FLANGEHAND_BBOX_WIDTH)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 1;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 2)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->x > this->x)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 2;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 3)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->y < this->y)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 3;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 4)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->x > this->x)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 4;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 5)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->y > this->y + FLANGEHAND_BBOX_HEIGHT)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 5;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 6)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->y < this->y)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 6;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	else if (this->FollowFlange->GetFl_ChangeMove() == 7)
	{
		this->SetState(FLANGEHAND_STATE_STOP);
		if (FollowFlange->x > this->x)
		{
			this->FollowFlange->SetFl_ChangeMove(0);
			this->change_move = 7;
			this->SetState(FLANGEHAND_STATE_MOVE);
		}
	}
	CGameObject::Update(dt, colliable_objects);
	x += dx;
	y += dy;
}

void FlangeHand::Render()
{
	ani->Render(x,y);
	RenderBoundingBox();
}
