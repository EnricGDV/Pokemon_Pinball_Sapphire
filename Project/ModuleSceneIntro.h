#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	
	//sprite rectangles
	SDL_Rect background;
	SDL_Rect background1;
	SDL_Rect background2;
	SDL_Rect pikaStatic;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* pinball;
	Animation water;
	Animation spoink;
	Animation redSpoink;
	Animation downSpoink;
	Animation pikachu;
	Animation mart;
	Animation shroomish;
	Animation wailmer;
	Animation pelipper;
	Animation minum;
	Animation plusle;
	Animation zigzagoon;
	Animation mart_minplus;
	Animation mart_plusle;
	Animation mart_minum;
	Animation pikaCharge;
	Animation pikaCharge2;
	Animation pikaCharge3;
	Animation pikaRoulette;
	
	bool martPlus;
	bool martMin;

	uint pikaLap;
	uint pikaCount;
	uint pikachu_pos;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
