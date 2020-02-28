#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Player.h"
#include "SaveManager.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();

	static void GetScreenSize(int &x, int &y);
	static bool IsKeyPressed(unsigned short key);

	static float getBounceTime();
	static void setBounceTime(float bt);
	static bool getFullscreen();
	static void setFullscreen(bool fullscreen);
	static int getPlayerNum();
	static void setPlayerNum(int num);
	static GLFWwindow* getWindow();
	static Player* getPlayer(int num);

private:

	//Declare a window object
	StopWatch m_timer;

	static GLFWwindow* m_window;
	static Player* player[4];
	static bool isFullscreen;
	static int playerNum;
	static float bounceTime;
	SaveManager save;

};

#endif