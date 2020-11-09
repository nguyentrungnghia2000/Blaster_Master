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

#define SCREEN_WIDTH	300
#define SCREEN_HEIGHT	240

class CPlayScene : public CScene
{
protected:
	//CMario* player;					// A play scene has to have player, right? 
	CCar* player;
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

	//CMario* GetPlayer() { return player; }
	CCar* GetPlayer() { return player; }
	vector<LPGAMEOBJECT> ReturnObject() { return objects; }

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
	bool up = true;
	bool jump = true;
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

