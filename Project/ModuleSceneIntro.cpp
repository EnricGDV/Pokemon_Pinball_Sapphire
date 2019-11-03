#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleAudio.h"


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
	bonus_fx = App->audio->LoadFx("audio/bonus.wav");

	//PHYSICS BODY CREATION//

	//create static bodies
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
	boxes.add(App->physics->CreateRectangle(365, 580, 25, 15));
	boxes.getLast()->data->body->SetType(b2_staticBody);
	boxes.add(App->physics->CreateRectangle(48, 588, 25, 35));
	boxes.getLast()->data->body->SetType(b2_staticBody);

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

	background = { 803, 6, SCREEN_WIDTH, SCREEN_HEIGHT };
	background1 = { 18, 6, SCREEN_WIDTH, 344 };
	background2 = { 410, 6, SCREEN_WIDTH, 576 };

	//create chains
	int wall_main[84] = {
			212, 647,
			212, 640,
			324, 577,
			324, 510,
			324, 497,
			317, 487,
			306, 484,
			296, 483,
			288, 476,
			288, 440,
			309, 416,
			329, 377,
			338, 344,
			342, 311,
			340, 250,
			331, 211,
			314, 172,
			285, 143,
			246, 121,
			202, 110,
			155, 110,
			107, 123,
			66, 150,
			40, 183,
			25, 218,
			15, 260,
			14, 296,
			18, 343,
			28, 375,
			46, 409,
			56, 424,
			70, 441,
			70, 474,
			63, 482,
			52, 482,
			42, 485,
			36, 494,
			34, 508,
			34, 578,
			150, 641,
			148, 664,
			212, 664
	};

	int wall_left[20] = {
	137, 596,
	129, 604,
	68, 566,
	65, 562,
	65, 511,
	68, 507,
	73, 507,
	73, 549,
	77, 557,
	135, 594
	};

	int wall_right[20] = {
	231, 606,
	222, 598,
	280, 560,
	286, 553,
	286, 512,
	289, 508,
	294, 509,
	294, 562,
	288, 569,
	232, 606
	};

	int mart_wall[40] = {
	96, 313,
	80, 322,
	58, 286,
	49, 261,
	50, 245,
	61, 218,
	76, 196,
	100, 173,
	119, 160,
	120, 180,
	107, 192,
	106, 208,
	122, 245,
	143, 285,
	128, 293,
	108, 264,
	96, 260,
	83, 264,
	81, 276,
	94, 306
	};

	int island[86] = {
	245, 323,
	239, 329,
	232, 326,
	230, 314,
	232, 301,
	243, 294,
	243, 272,
	226, 263,
	215, 273,
	212, 268,
	216, 251,
	239, 227,
	248, 214,
	251, 199,
	239, 181,
	225, 177,
	211, 175,
	205, 172,
	205, 161,
	211, 150,
	250, 161,
	272, 178,
	283, 193,
	292, 210,
	301, 229,
	311, 249,
	314, 267,
	316, 284,
	316, 306,
	316, 316,
	308, 325,
	282, 327,
	276, 318,
	297, 291,
	307, 282,
	308, 267,
	293, 256,
	279, 258,
	265, 270,
	260, 280,
	257, 293,
	250, 306,
	247, 316
	};

	int mini_wall1[8] = {
	153, 180,
	143, 180,
	143, 153,
	153, 153
	};

	int mini_wall2[8] = {
	184, 178,
	174, 178,
	174, 149,
	184, 149
	};

	int entrance1[36] = {
	285, 367,
	294, 344,
	312, 315,
	315, 310,
	313, 284,
	288, 308,
	277, 327,
	271, 336,
	267, 345,
	265, 342,
	281, 312,
	311, 282,
	317, 277,
	318, 297,
	320, 310,
	305, 331,
	296, 348,
	297, 357
	};

	int entrance2[20] = {
	49, 334,
	66, 365,
	64, 367,
	53, 357,
	54, 348,
	44, 330,
	69, 314,
	84, 340,
	82, 341,
	71, 321
	};

	int lvl2[94] = {
	82, 340,
	71, 322,
	58, 294,
	47, 261,
	43, 240,
	40, 204,
	48, 164,
	60, 137,
	94, 107,
	132, 92,
	171, 93,
	206, 112,
	218, 135,
	224, 162,
	222, 175,
	246, 177,
	246, 146,
	240, 120,
	227, 102,
	202, 82,
	188, 75,
	219, 68,
	251, 64,
	293, 64,
	330, 70,
	348, 86,
	353, 108,
	352, 624,
	375, 624,
	375, 108,
	374, 79,
	363, 54,
	338, 40,
	295, 37,
	255, 37,
	215, 41,
	166, 51,
	126, 68,
	90, 81,
	54, 108,
	38, 129,
	24, 155,
	18, 202,
	18, 244,
	30, 293,
	44, 328,
	62, 363
	};

	walls.add(App->physics->CreateChain(0, 0, wall_main, 84));
	walls.add(App->physics->CreateChain(0, 0, wall_left, 20));
	walls.add(App->physics->CreateChain(0, 0, wall_right, 20));
	walls.add(App->physics->CreateChain(0, 0, mart_wall, 40));
	walls.add(App->physics->CreateChain(0, 0, island, 86));
	walls.add(App->physics->CreateChain(0, 0, mini_wall1, 8));
	walls.add(App->physics->CreateChain(0, 0, mini_wall2, 8));
	walls.add(App->physics->CreateChain(0, 0, entrance1, 36));
	walls.add(App->physics->CreateChain(0, 0, entrance2, 20));
	//walls.add(App->physics->CreateChain(0, 0, lvl2, 94));

	//SPRITE COORDINATES//
	
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
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		App->renderer->Blit(pinball, 350, 550, &(downSpoink.GetCurrentFrame()));
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->renderer->Blit(pinball, 348, 550, &(redSpoink.GetCurrentFrame()));
	}
	else
	{
		App->renderer->Blit(pinball, 350, 550, &(spoink.GetCurrentFrame()));
	}

	//Pikachu
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
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		pikaLap = 1;
		++pikaCount;
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		pikaCount = 0;
		pikaCharge.Reset();
		pikaCharge2.Reset();
		pikaCharge3.Reset();
	}

	//background 1 / 2
	App->renderer->Blit(pinball, 0, 0, &background2);
	App->renderer->Blit(pinball, 0, 0, &background1);
	
	//Zigzagoon
	App->renderer->Blit(pinball, 300, 425, &(zigzagoon.GetCurrentFrame()));

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

	
	
	//if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	//{
	//	ray_on = !ray_on;
	//	ray.x = App->input->GetMouseX();
	//	ray.y = App->input->GetMouseY();
	//}

	//if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	//{
	//	circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
	//	circles.getLast()->data->listener = this;
	//}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	
		// Pivot 0, 0
	
	

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------

	//draw ball
	int x, y;
	circles.getLast()->data->GetPosition(x, y);
	//App->renderer->Blit(pinball, x, y, NULL, 1.0f, circles.getLast()->data->GetRotation());

	p2List_item<PhysBody*>* c = circles.getFirst();

	/*while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}*/

	c = boxes.getFirst();

	/*while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}*/

	c = walls.getFirst();

	/*while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}*/

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
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
