#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"


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
	bonus_fx = App->audio->LoadFx("audio/bonus.wav");

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	background = { 803, 6, SCREEN_WIDTH, SCREEN_HEIGHT };
	background1 = { 18, 6, SCREEN_WIDTH, 344 };
	background2 = { 410, 6, SCREEN_WIDTH, 576 };

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
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		pikachu_pos = 295;
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

	//if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	//{
	//	boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	//}

	//if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	//{
	//	// Pivot 0, 0
	//	int rick_head[64] = {
	//		14, 36,
	//		42, 40,
	//		40, 0,
	//		75, 30,
	//		88, 4,
	//		94, 39,
	//		111, 36,
	//		104, 58,
	//		107, 62,
	//		117, 67,
	//		109, 73,
	//		110, 85,
	//		106, 91,
	//		109, 99,
	//		103, 104,
	//		100, 115,
	//		106, 121,
	//		103, 125,
	//		98, 126,
	//		95, 137,
	//		83, 147,
	//		67, 147,
	//		53, 140,
	//		46, 132,
	//		34, 136,
	//		38, 126,
	//		23, 123,
	//		30, 114,
	//		10, 102,
	//		29, 90,
	//		0, 75,
	//		30, 62
	//	};

	//	ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	//}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
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

	while(c != NULL)
	{
		/*int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;*/
	}

	c = ricks.getFirst();

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
