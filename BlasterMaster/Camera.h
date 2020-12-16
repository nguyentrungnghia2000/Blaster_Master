#pragma once
#include "GameObject.h"
#include "Car.h"
#include "Human.h"
#include "BigHuman.h"

class Camera
{
	int mapWidth;
	int mapHeight;
public:

	Camera();
	~Camera();
	LPSPRITE spriteMap;
	int screen_width;
	float cam_x, cam_y;
	void Update(CGameObject* player, int spriteMapW, int spriteMapH, float& player_x, float& player_y);
	void GetCamPosition(float& x, float& y);
	bool IsInCam(LPGAMEOBJECT obj);
	void SetMapWidth(int width);
	void SetMapHeight(int height);
	int GetMapWidth();
};

