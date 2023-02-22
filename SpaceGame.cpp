//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
#include <vector>
//------------------------------------------------------------------------
#include "app\app.h"
#include "Player.h"
#include "Menu.h"
#include "Level.h"
#include "UI.h"
//------------------------------------------------------------------------

enum
{
	MENU,
	GAME,
	LOST,
	WON,
};

Player* player;
Level* level;
Menu* menu;
UI* ui;

//Objects with collision
std::vector<Collider> levelColliders;
std::vector<Collider> playerColliders;
std::vector<Collider> obstacles;
std::vector<Collider>colliders;

//Game stats
int gameState = MENU;
int dropsRequired = 0;
int playerHealth = 0;
int dropsCollected = 0;
int planetsTraversed = 1;
float playerXSpawn = 600.0f;
float playerYSpawn = 400.0f;

void NewGame()
{
	srand(time(0)); //Re-seed the random number generator
	dropsRequired = rand() % 5 + 10; // Generate # of scraps required ( for the win condition)

	// The structure of objects in this game is as such:
	//   - The Player object generates and keeps track of the Player as well as any Projectiles they shoot.
	//   - The Level object generates and keeps track of the Map, Enemies, and Drops which the enemies create upon dying.
	// I did this to follow the Interface Segregation coding principle, which states that clients should not be forced
	// to depend on interfaces unless necessary.
	ui = new UI(dropsRequired);
	player = new Player(playerXSpawn, playerYSpawn);
	level = new Level();

	App::PlaySound(".\\Assets\\Audio\\Music\\meet-the-princess.wav", true);
}

void Init()
{
	menu = new Menu();
}

int menuSelection = 0;
float playerXPos, playerYPos;

void Update(float deltaTime)
{

	switch (gameState) {
	case GAME:
		// Get updated list of colliders with walls, projectiles, enemies, player and projectiles
		colliders = level->GetObstacles();
		playerColliders = player->GetColliders();
		levelColliders = level->GetColliders();
		colliders.insert(colliders.end(), playerColliders.begin(), playerColliders.end());
		colliders.insert(colliders.end(), levelColliders.begin(), levelColliders.end());

		// Check collisions against the player, projectiles and enemies
		player->GetPosition(playerXPos, playerYPos); // Here I get the player's x position and y position to pass into the level object. I do this rather than passing the object to reduce dependencies in Level.
		player->Input(App::GetController(), colliders);
		player->Update(deltaTime, colliders);
		level->Update(deltaTime, colliders, playerXPos, playerYPos);

		// Get updated UI info
		playerHealth = player->GetHealth();
		dropsCollected = player->GetNumDropsCollected();
		ui->Update(deltaTime, playerHealth, dropsCollected);

		// Check if the player has fulfilled the loss condition (player is not alive)
		if (!player->GetIsAlive()) {
			gameState = LOST;
			App::StopSound(".\\Assets\\Audio\\Music\\meet-the-princess.wav");
			App::PlaySound(".\\Assets\\Audio\\Effects\\Loss.wav");
		}

		// Check if the player has fulfilled the win condition (has retrieved required # of scraps)
		if (dropsCollected >= dropsRequired) {
			planetsTraversed += 1;
			App::StopSound(".\\Assets\\Audio\\Music\\meet-the-princess.wav");
			App::PlaySound(".\\Assets\\Audio\\Effects\\Victory.wav");
			gameState = WON;
		}
		break;
	case MENU:
		menu->Update(deltaTime, App::GetController());
		menuSelection = menu->GetMenuSelection();

		// Check for menu selections
		if (menuSelection == PLAY) {
			gameState = GAME;
			App::StopSound(".\\Assets\\Audio\\Music\\loading.wav");
			NewGame();
		}
		else if (menuSelection == EXIT) {
			std::terminate();
		}
		break;
	case LOST:
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
		{
			planetsTraversed = 1;
			gameState = GAME;
			NewGame();
		}
		break;
	case WON:
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
		{
			gameState = GAME;
			NewGame();
		}
		break;
	}
}

char planetStats[20];

void Render()
{
	switch (gameState) {
	case MENU:
		menu->Draw();
		break;
	case GAME:
		level->Draw();
		player->Draw();
		ui->Draw();
		break;
	case LOST:
		App::Print(460.0f, 450.0f, "YOU HAVE DIED.", 1.0, 1.0, 1.0, GLUT_BITMAP_9_BY_15);
		sprintf(planetStats, "You explored %d planets.", planetsTraversed);
		App::Print(420.0f, 420.0f, planetStats, 1.0, 1.0, 1.0, GLUT_BITMAP_9_BY_15);
		App::Print(800.0f, 20.0f, "Press SPACE to restart", 1.0, 1.0, 1.0, GLUT_BITMAP_9_BY_15);
		break;
	case WON:
		level->Draw();
		player->Draw();
		App::Print(460.0f, 450.0f, "YOU WIN!", 1.0, 1.0, 1.0, GLUT_BITMAP_9_BY_15);
		App::Print(350.0f, 400.0f, "Press SPACE to go to the next planet", 1.0, 1.0, 1.0, GLUT_BITMAP_9_BY_15);
		break;
	}
}

void Shutdown()
{
	player->Shutdown();
	level->Shutdown();
	ui->Shutdown();
	menu->Shutdown();
	delete player;
	delete level;
	delete menu;
	delete ui;
}