#include "Globals.h"
#include <iostream>
#include <string>
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	pinball = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	pinball = App->textures->Load("textures/spritesheet.png"); 
	App->audio->PlayMusic("music/Sapphire_field.ogg", -1);
	spoink_fx = App->audio->LoadFx("audio/Throw.wav");
	flipper_fx = App->audio->LoadFx("audio/Do.wav");
	points_fx = App->audio->LoadFx("audio/Pokemon hit.wav");
	font = App->fonts->Load("textures/Fonts.png", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ ");

	//PHYSICS BODY CREATION//

	//create ball
	circles.add(App->physics->CreateCircle(initPosition.x, initPosition.y, 11));
	circles.getLast()->data->listener = this;

	//create flippers
	
	boxes.add(App->physics->CreateRectangle(134, 600, 39, 10));
	boxes.getLast()->data->listener = this;
	circles.add(App->physics->CreateCircle(133, 600, 2));
	circles.getLast()->data->body->SetType(b2_staticBody);

	boxes.add(App->physics->CreateRectangle(186, 600, 39, 10));
	boxes.getLast()->data->listener = this;
	circles.add(App->physics->CreateCircle(226, 602, 2));
	circles.getLast()->data->body->SetType(b2_staticBody);

	//create all bodies
	circles.add(App->physics->CreateCircle(198, 210, 10));
	circles.getLast()->data->body->SetType(b2_staticBody);
	circles.getLast()->data->listener = this;
	circles.add(App->physics->CreateCircle(142, 215, 10));
	circles.getLast()->data->body->SetType(b2_staticBody);
	circles.getLast()->data->listener = this;
	circles.add(App->physics->CreateCircle(170, 250, 10));
	circles.getLast()->data->body->SetType(b2_staticBody);
	circles.getLast()->data->listener = this;
	circles.add(App->physics->CreateCircle(105, 292, 10));
	circles.getLast()->data->body->SetType(b2_staticBody);
	boxes.add(App->physics->CreateRectangle(365, 570, 25, 15));
	boxes.getLast()->data->body->SetType(b2_kinematicBody);
	boxes.add(App->physics->CreateRectangle(48, 588, 25, 35));
	boxes.getLast()->data->body->SetType(b2_kinematicBody);
	walls.add(App->physics->CreateChain(0, 0, wall_left, 20));
	walls.add(App->physics->CreateChain(0, 0, wall_right, 20));
	walls.add(App->physics->CreateChain(0, 0, level2, 96));
	

	bumper_l = App->physics->CreateTriangle({ 0, 0 }, { -29, 46 }, { 2, 31 }, { 258, 509 });
	bumper_l->body->SetType(b2_staticBody);
	bumper_r = App->physics->CreateTriangle({ 0, 0 }, { 0, 34 }, { 29, 47 }, { 100, 509 });
	bumper_r->body->SetType(b2_staticBody);

	//create sensors
	sensorfall = App->physics->CreateRectangleSensor(180,  SCREEN_HEIGHT+10, 64, 20);
	sensorminum = App->physics->CreateRectangleSensor(135, 292, 10, 10);
	sensorplusle = App->physics->CreateRectangleSensor(87, 320, 10, 10);
	sensormart = App->physics->CreateRectangleSensor(93, 270, 10, 10);
	sensorpikachu = App->physics->CreateRectangleSensor(328, 255, 10, 10);
	sensorwailmer = App->physics->CreateRectangleSensor(288, 273, 10, 10);
	sensorpelipper = App->physics->CreateRectangleSensor(233, 270, 10, 10);
	sensorentrance2 = App->physics->CreateRectangleSensor(65, 335, 10, 10);
	sensorentrance1 = App->physics->CreateRectangleSensor(280, 340, 10, 10);
	sensorentrance3 = App->physics->CreateRectangleSensor(230, 205, 10, 10);
	sensorlights = App->physics->CreateRectangleSensor(362, 165, 10, 10);

	//create joints
	b2RevoluteJointDef revJoint_l;
	revJoint_l.bodyA = boxes.getFirst()->data->body;
	revJoint_l.bodyB = circles.getFirst()->next->data->body;
	revJoint_l.localAnchorA.Set(PIXEL_TO_METERS(-18), PIXEL_TO_METERS(0));
	revJoint_l.localAnchorB.Set(0, 0);
	revJoint_l.lowerAngle = DEGTORAD*(-30);
	revJoint_l.referenceAngle = 0;
	revJoint_l.upperAngle = DEGTORAD*30;
	revJoint_l.enableLimit = true;
	b2RevoluteJoint* joint_l = (b2RevoluteJoint*)boxes.getFirst()->data->body->GetWorld()->CreateJoint(&revJoint_l);

	b2RevoluteJointDef revJoint_r;
	revJoint_r.bodyA = boxes.getFirst()->next->data->body;
	revJoint_r.bodyB = circles.getFirst()->next->next->data->body;
	revJoint_r.localAnchorA.Set(PIXEL_TO_METERS(18), PIXEL_TO_METERS(0));
	revJoint_r.localAnchorB.Set(0, 0);
	revJoint_r.lowerAngle = DEGTORAD * (-30);
	revJoint_r.referenceAngle = 0;
	revJoint_r.upperAngle = DEGTORAD * (30);
	revJoint_r.enableLimit = true;
	b2RevoluteJoint* joint_r = (b2RevoluteJoint*)boxes.getFirst()->next->data->body->GetWorld()->CreateJoint(&revJoint_r);

	//SPRITE COORDINATES//

	ball = { 8, 1689, 22, 22 };
	background = { 803, 6, SCREEN_WIDTH, SCREEN_HEIGHT };
	background1 = { 18, 6, SCREEN_WIDTH, 344 };
	background2 = { 410, 6, SCREEN_WIDTH, 576 };
	flipper_l = { 154, 762, 47, 18 };
	flipper_r = { 154, 785, 47, 18 };
	lifeball = { 263, 1266, 23, 22 };
	nest1 = { 561, 1467, 43, 12 };
	nest2 = { 561, 1481, 43, 23 };
	

	//water
	water.PushBack({ 1010, 731, 57, 63 });
	water.PushBack({ 1070, 731, 57, 63 });
	water.PushBack({ 1130, 731, 57, 63 });
	water.speed = 0.05f;

	//Spoink
	spoink.PushBack({ 876, 1430, 31, 75 });
	spoink.PushBack({ 909, 1430, 31, 75 });
	spoink.speed = 0.05f;

	downSpoink.PushBack({ 942, 1430, 31, 75 });
	downSpoink.PushBack({ 975, 1430, 31, 75 });
	redSpoink.PushBack({ 1008, 1430, 33, 75 });
	redSpoink.PushBack({ 1039, 1430, 33, 75 });
	redSpoink.PushBack({ 1071, 1430, 33, 75 });
	redSpoink.speed = 0.3f;
	
	//Pikachu
	pikachu.PushBack({ 162, 2000, 38, 36 });
	pikachu.PushBack({ 200, 2000, 38, 36 });
	pikachu.speed = 0.05f;
	pikachu_pos = 30;

	//Mart
	mart.PushBack({ 310, 733, 75, 55 });
	mart.PushBack({ 387, 733, 75, 55 });
	mart.PushBack({ 463, 733, 75, 55 });
	mart.speed = 0.08f;

	//shroomish
	shroomish.PushBack({ 94, 1471, 41, 36 });
	shroomish.PushBack({ 137, 1471, 41, 36 });
	shroomish.speed = 0.02f;

	//Pluse
	plusle.PushBack({ 157, 1527, 34, 55 });
	plusle.PushBack({ 193, 1527, 34, 55 });
	plusle.PushBack({ 229, 1527, 34, 55 });
	plusle.speed = 0.08f;

	//Minum
	minum.PushBack({ 157, 1594, 34, 55 });
	minum.PushBack({ 193, 1594, 34, 55 });
	minum.PushBack({ 229, 1594, 34, 55 });
	minum.speed = 0.08f;

	//Wailmer
	wailmer.PushBack({ 265, 1461, 52, 44 });
	wailmer.PushBack({ 319, 1461, 57, 44 });
	wailmer.speed = 0.07f;

	//Pelipper
	pelipper.PushBack({ 111, 1375, 61, 52 });
	pelipper.PushBack({ 174, 1375, 61, 52 });
	pelipper.speed = 0.05f;

	//Zigzagoon
	zigzagoon.PushBack({ 274, 1684, 60, 46 });
	zigzagoon.PushBack({ 336, 1684, 60, 46 });
	zigzagoon.speed = 0.07f;

	//Mart door
	mart_minplus.PushBack({ 993, 1584, 25, 23 });
	mart_minplus.PushBack({ 1020, 1584, 25, 23 });
	mart_minplus.PushBack({ 1047, 1584, 25, 23 });
	mart_minplus.speed = 0.07f;

	mart_plusle.PushBack({ 900, 1555, 25, 18 });
	mart_plusle.PushBack({ 927, 1555, 25, 18 });
	mart_plusle.PushBack({ 954, 1555, 25, 18 });
	mart_plusle.speed = 0.07f;

	mart_minum.PushBack({ 901, 1623, 25, 17 });
	mart_minum.PushBack({ 928, 1623, 25, 17 });
	mart_minum.PushBack({ 955, 1623, 25, 17 });
	mart_minum.speed = 0.07f;

	martMin = true;
	martPlus = true;

	//PikaCharge
	pikaRoulette.PushBack({ 349, 1264, 28, 27 });
	pikaRoulette.PushBack({ 379, 1264, 28, 27 });
	pikaRoulette.PushBack({ 409, 1264, 28, 27 });
	pikaRoulette.PushBack({ 439, 1264, 28, 27 });
	pikaRoulette.PushBack({ 469, 1264, 28, 27 });
	pikaRoulette.PushBack({ 499, 1264, 28, 27 });
	pikaRoulette.PushBack({ 529, 1264, 28, 27 });
	pikaRoulette.PushBack({ 559, 1264, 28, 27 });
	pikaRoulette.PushBack({ 589, 1264, 28, 27 });
	pikaRoulette.PushBack({ 619, 1264, 28, 27 });
	pikaRoulette.PushBack({ 649, 1264, 28, 27 });
	pikaRoulette.PushBack({ 679, 1264, 28, 27 });
	pikaRoulette.PushBack({ 709, 1264, 28, 27 });
	pikaRoulette.PushBack({ 739, 1264, 28, 27 });
	pikaRoulette.PushBack({ 769, 1264, 28, 27 });
	pikaRoulette.speed = 1.0f;

	pikaCharge.PushBack({ 216, 1309, 54, 37 });
	pikaCharge.PushBack({ 271, 1309, 54, 37 });
	pikaCharge.PushBack({ 327, 1309, 54, 37 });
	pikaCharge.PushBack({ 382, 1309, 54, 37 });
	pikaCharge.speed = 0.09f;
	pikaCharge.loop = false;

	pikaCharge2.PushBack({ 438, 1309, 54, 37 });
	pikaCharge2.PushBack({ 492, 1309, 54, 37 });
	pikaCharge2.PushBack({ 549, 1309, 54, 37 });
	pikaCharge2.PushBack({ 604, 1309, 54, 37 });
	pikaCharge2.PushBack({ 660, 1309, 54, 37 });
	pikaCharge2.speed = 0.09f;
	pikaCharge2.loop = false;

	pikaCharge3.PushBack({ 715, 1309, 54, 37 });
	pikaCharge3.PushBack({ 771, 1309, 54, 37 });
	pikaCharge3.PushBack({ 826, 1309, 54, 37 });
	pikaCharge3.PushBack({ 882, 1309, 54, 37 });
	pikaCharge3.PushBack({ 937, 1309, 54, 37 });
	pikaCharge3.PushBack({ 0, 0, 2, 2 });
	pikaCharge3.PushBack({ 937, 1309, 54, 37 });
	pikaCharge3.PushBack({ 0, 0, 2, 02 });
	pikaCharge3.PushBack({ 937, 1309, 54, 37 });
	pikaCharge3.PushBack({ 937, 1309, 54, 37 });
	pikaCharge3.speed = 0.09f;
	pikaCharge3.loop = false;

	pikaStatic = { 349, 1264, 28, 27 };
	pikaCount = 0;
	pikaLap = 0;

	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//Blit background
	App->renderer->Blit(pinball, 0, 0, &background);

	//Water
	App->renderer->Blit(pinball, 253, 256, &(water.GetCurrentFrame()));

	//Spoink
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && spoinkable)
	{
		App->renderer->Blit(pinball, 350, 550, &(downSpoink.GetCurrentFrame()));
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && spoinkable)
	{
		App->renderer->Blit(pinball, 348, 550, &(redSpoink.GetCurrentFrame()));
	}
	else
	{
		App->renderer->Blit(pinball, 350, 550, &(spoink.GetCurrentFrame()));
	}

	//Flippers
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		boxes.getFirst()->data->body->ApplyForceToCenter(b2Vec2(0, -200), 1);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		boxes.getFirst()->next->data->body->ApplyForceToCenter(b2Vec2(0, -200), 1);
	}

	//Draw Ball 
	PhysBody* pokeball = circles.getFirst()->data;
	int x, y;
	pokeball->GetPosition(x, y);
	App->renderer->Blit(pinball, x, y, &ball, 1.0f, pokeball->GetRotation());

	//Draw Flippers
	PhysBody* flipperl = boxes.getFirst()->data;
	flipperl->GetPosition(x, y);
	App->renderer->Blit(pinball, x - 10, y - 2, &flipper_l, 1.0f, (flipperl->GetRotation()));
	PhysBody* flipperr = boxes.getFirst()->next->data;
	flipperr->GetPosition(x, y);
	App->renderer->Blit(pinball, x + 5 , y - 2, &flipper_r, 1.0f, (flipperr->GetRotation()));


	//Mart
	App->renderer->Blit(pinball, 32, 172, &(mart.GetCurrentFrame()));

	//Shroomish
	App->renderer->Blit(pinball, 122, 195, &(shroomish.GetCurrentFrame()));
	App->renderer->Blit(pinball, 177, 190, &(shroomish.GetCurrentFrame()));
	App->renderer->Blit(pinball, 150, 230, &(shroomish.GetCurrentFrame()));

	//Wailmer
	App->renderer->Blit(pinball, 260, 245, &(wailmer.GetCurrentFrame()));

	//PikaCharge
	if (pikaLap == 0)
	{
		App->renderer->Blit(pinball, 314, 240, &pikaStatic);
	}
	else
	{
		App->renderer->Blit(pinball, 314, 240, &(pikaRoulette.GetCurrentFrame()));
		
		if (pikaRoulette.Finished())
		{
			++pikaLap;
			pikaRoulette.Reset();
		}

		if (pikaLap > 7)
		{
			pikaLap = 0;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN || pika)
	{
		pikaLap = 1;
		++pikaCount;
		pika = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		pikaCount = 0;
		pikaCharge.Reset();
		pikaCharge2.Reset();
		pikaCharge3.Reset();
	}

	//Pikachu
	if (pikaCount >= 3)
	{
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			pikachu_pos = 30;
			boxes.getLast()->data->body->SetTransform({ PIXEL_TO_METERS(48), PIXEL_TO_METERS(588) }, boxes.getLast()->data->body->GetAngle());
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			pikachu_pos = 295;
			boxes.getLast()->data->body->SetTransform({ PIXEL_TO_METERS(313), PIXEL_TO_METERS(588) }, boxes.getLast()->data->body->GetAngle());
		}
		App->renderer->Blit(pinball, pikachu_pos, 570, &(pikachu.GetCurrentFrame()));

		if (pikaburst)
		{
			boxes.getLast()->data->body->SetLinearVelocity(b2Vec2(0, -10));
			
			pikaburst = false;
		}
		if (boxes.getLast()->data->body->GetPosition().y < PIXEL_TO_METERS(550));
		{
			int pos = pikachu_pos + 18;
			boxes.getLast()->data->body->SetTransform({ PIXEL_TO_METERS(pos), PIXEL_TO_METERS(588) }, boxes.getLast()->data->body->GetAngle());
		}
	}
		

	//background 1 / 2
	App->renderer->Blit(pinball, 0, 0, &background2);
	App->renderer->Blit(pinball, 0, 0, &background1);
	
	//Zigzagoon
	App->renderer->Blit(pinball, 300, 425, &(zigzagoon.GetCurrentFrame()));

	//Nest
	App->renderer->Blit(pinball, 15, 440, &nest1);
	App->renderer->Blit(pinball, 15, 445, &nest2);

	//Pelipper
	App->renderer->Blit(pinball, 223, 180, &(pelipper.GetCurrentFrame()));

	//Plusle
	App->renderer->Blit(pinball, 58, 248, &(plusle.GetCurrentFrame()));

	//Minum
	App->renderer->Blit(pinball, 107, 218, &(minum.GetCurrentFrame()));

	//Mart door
	if (martMin && martPlus)
	{
		App->renderer->Blit(pinball, 90, 280, &(mart_minplus.GetCurrentFrame()));
	}
	else if (martMin)
	{
		App->renderer->Blit(pinball, 90, 280, &(mart_minum.GetCurrentFrame()));
	}
	else if (martPlus)
	{
		App->renderer->Blit(pinball, 92, 285, &(mart_plusle.GetCurrentFrame()));
	}

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		if (martMin)
		{
			martMin = false;
		}
		else
		{
			martMin = true;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		if (martPlus)
		{
			martPlus = false;
		}
		else
		{
			martPlus = true;
		}
	}

	if ((martMin || martPlus) && circles.count() == 6)
	{
		circles.add(App->physics->CreateCircle(105, 292, 10));
		circles.getLast()->data->body->SetType(b2_staticBody);
	}
	else if (!martMin && !martPlus && circles.count() == 7)
	{
		circles.getLast()->data->body->GetWorld()->DestroyBody(circles.getLast()->data->body);
		circles.del(circles.getLast());
	}

	//Spoink

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && spoinkable)
	{
		boxes.getLast()->prev->data->body->SetLinearVelocity(b2Vec2(0, 1));
		App->audio->PlayFx(spoink_fx);
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP && spoinkable)
	{
		boxes.getLast()->prev->data->body->SetLinearVelocity(b2Vec2(0, -20));
		
	}
	else if (boxes.getLast()->prev->data->body->GetPosition().y > PIXEL_TO_METERS(590) || boxes.getLast()->prev->data->body->GetPosition().y < PIXEL_TO_METERS(570))
	{
		if (boxes.getLast()->prev->data->body->GetPosition().y < PIXEL_TO_METERS(570))
		{
			boxes.getLast()->prev->data->body->SetTransform({ boxes.getLast()->prev->data->body->GetPosition().x, PIXEL_TO_METERS(570) }, boxes.getLast()->prev->data->body->GetAngle());
		}
		
		boxes.getLast()->prev->data->body->SetLinearVelocity(b2Vec2(0, 0));
	}

	//PikaCharge Panel

	if (pikaCount == 1 && !pikaCharge.Finished())
	{
		App->renderer->Blit(pinball, 152, 325, &(pikaCharge.GetCurrentFrame()));
	}
	if (pikaCount == 2 && !pikaCharge2.Finished())
	{
		App->renderer->Blit(pinball, 152, 325, &(pikaCharge2.GetCurrentFrame()));
	}
	if (pikaCount == 3 && !pikaCharge3.Finished())
	{
		App->renderer->Blit(pinball, 152, 325, &(pikaCharge3.GetCurrentFrame()));
	}

	//Set ball to initial position
	if (goInit)
	{
		circles.getFirst()->data->body->SetTransform({ PIXEL_TO_METERS(initPosition.x), PIXEL_TO_METERS(initPosition.y) }, circles.getFirst()->data->body->GetAngle());
		goInit = false;
		changelvl = true;
	}

	//Change levels
	if (changelvl || App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (lvl2)
		{
			walls.getLast()->data->body->GetWorld()->DestroyBody(walls.getLast()->data->body);
			walls.del(walls.getLast());
			walls.add(App->physics->CreateChain(0, 0, wall_main, 84));
			walls.add(App->physics->CreateChain(0, 0, mart_wall, 40));
			walls.add(App->physics->CreateChain(0, 0, island, 86));
			walls.add(App->physics->CreateChain(0, 0, mini_wall1, 8));
			walls.add(App->physics->CreateChain(0, 0, mini_wall2, 8));
			walls.add(App->physics->CreateChain(0, 0, entrance1, 36));
			walls.add(App->physics->CreateChain(0, 0, entrance2, 20));
			lvl2 = false;
			lvl1 = true;
		}
		else if (lvl1)
		{
			int num = walls.count();
			for (int i = 0; i < num-2; i++)
			{
				walls.getLast()->data->body->GetWorld()->DestroyBody(walls.getLast()->data->body);
				walls.del(walls.getLast());
			}
			walls.add(App->physics->CreateChain(0, 0, level2, 96));
			lvl2 = true;
			lvl1 = false;
		}
		changelvl = false;
		
	}

	//Lose screen
	if (lose)
	{
		if (score > hi_score)
		{
			hi_score = score;
		}
		App->renderer->DrawQuad({ SCREEN_WIDTH / 6, SCREEN_HEIGHT / 4,  250, 380 }, 0, 0, 0, 200);
		App->fonts->BlitText(SCREEN_WIDTH / 6 + 30, SCREEN_HEIGHT / 4 + 30, font, "CURRENT SCORE");
		App->fonts->BlitText(SCREEN_WIDTH / 6 + 45 , SCREEN_HEIGHT / 4 + 65 , font, std::to_string(score).c_str());
		App->fonts->BlitText(SCREEN_WIDTH / 6 + 30, SCREEN_HEIGHT / 2 - 40, font, "PREVIOUS SCORE");
		App->fonts->BlitText(SCREEN_WIDTH / 6 + 45, SCREEN_HEIGHT / 2 - 5, font, std::to_string(prev_score).c_str());
		App->fonts->BlitText(SCREEN_WIDTH / 6 + 30, SCREEN_HEIGHT / 2 + 70, font, "HIGHEST SCORE");
		App->fonts->BlitText(SCREEN_WIDTH / 6 + 85, SCREEN_HEIGHT / 2 + 120, font, std::to_string(hi_score).c_str());
		App->fonts->BlitText(SCREEN_WIDTH / 6 + 70, SCREEN_HEIGHT / 2 + 185, font, "PRESS R");
		spoinkable = false;
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			prev_score = score;
			score = 0;
			spoinkable = true;
			lose = false;
		}
	}

	//UI
	App->fonts->BlitText(5, SCREEN_HEIGHT - 30, font, std::to_string(score).c_str());
	App->fonts->BlitText(SCREEN_WIDTH - 20, SCREEN_HEIGHT - 30, font, std::to_string(balls).c_str());
	App->renderer->Blit(pinball, 335, 620, &lifeball);

	//Move ball to mouse
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		circles.getFirst()->data->body->SetTransform({ PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) }, circles.getFirst()->data->body->GetAngle());
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//lvl change
	if ( bodyB == sensorentrance1 || bodyB == sensorentrance2 ||(bodyB->body->GetLinearVelocity().y < 0 && bodyB == sensorentrance3))
	{
		changelvl = true;
	}

	//lose
	if (bodyB == sensorfall && balls > 0)
	{
		goInit = true;
		balls--;
		return;
	}

	if (bodyB == sensorfall && balls <= 0)
	{
		goInit = true;
		balls = 2;
		martPlus = true;
		martMin = true;
		lose = true;
		
		
	}

	//minum & plusle  & mart
	if (bodyB == sensorminum)
	{
		martMin = !martMin;
	}

	if (bodyB == sensorplusle)
	{
		martPlus = !martPlus;
	}

	if (bodyB == sensormart)
	{
		App->audio->PlayFx(points_fx);
		score += 300;
		balls++;
		return;
	}

	//shroomish
	if (bodyB == circles.getLast()->prev->data || bodyB == circles.getLast()->prev->prev->data || bodyB == circles.getLast()->prev->prev->prev->data)
	{
		App->audio->PlayFx(points_fx);
		score += 200;
		return;
	}

	//bumper collision
	if (bodyB == bumper_l || bodyB == bumper_r)
	{
		App->audio->PlayFx(points_fx);
		score += 100;
		return;
	}
	
	//flipper fx
	if (bodyB == boxes.getFirst()->data || bodyB == boxes.getFirst()->next->data)
	{
		App->audio->PlayFx(flipper_fx);
		return;
	}

	//pikacharge
	if (bodyB == sensorpikachu)
	{
		pika = true;
	}

	//pikachu
	if (bodyB == boxes.getLast()->data)
	{
		pikaburst = true;
	}
}
