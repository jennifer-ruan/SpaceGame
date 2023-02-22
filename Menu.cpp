//---------------------------------------------------------------------------------
// Menu.h
//---------------------------------------------------------------------------------
#include "stdafx.h"
#include <windows.h> 

#include "App/app.h"

#include "Collision.h"
#include "Menu.h"
//---------------------------------------------------------------------------------


CSimpleSprite* button;
CSimpleSprite* controls;
Collider playButton, helpButton, exitButton;

Menu::Menu() {
	App::PlaySound(".\\Assets\\Audio\\Music\\loading.wav", true);
	m_background = App::CreateSprite(".\\Assets\\Art\\menubg.bmp", 1, 1);
	m_background->SetPosition(310.0f, 360.0f);
	m_background->SetScale(0.75f);

	controls = App::CreateSprite(".\\Assets\\Art\\controls.bmp", 2, 2);
	controls->SetScale(2.5f);

	button = App::CreateSprite(".\\Assets\\Art\\buttons.bmp", 1, 2);
	button->SetScale(5.0f);

	playButton = CreateCollider(540, 580, 140, 60, BOUNDS);
	exitButton = CreateCollider(540, 480, 140, 60, BOUNDS);
}

void Menu::Update(float dt, CController controller) {
	m_background->Update(dt);

	float mouseX, mouseY;
	App::GetMousePos(mouseX, mouseY);
	if (GetMouseIsInBounds(playButton, mouseX, mouseY)) {
		m_selection = PLAY;
	}
	else if (GetMouseIsInBounds(exitButton, mouseX, mouseY)) {
		m_selection = EXIT;
	}
	else {
		m_selection = NONE;
	}

	if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0) {
		m_hasSelected = true;
	}
}

float x, y;
const char* text;

void Menu::Draw() {
	m_background->Draw();
	App::Print(440, 680, "You are a space traveller.");
	App::Print(180, 360, "Kill enemies and collect fallen scraps to upgrade your ship and travel to distant planets.");

	App::Print(260, 310, "Use WASD to move.");
	controls->SetPosition(340, 250);
	controls->SetFrame(2);
	controls->Draw();

	App::Print(640, 310, "Use arrow keys to shoot.");
	controls->SetPosition(740, 250);
	controls->SetFrame(0);
	controls->Draw();

	for (int i = 1; i < 3; i++) {
		float x, y;
		x = 520;
		y = 680 - 100 * i;

		if (m_selection == i) {
			button->SetFrame(1);
			y -= 12.0f;
		}
		else {
			button->SetFrame(0);
		}
		button->SetPosition(540, y);
		button->Draw();

		switch (i) {
		case PLAY:
			text = "PLAY";
			break;
		case EXIT:
			text = "EXIT";
			break;
		}
		App::Print(x, y, text);
	}
}

void Menu::Shutdown() {
	delete m_background;
	delete button;
	delete controls;
}