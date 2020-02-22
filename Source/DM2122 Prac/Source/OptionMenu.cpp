#include "OptionMenu.h"

#include "Application.h"
#include "StateManager.h"
#include "GL\glew.h"
#include <GLFW/glfw3.h>
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "shader.hpp"

extern bool isFullscreen;	// Try to use accessor instead
extern float bounceTime;	// Try to use accessor instead
extern GLFWwindow* m_window;	// Try to use accessor instead

OptionMenu* OptionMenu::instance = nullptr;

OptionMenu::OptionMenu()
{
	meshText = MeshBuilder::GenerateText("text", 16, 16);
	meshText->textureID = LoadTGA("image//font.tga");

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
}

OptionMenu::~OptionMenu()
{

}

OptionMenu* OptionMenu::getInstance()
{
	if (instance == nullptr)
		instance = new OptionMenu;
	return instance;
}

void OptionMenu::Update(double dt)
{
	if ((Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP)) && bounceTime <= 0)
	{
		if (menuSelected[MENU_OPTIONS_Y] != 0)
			menuSelected[MENU_OPTIONS_Y]--;
		else
			menuSelected[MENU_OPTIONS_Y] = 1;

		bounceTime = 0.25f;
	}
	if ((Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN)) && bounceTime <= 0)
	{
		if (menuSelected[MENU_OPTIONS_Y] != 1)
			menuSelected[MENU_OPTIONS_Y]++;
		else
			menuSelected[MENU_OPTIONS_Y] = 0;

		bounceTime = 0.25f;
	}
	else if (Application::IsKeyPressed(VK_RETURN) && menuSelected[MENU_OPTIONS_Y] == 0 && bounceTime <= 0)
	{
		int screenSizeX, screenSizeY;
		Application::GetScreenSize(screenSizeX, screenSizeY);

		if (isFullscreen)
		{
			isFullscreen = 0;
			glfwSetWindowMonitor(m_window, NULL, -2, 30, screenSizeX, screenSizeY, NULL);
		}
		else
		{
			isFullscreen = 1;
			glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), NULL, NULL, screenSizeX, screenSizeY, NULL);
		}

		bounceTime = 0.25f;
	}
	else if (Application::IsKeyPressed(VK_RETURN) && menuSelected[MENU_OPTIONS_Y] == 1 && bounceTime <= 0)
	{
		// State check for game?
		StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_MAINMENU);

		bounceTime = 0.25f;
	}

	if (bounceTime > 0) bounceTime -= dt;
}

void OptionMenu::Render()
{
	Color temp[2] = { Color(1,1,1), Color(1,1,1) };
	temp[menuSelected[MENU_OPTIONS_Y]] = Color(1, 0, 0);

	int Size[2] = { 3,3 };

	Size[menuSelected[MENU_OPTIONS_Y]] = 5;
	RenderTextOnScreen(meshText, "Toggle Fullscreen", temp[0], Size[0], 3, 15, 1);
	RenderTextOnScreen(meshText, "Back", temp[1], Size[1], 3, 7, 1);
}

void OptionMenu::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, int type)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check return; 
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI 
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode 
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack 
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		if (type == 0) characterSpacing.SetToTranslation(i * .5f - (text.length() * 0.5f) / 2, 0, 0); //1.0f is the spacing of each character, you may change this value 
		else characterSpacing.SetToTranslation(i * .5f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}