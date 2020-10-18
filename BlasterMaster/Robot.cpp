#include "Robot.h"

void Robot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ROBOT_BBOX_WIDTH;
	bottom = y + ROBOT_BBOX_HEIGHT;
}

void Robot::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	//jump = x;
	vy += ROBOT_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* OnlyBrick = new vector<LPGAMEOBJECT>();
	//vector<LPGAMEOBJECT>* Player = new vector<LPGAMEOBJECT>();
	//Player->clear();
	OnlyBrick->clear();
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i)))
		{
			OnlyBrick->push_back(coObjects->at(i));
		}
	}
	//for (int i = 0; i < coObjects->size(); i++)
	//{
	//	if (dynamic_cast<CMario*>(coObjects->at(i)))
	//	{
	//		Player->push_back(coObjects->at(i));
	//	}
	//}

	CalcPotentialCollisions(OnlyBrick, coEvents);
	if (coEvents.size() == 0)
	{
		if (this->GetDistance(target) < DISTANCE * 2)
		{
			IsActive = true;
		}
		if (IsActive)
		{
			x += dx;
			y += dy;
			if (vx < 0)
			{
				if ((this->GetDistance(target) < DISTANCE) && (IsJump == false))
				{
					if (time->GetStartTime() == 0)
					{
						randomJump = 1 + rand() % 3;
						time->Start();
						this->SetState(ROBOT_STATE_HUNTING);
					}
					else
						this->SetState(ROBOT_STATE_HUNTING);
					if (time->IsTimeUp())
					{
						time->Reset();
						randomJump = NULL;
					}
				}
			}
			else
			{
				if ((this->GetDistance(target) < DISTANCE) && (IsJump == false))
				{
					if (time->GetStartTime() == 0)
					{
						randomJump = 1 + rand() % 4;
						time->Start();
						this->SetState(ROBOT_STATE_HUNTING);
					}
					else
						this->SetState(ROBOT_STATE_HUNTING);
					if (time->IsTimeUp())
					{
						time->Reset();
						randomJump = NULL;
						IsJump = false;
					}
				}
			}
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					IsJump = false;
				}
				if (nx != 0 && ny == 0)
				{
					vx = -vx;
				}
				if (IsJump == false)
				{
					if (vx < 0 && x < brick->x) {
						x = brick->x;
						vx = -vx;
					}
					if (vx > 0 && x + ROBOT_BBOX_WIDTH > (brick->x + BRICK_BBOX_WIDTH))
					{
						x = brick->x + BRICK_BBOX_WIDTH - ROBOT_BBOX_WIDTH;
						vx = -vx;
					}
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Robot::Render()
{
	if (IsActive)
	{
		int ani = ROBOT_ANI_WALKING_LEFT;
		if (vx > 0)
		{
			if (IsJump == true)
				ani = ROBOT_ANI_JUMP_RIGHT;
			else
				ani = ROBOT_ANI_WALKING_RIGHT;
		}
		else
		{
			if (IsJump == true)
				ani = ROBOT_ANI_JUMP_LEFT;
			else
				ani = ROBOT_ANI_WALKING_LEFT;
		}
		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();
	}
}

Robot::Robot(LPGAMEOBJECT Target)
{
	//srand(time(NULL));
	this->time = new Timer(ROBOT_HUNTING_TIME + ROBOT_SAVE_TIME);
	this->target = Target;
	SetState(ROBOT_STATE_UNACTIVE);
}

void Robot::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ROBOT_STATE_UNACTIVE:
		IsJump = false;
		IsHunting = false;
		vx = vy = 0;
	case ROBOT_STATE_WALKING:
		IsJump = false;
		vx = -ROBOT_WALKING_SPEED;
		break;
	case ROBOT_STATE_JUMP:
		vy = -ROBOT_JUMP_SPEED_Y;
		IsJump = true;
		if (target->x - this->x < 0)
			this->vx = -fabs(this->vx);
		else
			this->vx = fabs(this->vx);
		break;
	case ROBOT_STATE_HUNTING:
		IsHunting = true;
		if (GetTickCount() - time->GetStartTime() <= ROBOT_HUNTING_TIME)
		{
			if (randomJump != 0 && randomJump < 5)
			{
				this->SetState(ROBOT_STATE_JUMP);
				randomJump += 1;
			}
			//DebugOut(L"[INFO] KeyDown: %d\n",random);
			//random = NULL;
			//IsJump = false;
		}
		else
			IsJump = false;
		break;
	}
}
