#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height;
public:
	CPortal(float l, float t, float r, float b, int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetSceneId() { return scene_id; }

	// these funtion are useless
	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }
};