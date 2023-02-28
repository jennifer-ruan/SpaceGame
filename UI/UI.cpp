//---------------------------------------------------------------------------------
// UI.h
//---------------------------------------------------------------------------------
#include "stdafx.h"
#include <windows.h> 
#include <math.h>  
//---------------------------------------------------------------------------------
#include "App/app.h"
#include "Collision.h"
#include "UI.h"
//---------------------------------------------------------------------------------

using namespace std;

CSimpleSprite* heart;
CSimpleSprite* scrap;
char score[5];

UI::UI(int dropsRequired) {
	m_dropsRequired = dropsRequired;

	heart = App::CreateSprite(".\\Assets\\Art\\Heart.bmp", 1, 1);
	heart->SetScale(2.0f);

	scrap = App::CreateSprite(".\\Assets\\Art\\Items.bmp", 16, 22);
	scrap->SetPosition(windowWidth - 16, windowHeight - 48);
	scrap->SetFrame(273);
	scrap->SetScale(1.0f);
}

void UI::Draw() {
	// Prints player health
	for (int i = 0; i < m_playerHealth; i++) {
		heart->SetPosition(windowWidth - (32 * i) - 16, windowHeight - 16);
		heart->Draw();
	}
	//Prints # of scraps acquired / # of scraps required
	scrap->Draw();
	sprintf(score, "%d/%d", m_dropsCollected, m_dropsRequired);
	App::Print(windowWidth - 76, windowHeight - 54, score);
}

void UI::Shutdown() {
	delete heart;
	delete scrap;
}