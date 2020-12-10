#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
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
#define OBJECT_TYPE_CAR		5
#define OBJECT_TYPE_BUG		7
#define OBJECT_TYPE_ROBOT	8
#define OBJECT_TYPE_BEE		9
#define OBJECT_TYPE_MAYBUG	10
#define OBJECT_TYPE_DOOM	11
#define OBJECT_TYPE_SPIDER	12


#define OBJECT_TYPE_ITEM	100

#define OBJECT_TYPE_PORTAL	50

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

	switch (object_type) {
	case OBJECT_TYPE_CAR:
		if (player != NULL) {
			DebugOut(L"[ERROR] CAR object was created before!\n");
			return;
		}
		obj = new CCar(x, y);
		player = (CCar*)obj;

		DebugOut(L"[INFO] Player CAR created!\n");
		break;
	case OBJECT_TYPE_HUMAN:
		if (player2 != NULL) {
			DebugOut(L"[ERROR] HUMAN object was created before!\n");
			return;
		}
		obj = new Human(x, y);
		player2 = (Human*)obj;
		DebugOut(L"[INFO] Player HUMAN created!\n");
		break;
	case OBJECT_TYPE_BIGHUMAN:
		if (player3 != NULL) {
			DebugOut(L"[ERROR] BIGHUMAN object was created before!\n");
			return;
		}
		obj = new BigHuman(x, y);
		player3 = (BigHuman*)obj;
		DebugOut(L"[INFO] Player BIGHUMAN created!\n");
		break;
	case OBJECT_TYPE_BRICK:
		w = atof(tokens[4].c_str());
		h = atof(tokens[5].c_str());
		obj = new CBrick(w, h);
		break;
	case OBJECT_TYPE_BUG: obj = new Bug(player); break;
	case OBJECT_TYPE_ROBOT: obj = new Robot(player); break;
	case OBJECT_TYPE_BEE: obj = new Bee(player); break;
	case OBJECT_TYPE_MAYBUG: obj = new MayBug(player); break;
	case OBJECT_TYPE_DOOM:
		IsBrick = atof(tokens[5].c_str());
		IsState = atof(tokens[4].c_str());
		obj = new Doom(player, IsState, IsBrick);
		break;
	case OBJECT_TYPE_SPIDER: obj = new Spider(player); break;
	case OBJECT_TYPE_ITEM: obj = new Item(); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	//set id of the map
	maptextures = CTextures::GetInstance()->Get(40);

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
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


	//Load map
	spriteMap = CSprites::GetInstance()->Get(123456);


	LoadObjects();
}

void CPlayScene::LoadObjects()
{
	/*if (player == NULL) {
		player = new CCar(50, 95);
	}
	if (player2 == NULL) {
		player2 = new Human(50, 95);
	}*/
	if (playerHUD == NULL) {
		playerHUD = new HUD(PLAYER_HEALTH, PLAYER_HEALTH);
	}
}

void CPlayScene::Update(DWORD dt)
{
#pragma region CAMERA
	float cx, cy;
	CGame* GameCamera = CGame::GetInstance();
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
	else if (player2->isActive == true) {
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
		}
		GameCamera->SetCamPos(cx, cy);
	}
#pragma endregion

#pragma region objects and bullets

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++) {
		coObjects.push_back(objects[i]);
	}
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
	if (player2->isAttack)
	{
		MainBullets* bullet = new MainBullets();
		player2->Get_HumanDirection(bullet->BulletDirection);

		if (bullet->BulletDirection > 0)
			bullet->SetPosition(player2->x + DISTANCE_TO_HUMAN_WIDTH_RIGHT, player2->y + DISTANCE_TO_HUMAN_HEIGTH);
		else
			bullet->SetPosition(player2->x + DISTANCE_TO_HUMAN_WIDTH_LEFT, player2->y + DISTANCE_TO_HUMAN_HEIGTH);
		lsBullets.push_back(bullet);
		player2->isAttack = false;
	}
#pragma endregion

#pragma region update objects
	for (size_t i = 0; i < objects.size(); i++) {
		objects[i]->Update(dt, &coObjects);
	}
	/*player->Update(dt, &coObjects);
	player2->Update(dt, &coObjects);*/
	for (int i = 0; i < lsBullets.size(); i++) {
		lsBullets[i]->Update(dt, &coObjects);
	}
	for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i]->Get_IsDead() == true) {
			objects.erase(objects.begin() + i);
		}
	}
	for (int i = 0; i < lsBullets.size(); i++) {
		if (lsBullets[i]->Get_IsFinish() == true)
			lsBullets.erase(lsBullets.begin() + i);
	}
	if (player->IsDead == true) {
		//player->y = (CAR_DIE_BBOX_HEIGTH - CAR_BBOX_HEIGHT);
	}
	if (playerHUD != NULL) {
		if (player->isActive == true) {
			player->Get_health(healthunit);
			player->Get_power(powerunit);
			playerHUD->Update(cx, cy, healthunit, powerunit);
		}
		else if (player2->isActive == true) {
			player2->Get_health(healthunit);
			player2->Get_power(powerunit);
			playerHUD->Update(cx, cy, healthunit, powerunit);
		}
	}
#pragma endregion

	if (player == NULL) return;
	if (player2 == NULL) return;
}

void CPlayScene::Render()
{
	spriteMap->Draw(0, 0);
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

	/*player->Render();
	player2->Render();*/

	for (int i = 0; i < lsBullets.size(); i++) {
		lsBullets[i]->Render();
	}
	if (player->isActive == true) {
		playerHUD->Render(player);
	}
	else if (player2->isActive == true) {
		playerHUD->Render(player2);
	}
}

void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	//CGameObject* controlplayer = ((CPlayScene*)scence)->Get_Player();
	CCar* car = ((CPlayScene*)scence)->GetPlayer();
	Human* human = ((CPlayScene*)scence)->GetPlayer1();
	BigHuman* bhuman = ((CPlayScene*)scence)->GetPlayer2();
	vector<LPGAMEOBJECT> listObjects = ((CPlayScene*)scence)->ReturnObject();
	float Carx = 0, Cary = 0;
	int direction;
	bool IsTargetTop;
	car->Get_CarStateForBullet(direction, IsTargetTop, Carx, Cary);

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
			}
			else if (car->isActive == false && human->isActive == true) {
				if (human->isCollisionWithCar == true) {
					car->isActive = true;
					human->isActive = false;
				}
				else
					return;
			}
			if (human->isActive == false) {
				car->Get_CarDirection(human->nx);
				if (human->nx > 0)
					human->SetPosition(car->x + HUMAN_APPEAR_DISTANCE_WIDTH_RIGHT, car->y + HUMAN_APPEAR_DISTANCE_HEIGHT);
				else
					human->SetPosition(car->x + HUMAN_APPEAR_DISTANCE_WIDTH_LEFT, car->y + HUMAN_APPEAR_DISTANCE_HEIGHT);
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
		break;
	case DIK_R:
		if (car != NULL && human != NULL) {
			car->Reset();
			human->Reset();
		}
		break;
	case DIK_P:
		if (CGame::GetInstance()->GetIDCurrentScene() == 8)
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
				bhuman->SetState(BIGHUMAN_STATE_MOVE_DOWN);
			}
		}
		if (human != NULL) {
			if (human->isActive == true) {
				human->PressDown = true;
			}
		}
		break;
	case DIK_RIGHT:
		if (bhuman != NULL) {
			if (bhuman->isActive == true) {
				bhuman->SetState(BIGHUMAN_STATE_MOVE_RIGHT);
			}
		}
		break;
	case DIK_UP:
		if (human != NULL) {
			if (human->isActive == true) {
				if (human->PressDown == true) {
					human->PressDown = false;
					human->isLying = false;
					human->SetPosition(human->x, human->y - (HUMAN_BBOX_HEIGHT - HUMAN_LIE_BBOX_HEIGHT));
				}
			}
		}
		if (bhuman != NULL) {
			if (bhuman->isActive == true) {
				bhuman->SetState(BIGHUMAN_STATE_MOVE_UP);
			}
		}
		break;
	case DIK_LEFT:
		if (bhuman != NULL) {
			if (bhuman->isActive == true) {
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
