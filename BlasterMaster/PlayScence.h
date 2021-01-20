#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Car.h"
#include "Bullets.h"
#include "MainBullets.h"
#include "HUD.h"
#include "Human.h"
#include "BigHuman.h"
#include "Camera.h"
#include "Grid.h"
#include "Item.h"
#include "Boss.h"

#define SCREEN_WIDTH	300
#define SCREEN_HEIGHT	240
#define TIMETOCHANGESCENE 500

class CPlayScene : public CScene
{
protected:
	Grid* grid;
	CGameObject* controlplayer;
	CCar* player;
	Human* player2;
	BigHuman* player3;
	HUD* playerHUD;
	Camera* cam;
	Boss* BigBoss;
	LPDIRECT3DTEXTURE9 maptextures;
	int mapWidth = 1024, mapHeight = 240;
	vector<LPGAMEOBJECT> lsObjects;
	vector<LPGAMEOBJECT> lsPlayers;
	vector< LPGAMEOBJECT> lsEnermies;
	vector<LPCWSTR> ListSceneFilePath;
	vector<LPGAMEOBJECT> lsBoss;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_SCENE(string line);
	void _ParseSection_SWITCHSCENE(string line);
	void _ParseSection_MAP(string line);

	LPSPRITE spriteMap;

public:
	float posX, posY;
	int count = 0;
	int healthunit, powerunit;
	//int playeroldhealth, playeroldpower;
	int mapW, mapH;
	bool BossIsDead;
	DWORD TimeToChangeScene;
	CPlayScene(int id, LPCWSTR filePath);

	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> coPlayers;
	vector<LPGAMEOBJECT> coEnermies;
	vector<LPGAMEOBJECT> lsEnermiesGrid;
	vector<Bullets*> lsBullets;
	vector<Item*> lsItems;
	CGame* GameCamera = CGame::GetInstance();

	virtual void Load();
	void LoadObjects();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void GetObjectFromGrid(float x, float y);
	void ChangeScene();

	CCar* GetPlayer() { return player; }
	CGameObject* Get_Player() { return controlplayer; }
	Human* GetPlayer1() { return player2; }
	BigHuman* GetPlayer2() { return player3; }
	vector<LPGAMEOBJECT> ReturnObject() { return lsObjects; }
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
	bool changeplayer = true;
	bool up = true;
	bool jump = true;
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

