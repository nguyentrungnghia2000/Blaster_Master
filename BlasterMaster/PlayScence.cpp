﻿#include <iostream>
#include <fstream>

#include "Intro.h"
#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Portal1.h"
#include "Brick.h"
#include "Car.h"
#include "Bug.h"
#include "Robot.h"
#include "Bee.h"
#include "MayBug.h"
#include "Doom.h"
#include "Spider.h"
#include "Item.h"
#include "Human.h"
#include "HumanBullets.h"
#include "Arrows.h"
#include "Ladder.h"
#include "Lava.h"
#include "Camera.h"
#include "EndGame.h"
#include "Skeleton.h"
#include "Gunner.h"
#include "MayBug2.h"
#include "Mine.h"
#include "Eyeball.h"
#include "Tele.h"
#include "Boss.h"
#include "BossHand.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_SCENE	7
#define SCENE_SECTION_SWITCHSCENE	8
#define SCENE_SECTION_MAP	9

#define OBJECT_TYPE_HUMAN	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_BIGHUMAN	2
#define OBJECT_TYPE_INTRO	3
#define OBJECT_TYPE_ENDGAME	4
#define OBJECT_TYPE_CAR		5
#define OBJECT_TYPE_BUG		7
#define OBJECT_TYPE_ROBOT	8
#define OBJECT_TYPE_BEE		9
#define OBJECT_TYPE_MAYBUG	10
#define OBJECT_TYPE_DOOM	11
#define OBJECT_TYPE_SPIDER	12
#define OBJECT_TYPE_SKELETON 13
#define OBJECT_TYPE_GUNNER	14
#define OBJECT_TYPE_MAYBUG2	15
#define OBJECT_TYPE_MINE	16
#define OBJECT_TYPE_EYEBALL	17
#define OBJECT_TYPE_TELE	18
#define OBJECT_TYPE_BOSS	60
#define OBJECT_TYPE_BOSSHAND 61
#define OBJECT_TYPE_LADDER	23
#define OBJECT_TYPE_LAVA	24
#define OBJECT_TYPE_ARROWS	25
#define OBJECT_TYPE_ITEM	100
#define OBJECT_TYPE_PORTAL	50
#define OBJECT_TYPE_PORTAL1	51

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	if (ID == 123456) {
		mapW = r;
		mapH = b;
	}
	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; 

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return;

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	float w, h;
	int IsState;
	int IsBrick;
	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;
	CGameObject* players = NULL;

	switch (object_type){
	case OBJECT_TYPE_INTRO:
	{
		obj = new Intro();
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ENDGAME:
	{
		obj = new EndGame();
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_CAR:
	{
		if (player != NULL) {
			DebugOut(L"[ERROR] CAR object was created before!\n");
			return;
		}
		obj = new CCar(x, y);
		player = (CCar*)obj;

		DebugOut(L"[INFO] Player CAR created!\n");
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsPlayers.push_back(obj);
		break;
	}
	case OBJECT_TYPE_HUMAN:
	{
		if (player2 != NULL) {
			DebugOut(L"[ERROR] HUMAN object was created before!\n");
			return;
		}
		obj = new Human(x, y);
		player2 = (Human*)obj;
		DebugOut(L"[INFO] Player HUMAN created!\n");
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsPlayers.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BIGHUMAN:
	{
		if (player3 != NULL) {
			DebugOut(L"[ERROR] BIGHUMAN object was created before!\n");
			return;
		}
		obj = new BigHuman(x, y);
		player3 = (BigHuman*)obj;
		DebugOut(L"[INFO] Player BIGHUMAN created!\n");
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsPlayers.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BOSS:
	{
		int x = atof(tokens[1].c_str());
		int y = atof(tokens[2].c_str());
		if (BigBoss != NULL)
		{
			DebugOut(L"[ERROR] BOSS object was created before!\n");
		}
		//obj = new Boss(player3, x, y);
		obj = new Boss(player3,x,y);
		BigBoss = (Boss*)obj;
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BRICK:
	{
		w = atof(tokens[4].c_str());
		h = atof(tokens[5].c_str());
		obj = new CBrick(w, h);
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_LADDER:
	{
		w = atof(tokens[4].c_str());
		h = atof(tokens[5].c_str());
		obj = new Ladder(w, h);
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_LAVA:
	{
		w = atof(tokens[4].c_str());
		h = atof(tokens[5].c_str());
		obj = new Lava(w, h);
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ARROWS:
	{
		w = atof(tokens[4].c_str());
		h = atof(tokens[5].c_str());
		obj = new Arrows(w, h);
		break;
	}
	case OBJECT_TYPE_BUG: 
	{
		obj = new Bug(player);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] BUG created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ROBOT: 
	{
		obj = new Robot(player);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] ROBOT created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BEE: 
	{
		obj = new Bee(player);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] BEE created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_MAYBUG: 
	{
		obj = new MayBug(player);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] MAYBUG created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_DOOM:
	{
		IsBrick = atof(tokens[5].c_str());
		IsState = atof(tokens[4].c_str());
		obj = new Doom(player, IsState, IsBrick);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] DOOM created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_SPIDER: 
	{
		obj = new Spider(player);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] SPIDER created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_SKELETON:
	{
		obj = new Skeleton(player);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] SPIDER created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_GUNNER:
	{
		obj = new Gunner();
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] SPIDER created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_MAYBUG2:
	{
		int x = atof(tokens[1].c_str());
		int y = atof(tokens[2].c_str());
		obj = new MayBug2(player,x,y);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] SPIDER created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_MINE:
	{
		obj = new Mine();
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] SPIDER created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_EYEBALL:
	{
		int x = atof(tokens[1].c_str());
		int y = atof(tokens[2].c_str());
		obj = new Eyeball(player3, x, y);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] SPIDER created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_TELE:
	{
		int x = atof(tokens[4].c_str());
		int y = atof(tokens[5].c_str());
		obj = new Tele(player3,x,y);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] SPIDER created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BOSSHAND:
	{
		int type = atof(tokens[4].c_str());
		int x = atof(tokens[1].c_str());
		int y = atof(tokens[2].c_str());
		obj = new BossHand(x,y,type);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] SPIDER created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsEnermies.push_back(obj);
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] PORTAL created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_PORTAL1:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal1(x, y, r, b, scene_id);
		obj->SetPosition(x, y);
		DebugOut(L"[INFO] PORTAL1 created!\n");
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		lsObjects.push_back(obj);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	
}

void CPlayScene::_ParseSection_SCENE(string line)
{
	vector<string> tokens = split(line);
}

void CPlayScene::_ParseSection_SWITCHSCENE(string line)
{
	vector<string> tokens = split(line);
	int id_scene = atoi(tokens[0].c_str());
	CGame::GetInstance()->SwitchScene(id_scene);
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);
	int id_map = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int id_texture = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(id_texture);
	//spriteMap = CSprites::GetInstance()->Get(id_map);
	spriteMap = new CSprite(id_map, l, t, r, b, tex);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[SCENE]") {
			section = SCENE_SECTION_SCENE; continue;
		}
		if (line == "[SWITCHSCENE]") {
			section = SCENE_SECTION_SWITCHSCENE; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_SWITCHSCENE: _ParseSection_SWITCHSCENE(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	LoadObjects();
	
	//Load map
	spriteMap = CSprites::GetInstance()->Get(123456);
	
}

void CPlayScene::LoadObjects()
{
	if (playerHUD == NULL) {
		playerHUD = new HUD(PLAYER_HEALTH, PLAYER_HEALTH);
	}
	if (grid == NULL) {
		grid = new Grid(mapW, mapH);
	}
	else
		grid->Reset(mapW, mapH);

	grid->PushObjectIntoGrid(lsEnermies);
}

void CPlayScene::Update(DWORD dt)
{
	if (GetTickCount() - TimeToChangeScene < TIMETOCHANGESCENE) {
		BossIsDead = true;
	}
#pragma region CAMERA
	float cx, cy;
	if (player != NULL) {
		if (player->isActive == true) {
			player->GetPosition(cx, cy);
			if (player->x + SCREEN_WIDTH / 2 >= spriteMap->GetWidth())
				cx = spriteMap->GetWidth() - SCREEN_WIDTH;
			else
			{
				if (player->x < SCREEN_WIDTH / 2)
					cx = 0;
				else
					cx -= SCREEN_WIDTH / 2;
			}

			if (player->y > spriteMap->GetHeight() - SCREEN_HEIGHT / 2)
			{
				if (spriteMap->GetHeight() < SCREEN_HEIGHT)
					cy = 0;
				else
				{
					if (player->y + SCREEN_HEIGHT / 2 > spriteMap->GetHeight())
						cy = spriteMap->GetHeight() - SCREEN_HEIGHT;
					else
					{
						cy -= SCREEN_HEIGHT / 2;
					}
				}
			}
			else if (player->y < spriteMap->GetHeight() - SCREEN_HEIGHT / 2)
			{
				if (spriteMap->GetHeight() < SCREEN_HEIGHT)
					cy = 0;
				else
				{
					if (player->y < SCREEN_HEIGHT / 2)
						cy = 0;
					else
						cy -= SCREEN_HEIGHT / 2;
				}
			}
			GameCamera->SetCamPos(cx, cy);
		}
	}
	if (player2 != NULL) {
		if (player2->isActive == true) {
			player2->GetPosition(cx, cy);
			if (player2->x + SCREEN_WIDTH / 2 >= spriteMap->GetWidth())
				cx = spriteMap->GetWidth() - SCREEN_WIDTH;
			else
			{
				if (player2->x < SCREEN_WIDTH / 2)
					cx = 0;
				else
					cx -= SCREEN_WIDTH / 2;
			}

			if (player2->y > spriteMap->GetHeight() - SCREEN_HEIGHT / 2)
			{
				if (spriteMap->GetHeight() < SCREEN_HEIGHT)
					cy = 0;
				else
				{
					if (player2->y + SCREEN_HEIGHT / 2 > spriteMap->GetHeight())
						cy = spriteMap->GetHeight() - SCREEN_HEIGHT;
					else
					{
						cy -= SCREEN_HEIGHT / 2;
					}
				}
			}
			else if (player2->y < spriteMap->GetHeight() - SCREEN_HEIGHT / 2)
			{
				if (spriteMap->GetHeight() < SCREEN_HEIGHT)
					cy = 0;
				else
				{
					if (player2->y < SCREEN_HEIGHT / 2)
						cy = 0;
					else
						cy -= SCREEN_HEIGHT / 2;
				}
			}
			GameCamera->SetCamPos(cx, cy);
		}
	}
	if (player3 != NULL) {
		if (player3->isActive == true) {
			player3->GetPosition(cx, cy);
			if (player3->x + SCREEN_WIDTH / 2 >= spriteMap->GetWidth())
				cx = spriteMap->GetWidth() - SCREEN_WIDTH;
			else
			{
				if (player3->x < SCREEN_WIDTH / 2)
					cx = 0;
				else
					cx -= SCREEN_WIDTH / 2;
			}

			if (player3->y > spriteMap->GetHeight() - SCREEN_HEIGHT / 2)
			{
				if (spriteMap->GetHeight() < SCREEN_HEIGHT)
					cy = 0;
				else {
					if (player3->y + SCREEN_HEIGHT / 2 > spriteMap->GetHeight())
						cy = spriteMap->GetHeight() - SCREEN_HEIGHT;
					else {
						cy -= SCREEN_HEIGHT / 2;
					}
				}
			}
			else if (player3->y < spriteMap->GetHeight() - SCREEN_HEIGHT / 2) {
				if (spriteMap->GetHeight() < SCREEN_HEIGHT)
					cy = 0;
				else {
					if (player3->y < SCREEN_HEIGHT / 2)
						cy = 0;
					else
						cy -= SCREEN_HEIGHT / 2;
				}
			}
			GameCamera->SetCamPos(cx, cy);
		}
		
	}
#pragma endregion

#pragma region objects and bullets

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < lsObjects.size(); i++) {
		coObjects.push_back(lsObjects[i]);
	}
	for (size_t i = 0; i < lsItems.size(); i++) {
		coObjects.push_back(lsItems[i]);
	}
	for (size_t i = 0; i < coEnermies.size(); i++) {
		if(coEnermies[i]->IsDead==false)
			coObjects.push_back(coEnermies[i]);
		else
		{
			coEnermies.erase(coEnermies.begin() + i);
		}
	}
	DebugOut(L"\nCount:%d", count);
	for (size_t i = 0; i < lsPlayers.size(); i++) {
		coObjects.push_back(lsPlayers[i]);
	}
	if (player != NULL) {
		if (player->isAttack)
		{
			MainBullets* bullet = new MainBullets();
			player->Get_CarDirection(bullet->BulletDirection);
			player->Get_CarFlipUp(bullet->IsTargetTop);
			if (bullet->IsTargetTop == true) {
				bullet->SetPosition(player->x + DISTANCE_TO_CAR_FLIP_UP_WIDTH, player->y + DISTANCE_TO_CAR_FLIP_UP_HEIGHT);
			}
			else {
				if (bullet->BulletDirection > 0)
					bullet->SetPosition(player->x + DISTANCE_TO_CAR_WIDTH_RIGHT, player->y + DISTANCE_TO_CAR_HEIGTH);
				else
					bullet->SetPosition(player->x + DISTANCE_TO_CAR_WIDTH_LEFT, player->y + DISTANCE_TO_CAR_HEIGTH);
			}
			lsBullets.push_back(bullet);
			player->isAttack = false;
		}
	}
	if (player2 != NULL) {
		if (player2->isAttack)
		{
			HumanBullets* bullet = new HumanBullets();
			player2->Get_HumanDirection(bullet->BulletDirection);

			if (bullet->BulletDirection > 0)
				bullet->SetPosition(player2->x + DISTANCE_TO_HUMAN_WIDTH_RIGHT, player2->y + DISTANCE_TO_HUMAN_HEIGTH);
			else
				bullet->SetPosition(player2->x + DISTANCE_TO_HUMAN_WIDTH_LEFT, player2->y + DISTANCE_TO_HUMAN_HEIGTH);
			lsBullets.push_back(bullet);
			player2->isAttack = false;
		}
	}
	if (player3 != NULL) {
		if (player3->isAttack)
		{
			MainBullets* bullet = new MainBullets();
			player3->Get_DirectionY(bullet->IsTargetTop);
			if (bullet->IsTargetTop == true) {
				player3->Get_BigHumanDirectionY(bullet->BulletDirection);
				if (bullet->BulletDirection > 0)
					bullet->SetPosition(player3->x + 10, player3->y);
				else
					bullet->SetPosition(player3->x + DISTANCE_TO_HUMAN_WIDTH_LEFT, player3->y + 24);
			}
			else {
				player3->Get_BigHumanDirection(bullet->BulletDirection);
				if (bullet->BulletDirection > 0)
					bullet->SetPosition(player3->x + DISTANCE_TO_BHUMAN_WIDTH_RIGHT, player3->y + DISTANCE_TO_BHUMAN_HEIGTH);
				else
					bullet->SetPosition(player3->x + DISTANCE_TO_BHUMAN_WIDTH_LEFT, player3->y + DISTANCE_TO_BHUMAN_HEIGTH);
			}
			lsBullets.push_back(bullet);
			player3->isAttack = false;
		}
	}
#pragma endregion

#pragma region update objects
	for (size_t i = 0; i < lsObjects.size(); i++) {
		lsObjects[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < lsPlayers.size(); i++) {
		lsPlayers[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < lsPlayers.size(); i++) {
		if (lsPlayers[i]->Get_IsDead() == true) {
			lsPlayers.erase(lsPlayers.begin() + i);
		}
	}
	for (int i = 0; i < coEnermies.size(); i++) {
		coEnermies[i]->Update(dt, &coObjects);

		if (dynamic_cast<Boss*>(coEnermies[i])) {
			Boss* bos = dynamic_cast<Boss*>(coEnermies[i]);
			if (bos->Get_IsDead() == true) {
				TimeToChangeScene = GetTickCount();
			}
		}
		else {
			if (coEnermies[i]->Get_IsDead() == true) {
				float ix = coEnermies[i]->x;
				float iy = coEnermies[i]->y;
				coEnermies.erase(coEnermies.begin() + i);
				Item* item = new Item(ix, iy);
				lsItems.push_back(item);
				//DebugOut(L"\nsize item:%d", lsItems.size());
				//break;
			}
		}
	}
	for (int i = 0; i < lsBullets.size(); i++) {
		lsBullets[i]->Update(dt, &coObjects);

		if (lsBullets[i]->Get_IsFinish() == true)
			lsBullets.erase(lsBullets.begin() + i);
	}
	for (int i = 0; i < lsItems.size(); i++) {
		if (lsItems[i]->Get_IsDead() == true)
			lsItems.erase(lsItems.begin() + i);
	}
	//DebugOut(L"\nsize item:%d", lsItems.size());
	if (player->IsDead == true) {
		//player->y = (CAR_DIE_BBOX_HEIGTH - CAR_BBOX_HEIGHT);
	}
	if (playerHUD != NULL) {
		if (player != NULL) {
			if (player->isActive == true) {
				player->Get_health(healthunit);
				player->Get_power(powerunit);
				playerHUD->Update(cx, cy, healthunit, powerunit);
			}
		}
		if (player2 != NULL) {
			if (player2->isActive == true) {
				player2->Get_health(healthunit);
				player2->Get_power(powerunit);
				playerHUD->Update(cx, cy, healthunit, powerunit);
			}
		}
		if (player3 != NULL) {
			if (player3->isActive == true) {
				player3->Get_health(healthunit);
				player3->Get_power(powerunit);
				playerHUD->Update(cx, cy, healthunit, powerunit);
			}
		}
	}
#pragma endregion
	ChangeScene();
	GetObjectFromGrid(GameCamera->GetCamx(), GameCamera->GetCamy());

	if (player == NULL) return;
	if (player2 == NULL) return;
}

void CPlayScene::Render()
{
	if (spriteMap != NULL)
		spriteMap->Draw(0, 0);
	for (int i = 0; i < lsObjects.size(); i++)
		lsObjects[i]->Render();
	for (int i = 0; i < lsPlayers.size(); i++)
		lsPlayers[i]->Render();
	for (int i = 0; i < coEnermies.size(); i++)
		coEnermies[i]->Render();
	for (int i = 0; i < lsItems.size(); i++)
		lsItems[i]->Render();

	for (int i = 0; i < lsBullets.size(); i++) {
		lsBullets[i]->Render();
	}
	if (player != NULL) {
		if (player->isActive == true) {
			playerHUD->Render(player);
		}
	}
	if (player2 != NULL) {
		if (player2->isActive == true) {
			playerHUD->Render(player2);
		}
	}
	if (player3 != NULL) {
		if (player3->isActive == true) {
			playerHUD->Render(player3);
		}
	}
}

void CPlayScene::Unload()
{
	for (int i = 0; i < lsObjects.size(); i++)
		delete lsObjects[i];
	lsObjects.clear();
	for (int i = 0; i < lsPlayers.size(); i++)
		delete lsPlayers[i];
	lsPlayers.clear();
	for (int i = 0; i < lsEnermies.size(); i++)
		delete lsEnermies[i];
	lsEnermies.clear();
	/*for (int i = 0; i < coEnermies.size(); i++)
		delete coEnermies[i];*/
	coEnermies.clear();
	/*for (int i = 0; i < lsEnermiesGrid.size(); i++)
		delete lsEnermiesGrid[i];*/
	lsEnermiesGrid.clear();
	
	lsBullets.clear();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::GetObjectFromGrid(float x, float y)
{
	lsEnermiesGrid.clear();
	coEnermies.clear();
	grid->GetListObject(x, y, lsEnermiesGrid);
	for (UINT i = 0; i < lsEnermiesGrid.size(); i++) {
		coEnermies.push_back(lsEnermiesGrid[i]);
	}
}

void CPlayScene::ChangeScene()
{
	if (player != NULL) {
		if (player->IsChangeScene == true) {
			CGame::GetInstance()->SwitchScene(CGame::GetInstance()->GetIDCurrentScene() + 1);
			player->IsChangeScene = false;
		}
	}
	if (player2 != NULL) {
		if (player2->IsChangeScene == true) {
			CGame::GetInstance()->SwitchScene(CGame::GetInstance()->GetIDCurrentScene() + 1);
			player2->IsChangeScene = false;
		}
	}
	if (BossIsDead == true) {
		CGame::GetInstance()->SwitchScene(CGame::GetInstance()->GetIDCurrentScene() + 1);
	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{	
	CGame* game = CGame::GetInstance();
	CCar* car = ((CPlayScene*)scence)->GetPlayer();
	Human* human = ((CPlayScene*)scence)->GetPlayer1();
	BigHuman* bhuman = ((CPlayScene*)scence)->GetPlayer2();
	vector<LPGAMEOBJECT> listObjects = ((CPlayScene*)scence)->ReturnObject();
	float Carx = 0, Cary = 0;
	int direction;
	bool IsTargetTop;
	
	switch (KeyCode)
	{
	case DIK_O:
		if (car != NULL && human != NULL && bhuman != NULL) {
			if (bhuman->isActive == false) {
				car->isOverWorld = true;
				car->isActive = false;
				human->isOverWorld = true;
				human->isActive = false;
				bhuman->isActive = true;
			}
			else {
				car->isOverWorld = false;
				car->isActive = true;
				human->isOverWorld = false;
				human->isActive = false;
				bhuman->isActive = false;
			}
		}
	case DIK_T:
		if (car != NULL && human != NULL) {
			if (car->isActive == true && human->isActive == false) {
				car->isActive = false;
				human->isActive = true;
				car->Get_CarDirection(human->nx);
				if (human->nx > 0)
					human->SetPosition(car->x + HUMAN_APPEAR_DISTANCE_WIDTH_RIGHT, car->y + HUMAN_APPEAR_DISTANCE_HEIGHT);
				else
					human->SetPosition(car->x + HUMAN_APPEAR_DISTANCE_WIDTH_LEFT, car->y + HUMAN_APPEAR_DISTANCE_HEIGHT);
			}
			else if (car->isActive == false && human->isActive == true) {
				if ((car->x < (human->x + HUMAN_BBOX_WIDTH) && car->y < human->y && human->isCollisionWithCar == true)) {
					car->isActive = true;
					human->isActive = false;
				}
			}
		}
		break;
	case DIK_X:
		if (car != NULL) {
			if (car->isActive == true) {
				car->SetState(CAR_STATE_JUMP);
				car->PressJump = true;
			}
		}
		if (human != NULL) {
			if (human->isActive == true && human->isLying == false) {
				human->SetState(HUMAN_STATE_JUMP);
				human->PressJump = true;
			}
		}
		break;
	case DIK_C:
		if (car != NULL) {
			if (car->isActive == true)
				car->isAttack = true;
		}
		if (human != NULL) {
			if (human->isActive == true)
				human->isAttack = true;
		}
		if (bhuman != NULL) {
			if (bhuman->isActive == true)
				bhuman->isAttack = true;
		}
		break;
	case DIK_R:
		if (car != NULL && human != NULL) {
			car->Reset();
			human->Reset();
		}
		break;
	case DIK_Q:
		if (CGame::GetInstance()->GetIDCurrentScene() == 10)
			CGame::GetInstance()->SwitchScene(1);
		else
			CGame::GetInstance()->SwitchScene(1 + CGame::GetInstance()->GetIDCurrentScene());
		break;
	case DIK_L:
		for (int i = 0; i < listObjects.size(); i++)
		{
			if (listObjects[i]->GetAlpha() == 0)
				listObjects[i]->SetAlpha(155);
			else
				listObjects[i]->SetAlpha(0);
		}
		break;
	case DIK_DOWN:
		if (bhuman != NULL) {
			if (bhuman->isActive == true) {
				bhuman->directionY = true;
				bhuman->SetState(BIGHUMAN_STATE_MOVE_DOWN);
			}
		}
		if (human != NULL) {
			if (human->isActive == true) {
				if (human->isLadder == true) {
					human->SetState(HUMAN_STATE_MOVE_DOWN_LADDER);
					human->isMovingonLadder = true;
				}
				else
					human->PressDown = true;
			}
		}
		break;
	case DIK_RIGHT:
		if (bhuman != NULL) {
			if (bhuman->isActive == true) {
				bhuman->directionY = false;
				if (bhuman->isLadder == true) {
					bhuman->isMovingonLadder = true;
					bhuman->SetState(BIGHUMAN_STATE_MOVE_RIGHT);
				}
				else
					bhuman->SetState(BIGHUMAN_STATE_MOVE_RIGHT);
			}
		}
		break;
	case DIK_UP:
		if (human != NULL) {
			if (human->isActive == true) {
				if (human->isLadder == true) {
					human->SetState(HUMAN_STATE_MOVE_UP_LADDER);
					human->isMovingonLadder = true;
				}
				else {
					if (human->PressDown == true) {
						human->PressDown = false;
						human->isLying = false;
						human->SetPosition(human->x, human->y - (HUMAN_BBOX_HEIGHT - HUMAN_LIE_BBOX_HEIGHT));
					}
				}
			}
		}
		if (bhuman != NULL) {
			if (bhuman->isActive == true) {
				bhuman->directionY = true;
				bhuman->SetState(BIGHUMAN_STATE_MOVE_UP);
			}
		}
		break;
	case DIK_LEFT:
		if (bhuman != NULL) {
			if (bhuman->isActive == true) {
				bhuman->directionY = false;
				if (bhuman->isLadder == true) {
					bhuman->isMovingonLadder = true;
					bhuman->SetState(BIGHUMAN_STATE_MOVE_LEFT);
				}
				else
					bhuman->SetState(BIGHUMAN_STATE_MOVE_LEFT);
			}
		}
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode) {
	CCar* car = ((CPlayScene*)scence)->GetPlayer();
	Human* human = ((CPlayScene*)scence)->GetPlayer1();
	BigHuman* bhuman = ((CPlayScene*)scence)->GetPlayer2();
	switch (KeyCode)
	{
	case DIK_UP:
		if (car != NULL) {
			if (car->isActive == true) {
				car->SetPosition(car->x, car->y + (CAR_UP_BBOX_HEIGHT - CAR_BBOX_HEIGHT));
				car->PressKeyUp = false;
				car->FlippingUp = false;
				car->SetState(CAR_STATE_IDLE);
			}
		}
		if (bhuman != NULL) {
			if (bhuman->isActive == true) {
				bhuman->SetState(BIGHUMAN_STATE_IDLE_Y);
			}
		}
		if (human != NULL) {
			if (human->isActive == true) {
				if (human->isLying == true) {
					human->SetPosition(human->x, human->y + (HUMAN_BBOX_HEIGHT - HUMAN_LIE_BBOX_HEIGHT));
				}
			}
		}
		break;
	case DIK_DOWN:
		if (bhuman != NULL) {
			if (bhuman->isActive == true) {
				bhuman->SetState(BIGHUMAN_STATE_IDLE_Y);
			}
		}
		break;
	case DIK_LEFT:
	case DIK_RIGHT:
		if (car != NULL) {
			if (car->isActive == true)
				car->SetState(CAR_STATE_IDLE);
		}
		if (human != NULL) {
			if (human->isActive == true) {
				if (human->isLying == false)
					human->SetState(HUMAN_STATE_IDLE);
				else
					human->SetState(HUMAN_STATE_LIE);
			}
		}
		if (bhuman != NULL) {
			if (bhuman->isActive == true) {
				bhuman->SetState(BIGHUMAN_STATE_IDLE_X);
			}
		}
		break;
	case DIK_X:
		if (car != NULL) {
			if (car->isActive == true) {
				car->PressJump = false;
			}
		}
		if (human != NULL) {
			if (human->isActive == true) {
				human->PressJump = false;
			}
		}
		break;
	case DIK_C:
		if (car != NULL) {
			if (car->isActive == true)
				car->isAttack = false;
		}
		if (human != NULL) {
			if (human->isActive == true)
				human->isAttack = false;
		}
		break;
	}
}
void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CCar* car = ((CPlayScene*)scence)->GetPlayer();
	Human* human = ((CPlayScene*)scence)->GetPlayer1();
	BigHuman* bhuman = ((CPlayScene*)scence)->GetPlayer2();

	int KeyCode;
	if (car != NULL) {
		if (car->isActive == true) {
			if (car->GetState() == CAR_STATE_DIE) return;

			if (game->IsKeyDown(DIK_UP)) {
				car->PressKeyUp = true;

				if (!car->FlippingUp)
					car->SetPosition(car->x, car->y - (CAR_UP_BBOX_HEIGHT - CAR_BBOX_HEIGHT));
				car->FlippingUp = true;
				if (game->IsKeyDown(DIK_LEFT)) {
					car->SetState(CAR_STATE_WALKING_UP_LEFT);
				}
				else if (game->IsKeyDown(DIK_RIGHT)) {
					car->SetState(CAR_STATE_WALKING_UP_RIGHT);
				}
				else
					car->SetState(CAR_STATE_UP);
			}

			else if (game->IsKeyDown(DIK_RIGHT)) {
				car->SetState(CAR_STATE_WALKING_RIGHT);
			}
			else if (game->IsKeyDown(DIK_LEFT)) {
				car->SetState(CAR_STATE_WALKING_LEFT);
			}

			else if (game->IsKeyDown(DIK_X)) {
				if (car->PressJump == true) {
					car->IsJumping = true;
				}
			}
		}
	}
	if (human != NULL) {
		if (human->isActive == true) {
			if (human->isLying == true) {
				if (game->IsKeyDown(DIK_RIGHT)) {
					human->SetState(HUMAN_STATE_LIE_MOVE_RIGHT);
				}
				else if (game->IsKeyDown(DIK_LEFT)) {
					human->SetState(HUMAN_STATE_LIE_MOVE_LEFT);
				}
			}
			else {
				if (game->IsKeyDown(DIK_RIGHT)) {
					human->SetState(HUMAN_STATE_WALKING_RIGHT);
				}
				else if (game->IsKeyDown(DIK_LEFT)) {
					human->SetState(HUMAN_STATE_WALKING_LEFT);
				}
				else if (game->IsKeyDown(DIK_X)) {
					if (human->PressJump == true) {
						human->IsJumping = true;
					}
				}
			}
		}
	}
}
