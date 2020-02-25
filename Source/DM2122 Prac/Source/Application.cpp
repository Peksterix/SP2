//Include GLEW

//Include GLFW

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include "wtypes.h"

#include "Application.h"

#include "StateManager.h"

static bool windowActive = 1;
static int	screenSize_x, screenSize_y;

const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

bool Application::isFullscreen = 0;
int Application::playerNum = 1;
float Application::bounceTime = 0;
GLFWwindow* Application::m_window = NULL;
Player* Application::player[4];

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	//update opengl new window size
	glViewport(0, 0, w, h);
}

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


bool Application::IsKeyPressed(unsigned short key)
{
	if (windowActive) return ((GetAsyncKeyState(key) & 0x8001) != 0);
	else return 0;
}

void Application::GetScreenSize(int& x, int& y)
{
	x = screenSize_x;
	y = screenSize_y;
}
;
Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	GetDesktopResolution(screenSize_x, screenSize_y);

	//Create a window and create its OpenGL context
	//m_window = glfwCreateWindow(screenSize_x, screenSize_y, "Computer Graphics Project", glfwGetPrimaryMonitor(), NULL);
	m_window = glfwCreateWindow(screenSize_x, screenSize_y, "Computer Graphics Project", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	glfwSetWindowSizeCallback(m_window, resize_callback);

	for (int i = 0; i < 4; ++i) player[i] = new Player;
	player[0]->setName("Player 1");
	player[0]->setInput(player[0]->UP, 'W');
	player[0]->setInput(player[0]->DOWN, 'S');
	player[0]->setInput(player[0]->LEFT, 'A');
	player[0]->setInput(player[0]->RIGHT, 'D');
	player[0]->setInput(player[0]->ENTER, 'Z');

	player[1]->setName("Player 2");
	player[1]->setInput(player[1]->UP, 'T');
	player[1]->setInput(player[1]->DOWN, 'G');
	player[1]->setInput(player[1]->LEFT, 'F');
	player[1]->setInput(player[1]->RIGHT, 'H');
	player[1]->setInput(player[1]->ENTER, 'V');
	
	player[2]->setName("Player 3");
	player[2]->setInput(player[2]->UP, 'I');
	player[2]->setInput(player[2]->DOWN, 'K');
	player[2]->setInput(player[2]->LEFT, 'J');
	player[2]->setInput(player[2]->RIGHT, 'L');
	player[2]->setInput(player[2]->ENTER, 'M');
	
	player[3]->setName("Player 4");
	player[3]->setInput(player[3]->UP, VK_UP);
	player[3]->setInput(player[3]->DOWN, VK_DOWN);
	player[3]->setInput(player[3]->LEFT, VK_LEFT);
	player[3]->setInput(player[3]->RIGHT, VK_RIGHT);
	player[3]->setInput(player[3]->ENTER, VK_RETURN);
}

void Application::Run()
{
	//Main Loop

	//StateManager::getInstance()->setScene(StateManager::SCENE_STATES::SS_TEST);
	StateManager::getInstance()->setScene(StateManager::SCENE_STATES::SS_MAINMENU);

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window))
	{
		//windowActive = GetActiveWindow();
		
		if (bounceTime > 0) bounceTime -= m_timer.getElapsedTime();

		StateManager::getInstance()->getScene()->Update(m_timer.getElapsedTime());
		StateManager::getInstance()->getScene()->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //If the window had been closed

	StateManager::getInstance()->getScene()->Exit();
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();

	for (int i = 0; i < 4; ++i) delete player[i];
}

float Application::getBounceTime()
{
	return bounceTime;
}

void Application::setBounceTime(float bt)
{
	bounceTime = bt;
}

bool Application::getFullscreen()
{
	return isFullscreen;
}

void Application::setFullscreen (bool fullscreen)
{
	isFullscreen = fullscreen;
}

int Application::getPlayerNum()
{
	return playerNum;
}

void Application::setPlayerNum(int num)
{
	playerNum = num;
}

GLFWwindow* Application::getWindow()
{
	return m_window;
}

Player* Application::getPlayer(int num)
{
	return player[num];
}
