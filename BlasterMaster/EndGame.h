#pragma once
#include "GameObject.h"
#include "Game.h"

#define TOWER_SPEED	0.1f

#define ANI_SET_ENDGAME	4
#define ANI_ENDGAME_1	0
#define ANI_ENDGAME_2	1
#define ANI_ENDGAME_3	2

#define DISTANCE_ANI	143
#define DISTANCE_ANI_2_3_X	125
#define DISTANCE_ANI_2_3_Y	117

#define ENDGAME_TIME	1000
#define TOWER_TIME	3000
#define WORLD_TIME	6000
#define CAM_TIME	8000


class EndGame : public CGameObject
{
	LPANIMATION_SET ani_set;
	DWORD timer, tower, world;
	float ani1_x, ani1_y, ani2_x, ani2_y, ani3_x, ani3_y;
	bool TowerMoveDown, ChangeWorld, change;
	float spriteW, spriteH;
	float cx = 0, cy = 0;
public:
	CGame* GameCamera = CGame::GetInstance();

	EndGame();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }


};

