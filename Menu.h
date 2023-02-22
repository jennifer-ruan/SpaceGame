// Menu.h
//---------------------------------------------------------------------------------
#ifndef _MENU_H
#define _MENU_H
//---------------------------------------------------------------------------------
#include <windows.h> 
//---------------------------------------------------------------------------------

enum {
	NONE,
	PLAY,
	EXIT,
};

class Menu {
public:
	Menu();
	int GetMenuSelection() { if (m_hasSelected) { return m_selection; }; }
	void Update(float dt, CController controller);
	void Draw();
	void Shutdown();
private:
	CSimpleSprite* m_background;
	bool m_hasSelected = false;
	int m_selection = NONE;
};

#endif