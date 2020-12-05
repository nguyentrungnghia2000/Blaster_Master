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

#define SCREEN_WIDTH	300
#define SCREEN_HEIGHT	240

class CPlayScene : public CScene
{
protected:

	CGameObject* controlplayer;
	CCar* player;
	Human* player2;
	BigHuman* player3;
	HUD* playerHUD;
	LPDIRECT3DTEXTURE9 maptextures;
	int mapWidth = 1024, mapHeight = 240;
	vector<LPGAMEOBJECT> objects;
	vector<LPCWSTR> ListSceneFilePath;

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
	int healthunit, powerunit;
	CPlayScene(int id, LPCWSTR filePath);
	vector<MainBullets*> lsBullets;
	virtual void Load();
	void LoadObjects();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CCar* GetPlayer() { return player; }
	CGameObject* Get_Player() { return controlplayer; }
	Human* GetPlayer1() { return player2; }
	BigHuman* GetPlayer2() { return player3; }
	vector<LPGAMEOBJECT> ReturnObject() { return objects; }
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

