#pragma once
#include "GameObject.h"
#include "Game.h"
#include "Camera.h"
#include <vector>
#include <unordered_map>

class Grid
{
	int map_width;
	int map_height;

	int cell_width;
	int cell_height;

	int numberOfColumns;
	int numberOfRows;
	vector<vector<vector<LPGAMEOBJECT>>> cells;
public:
	Grid(int map_width, int map_height);
	~Grid();
	void PushObjectIntoGrid(vector<LPGAMEOBJECT> listObj);
	void GetListObject(float camX, float camY, vector<LPGAMEOBJECT>& listObj);
	void DeleteObject(vector<LPGAMEOBJECT> listObj, CGameObject& object);
	void Reset(int map_width, int map_height);
};

