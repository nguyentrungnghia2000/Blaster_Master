#include "HumanBullets.h"
#include "Brick.h"
#include "Item.h"
#include "Bug.h"
#include "Robot.h"
#include "Bee.h"
#include "MayBug.h"
#include "Doom.h"
#include "Spider.h"
#include "Human.h"
#include "Car.h"
#include "Portal.h"

HumanBullets::HumanBullets()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_HUMAN_BULLET));
	x = 0;
	y = 0;
	alpha = 0;
	IsCollisionBrick = 0;
	IsDone = true;
	TimeDelay = 0;
	TimeDelayMax = HUMAN_BULLET_DELAY;
}

HumanBullets::~HumanBullets()
{
}

void HumanBullets::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y + BULLET_BBOX_HEIGHT;
}

void HumanBullets::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	if (IsTargetTop == false) {
		vx = BULLET_SPEED * BulletDirection;
	}
	else {
		vy = -BULLET_SPEED;
	}
	if (GetTickCount() - timer < EXPLOTION_TIME && flag == true) {
		isFinish = true;
		flag = false;
	}
#pragma region Xử lý va chạm
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* OnlyBrick = new vector<LPGAMEOBJECT>();
	vector<LPGAMEOBJECT>* OnlyItem = new vector<LPGAMEOBJECT>();
	vector<LPGAMEOBJECT>* OnlyEnermies_Brick = new vector<LPGAMEOBJECT>();
	coEvents.clear();
	OnlyBrick->clear();
	for (int i = 0; i < colliable_objects->size(); i++)
	{
		if (dynamic_cast<Item*>(colliable_objects->at(i))) {
			OnlyItem->push_back(colliable_objects->at(i));
		}
		else if (dynamic_cast<CCar*>(colliable_objects->at(i))) {
			OnlyItem->push_back(colliable_objects->at(i));
		}
		else if (dynamic_cast<CPortal*>(colliable_objects->at(i))) {
			OnlyEnermies_Brick->push_back(colliable_objects->at(i));
		}
		else {
			OnlyEnermies_Brick->push_back(colliable_objects->at(i));
		}
	}

	CalcPotentialCollisions(OnlyEnermies_Brick, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

#pragma region return the object type which have been hit by bullets
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj)) {
				if (e->nx != 0) {
					IsCollisionBrick = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
				if (e->ny != 0) {
					IsCollisionBrick = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
			}

			if (dynamic_cast<CPortal*>(e->obj)) {
				if (e->nx != 0) {
					IsCollisionBrick = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
				if (e->ny != 0) {
					IsCollisionBrick = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
			}

			else if (dynamic_cast<Robot*>(e->obj)) {
				Robot* rob = dynamic_cast<Robot*>(e->obj);
				if (e->nx != 0) {
					rob->SubDamage(-BULLETS_DAMAGE);

					IsCollisionEnermies = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
				if (e->ny != 0) {
					rob->SubDamage(-BULLETS_DAMAGE);
					IsCollisionEnermies = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
			}

			else if (dynamic_cast<Bug*>(e->obj)) {
				Bug* bug = dynamic_cast<Bug*>(e->obj);
				if (e->nx != 0) {
					bug->SubDamage(-BULLETS_DAMAGE);
					IsCollisionEnermies = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
				if (e->ny != 0) {
					bug->SubDamage(-BULLETS_DAMAGE);
					IsCollisionEnermies = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
			}

			else if (dynamic_cast<Doom*>(e->obj)) {
				Doom* doom = dynamic_cast<Doom*>(e->obj);
				if (e->nx != 0) {
					doom->SubDamage(-BULLETS_DAMAGE);
					IsCollisionEnermies = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
				if (e->ny != 0) {
					doom->SubDamage(-BULLETS_DAMAGE);
					IsCollisionEnermies = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
			}

			else if (dynamic_cast<MayBug*>(e->obj)) {
				MayBug* maybug = dynamic_cast<MayBug*>(e->obj);
				if (e->nx != 0) {
					maybug->SubDamage(-BULLETS_DAMAGE);
					IsCollisionEnermies = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
				if (e->ny != 0) {
					maybug->SubDamage(-BULLETS_DAMAGE);
					IsCollisionEnermies = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
			}

			else if (dynamic_cast<Spider*>(e->obj)) {
				Spider* spider = dynamic_cast<Spider*>(e->obj);
				if (e->nx != 0) {
					spider->SubDamage(-BULLETS_DAMAGE);
					IsCollisionEnermies = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
				if (e->ny != 0) {
					spider->SubDamage(-BULLETS_DAMAGE);
					IsCollisionEnermies = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
			}
		}
#pragma endregion
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void HumanBullets::Render()
{
	int ani;
	if (TimeDelay >= TimeDelayMax) {
		IsDone = true;
		TimeDelay = 0;
	}
	else {
		if (IsCollisionBrick == 0 && IsCollisionEnermies == 0) {
			ani = HUMAN_BULLET_ANI;
			animation_set->at(ani)->Render(x, y, alpha);

		}
		else {
			flag = true;
			ani = HUMAN_BULLET_EXPLOTION_ANI;
			animation_set->at(ani)->Render(x, y - DISTANCE_TO_BANG, alpha);
			if (animation_set->at(ani)->GetFrame() == 2) {
				IsDone = true;
				TimeDelay = 0;
			}
			timer = GetTickCount();
		}
	}
}
