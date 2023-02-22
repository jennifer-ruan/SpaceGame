//---------------------------------------------------------------------------------
// UI.h
//---------------------------------------------------------------------------------
#ifndef _UI_H
#define _UI_H

class UI {
public:
	UI(int dropsRequired);
	void Update(float dt, int playerHealth, int dropsCollected) { m_playerHealth = playerHealth; m_dropsCollected = dropsCollected; };
	void Draw();
	void Shutdown();
private:
	int m_dropsRequired = 0;
	int m_playerHealth = 0;
	int m_dropsCollected = 0;
};


#endif