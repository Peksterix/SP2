#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include "Mesh.h"
#include <MatrixStack.h>

class OptionMenu
{
private:
	enum OPTIONSMENU
	{
		MENU_OPTIONS_X = 0,
		MENU_OPTIONS_Y,

		OPTIONS_TOTAL
	};

	enum TEXT_INPUT_TYPE
	{
		TEXT_PLAYER1 = 0,
		TEXT_PLAYER2,
		TEXT_PLAYER3,
		TEXT_PLAYER4,

		TEXT_TOTAL,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

	static OptionMenu* instance;

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	int menuSelected[OPTIONS_TOTAL],
		NumberOfPlayers,
		Player;
	bool isNameInput,
		isNameSelect;
	float Volume;
	MS modelStack, viewStack, projectionStack;
	Mesh* meshText;

	std::string textInput[TEXT_TOTAL]; // Players


public:
	OptionMenu();
	~OptionMenu();

	static OptionMenu* getInstance();

	void Update(double dt);
	void Render();
	void Keyboard();
	int getVolume();

	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, int type = 0);
};

#endif