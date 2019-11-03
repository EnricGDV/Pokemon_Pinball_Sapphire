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
	//initial position ball
	p2Point<float> initPosition = { 365, 550 };

	//bodies
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> walls;

	//bumpers
	PhysBody* bumper_l;
	PhysBody* bumper_r;
	
	//sprite rectangles
	SDL_Rect ball;
	SDL_Rect flipper_l;
	SDL_Rect flipper_r;
	SDL_Rect background;
	SDL_Rect background1;
	SDL_Rect background2;
	SDL_Rect pikaStatic;
	SDL_Rect lifeball;
	SDL_Rect nest1;
	SDL_Rect nest2;

	//font
	int font;

	//sensors
	PhysBody* sensorfall;
	PhysBody* sensorminum;
	PhysBody* sensorplusle;
	PhysBody* sensormart;
	PhysBody* sensorpikachu;
	PhysBody* sensorwailmer;
	PhysBody* sensorpelipper;
	PhysBody* sensorentrance1;
	PhysBody* sensorentrance2;
	PhysBody* sensorentrance3;
	PhysBody* sensorzigzagoon;
	PhysBody* sensorlights;

	//booleans & variables
	bool sensed;
	bool lvl1 = false;
	bool lvl2 = true;
	bool goInit = false;
	bool changelvl = false;
	int balls = 2;
	int prev_score = 0;
	int score = 0;
	int hi_score = 0;
	char scores[5] = {};
	int lose = false;
	int spoinkable = true;
	
	bool martPlus;
	bool martMin;

	bool srmshHit = false;

	bool pika = false;
	bool pikaburst = false;

	uint pikaLap;
	uint pikaCount;
	uint pikachu_pos;
	uint spoink_fx;
	uint flipper_fx;
	uint points_fx;

	p2Point<int> ray;
	bool ray_on;

	//animations
	SDL_Texture* pinball;
	Animation water;
	Animation spoink;
	Animation redSpoink;
	Animation downSpoink;
	Animation pikachu;
	Animation mart;
	Animation shroomish;
	Animation shroomishHit;
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

	//wall coordinates
	//create chains
	int wall_main[84] =
	{
	212, 647, 212, 640, 324, 577,  324, 510, 324, 497, 317, 487, 306, 484, 296, 483, 288, 476, 288, 440, 309, 416, 329, 377, 338, 344, 342, 311, 340, 250, 331, 211, 314, 172,
	285, 143, 246, 121, 202, 110, 155, 110, 107, 123, 66, 150, 40, 183, 25, 218, 15, 260, 14, 296, 18, 343, 28, 375, 46, 409, 56, 424, 70, 441, 70, 474, 63, 482, 52, 482, 42, 
	485, 36, 494, 34, 508, 34, 578, 150, 641, 148, 664, 212, 664
	};

	int wall_left[20] = 
	{
	137, 596, 129, 604, 68, 566, 65, 562, 65, 511, 68, 507, 73, 507, 73, 549, 77, 557, 135, 594
	};

	int wall_right[20] = 
	{
	231, 606, 222, 598, 280, 560, 286, 553, 286, 512, 289, 508, 294, 509, 294, 562, 288, 569, 232, 606
	};

	int mart_wall[40] = 
	{
	96, 313, 80, 322, 58, 286, 49, 261, 50, 245, 61, 218, 76, 196, 100, 173, 119, 160, 120, 180, 107, 192, 106, 208, 122, 245, 143, 285, 128, 293, 108, 264, 96, 260,
	83, 264, 81, 276, 94, 306
	};

	int island[86] = 
	{
	245, 323, 239, 329, 232, 326, 230, 314, 232, 301, 243, 294, 243, 272, 226, 263, 215, 273, 212, 268, 216, 251, 239, 227, 248, 214, 251, 199, 239, 181, 225, 177, 211, 175, 205, 
	172, 205, 161, 211, 150, 250, 161, 272, 178, 283, 193, 292, 210, 301, 229, 311, 249, 314, 267,  316, 284,316, 306, 316, 316, 308, 325, 282, 327, 276, 318, 297, 291, 307, 282, 
	308, 267, 293, 256, 279, 258, 265, 270, 260, 280,257, 293,250, 306, 247, 316
	};

	int mini_wall1[8] = 
	{ 
	153, 180, 143, 180, 143, 153, 153, 153
	};

	int mini_wall2[8] = 
	{
	184, 178, 174, 178, 174, 149, 184, 149
	};

	int entrance1[36] = 
	{
	285, 367, 294, 344, 312, 315, 315, 310, 313, 284, 288, 308, 277, 327, 271, 336, 267, 345, 265, 342, 281, 312, 311, 282, 317, 277, 318, 297, 320, 310, 305, 331, 296, 348, 297, 357
	};

	int entrance2[20] = 
	{
	49, 334, 66, 365, 64, 367, 53, 357, 54, 348, 44, 330, 69, 314, 84, 340, 82, 341, 71, 321
	};

	int level2[96] = 
	{
	82, 340, 60, 296, 46, 256, 44, 232, 44, 187, 51, 156, 73, 124, 107, 101, 137, 93, 176, 94, 210, 115, 220, 139, 223, 171, 216, 201, 241, 208, 247, 192, 247, 170, 243, 130, 231, 108, 
	220, 92, 192, 75, 229, 65, 252, 63, 300, 63, 329, 67, 344, 83, 350, 109, 352, 147, 352, 625, 378, 625, 378, 144, 378, 106, 374, 74, 358, 49, 336, 37, 257, 37, 206, 44, 167, 52, 126, 
	69, 94, 79, 51, 110, 38, 132, 27, 152, 18, 192,16, 222, 20, 264, 37, 310, 63, 365
	};
	
};
