#include "Item.h"
#include "Utils.h"
#include <random>

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ITEM_BBOX_WIDTH;
	bottom = y + ITEM_BBOX_HEIGHT;
}

void Item::Render()
{
	if (Time_Active->IsTimeUp() == false)
	{
		int ani;
		if (id <= 5 && id!=0)
		{
			switch (id)
			{
			case 1:
				if (GetTickCount() - Time_Active->GetStartTime() < 8000)
					ani = P_ITEM_ANI_ACTIVE;
				else
					ani = P_ITEM_ANI_TIMEUP;
				break;
			case 2:
				if (GetTickCount() - Time_Active->GetStartTime() < 8000)
					ani = H_ITEM_ANI_ACTIVE;
				else
					ani = H_ITEM_ANI_TIMEUP;
				break;
			case 3:
				if (GetTickCount() - Time_Active->GetStartTime() < 8000)
					ani = LIGHTNING_ANI_ACTIVE;
				else
					ani = LIGHTNING_ANI_TIMEUP;
				break;
			case 4:
				if (GetTickCount() - Time_Active->GetStartTime() < 8000)
					ani = ROCKET_ANI_ACTIVE;
				else
					ani = ROCKET_ANI_TIMEUP;
				break;
			case 5:
				if (GetTickCount() - Time_Active->GetStartTime() < 8000)
					ani = HROCKET_ANI_ACTIVE;
				else
					ani = HROCKET_ANI_TIMEUP;
				break;
			}
			animation_set->at(ani)->Render(x, y);
		}
		//RenderBoundingBox();
	}
	else
	{
		this->IsDead = true;
	}
}

Item::Item(float x, float y)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ITEM_STATE_ACTIVE));
	this->IsDead = false;
	random_device rm;
	mt19937 t(rm());
	uniform_int_distribution<int>	randi(1,15);
	int random = randi(t);
	if (random > 5&&random<8)
	{
		id = 1;
	}
	else if (random >=2&&random<=5)
	{
		id = random;
	}
	else if (random == 1||random>=8)
	{
		id = 6;
	}
	this->Time_Active = new Timer(ITEM_TIME_ACTIVE);
	SetState(ITEM_STATE_ACTIVE);
	this->x = x;
	this->y = y;
}

void Item::SetState(int state)
{
	switch (state)
	{
	case ITEM_STATE_ACTIVE:
		Time_Active->Start();
		break;
	case ITEM_STATE_TIMEUP:
		break;
	}
}

