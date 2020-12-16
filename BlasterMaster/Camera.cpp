#include "Camera.h"
#include "Game.h"
#include "PlayScence.h"
#include "Sprites.h"

Camera::Camera()
{
	cam_x = cam_y = 0.0f;
	screen_width = CGame::GetInstance()->GetScreenWidth();
}

Camera::~Camera()
{
}

void Camera::Update(CGameObject* player, int spriteMapW, int spriteMapH, float& player_x, float& player_y)
{
	//float player_x, player_y;
	CGame* GameCamera = CGame::GetInstance();
	player->GetPosition(player_x, player_y);
	if (player->x + SCREEN_WIDTH / 2 >= spriteMapW)
		player_x = spriteMapW - SCREEN_WIDTH;
	else
	{
		if (player->x < SCREEN_WIDTH / 2)
			player_x = 0;
		else
			player_x -= SCREEN_WIDTH / 2;
	}

	if (player->y > spriteMapH - SCREEN_HEIGHT / 2)
	{
		if (spriteMapH < SCREEN_HEIGHT)
			player_y = 0;
		else
		{
			if (player->y + SCREEN_HEIGHT / 2 > spriteMapH)
				player_y = spriteMapH - SCREEN_HEIGHT;
			else
			{
				player_y -= SCREEN_HEIGHT / 2;
			}
		}
	}
	else if (player->y < spriteMapH - SCREEN_HEIGHT / 2)
	{
		if (spriteMapH < SCREEN_HEIGHT)
			player_y = 0;
		else
		{
			if (player->y < SCREEN_HEIGHT / 2)
				player_y = 0;
			else
				player_y -= SCREEN_HEIGHT / 2;
		}
	}
	GameCamera->SetCamPos(player_x, player_y);
}

void Camera::GetCamPosition(float& x, float& y)
{
	x = cam_x;
	y = cam_y;
}

bool Camera::IsInCam(LPGAMEOBJECT obj)
{
	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);

	/*return l > cam_x - TILE_WIDTH && r < cam_x + screen_width + TILE_WIDTH
		&& t > cam_y
		&& b - STATUS_BOARD_HEIGHT < cam_y + mapHeight + TILE_HEIGHT;*/
	return false;
}

void Camera::SetMapWidth(int width)
{
	mapWidth = width;
}

void Camera::SetMapHeight(int height)
{
	mapHeight = height;
}

int Camera::GetMapWidth()
{
	return mapWidth;
}
