#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"

#include "Timer.h"
#include <cstdlib>
#include <ctime>


using namespace std;
#define PLAYER_HEALTH	8
#define ENERMIES_HEALTH	3
#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:

	int ID;
	int tag;
	float x;
	float y;

	float dx;
	float dy;

	float vx;
	float vy;

	int nx, ny;
	int alpha;
	int state;
	int direction;
	int damage;
	int EnermiesHealth;
	bool IsDead = false;
	bool isEnabled;
	float width, height;
	DWORD dt;
	LPANIMATION_SET animation_set;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	CGameObject();
	int GetAlpha() { return alpha; }
	void SetAlpha(int A) { alpha = A; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() = 0;
	//virtual void Render_Current_Frame() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual float GetDistance(LPGAMEOBJECT);

	virtual int Get_ID() = 0;
	virtual bool Get_IsDead() = 0;
	virtual void Set_IsDead(bool value) = 0;
	virtual void SubDamage(int damage) = 0;
	virtual float Get_width() = 0;
	virtual float Get_height() = 0;

	~CGameObject();
	bool IsColidingAABB(CGameObject* obj);
};

