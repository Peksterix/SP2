#include "GameScene0.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <string>
#include <MyMath.h>
#include <conio.h>
#include <GLFW/glfw3.h>
#include "StateManager.h"
#include "OptionMenu.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

static float framesPerSecond;
static int fps;
static float lastTime;

GameScene0::GameScene0()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

GameScene0::~GameScene0()
{
}

void GameScene0::Exit()
{
	// Cleanup here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}

void GameScene0::Init()
{

	#pragma region Initialise OpenGL

		//glClearColor(68.f / 255.f, 235.f / 255.f, 201.f / 255.f, 0.0f);
		glClearColor(1, 1, 1, 0);

		// Generate a default VAO for now
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);
		glEnable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Mtx44 projection;
		projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
		projectionStack.LoadMatrix(projection);

		m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

		//m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader"); 

		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
		m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
		m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
		m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
		m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
		m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

		m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
		m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
		m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
		m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
		m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
		m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
		m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
		m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
		m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
		m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
		m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

		m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
		m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
		m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
		m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
		m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
		m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
		m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
		m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
		m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
		m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
		m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

		m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
		m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
		m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
		m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
		m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
		m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
		m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
		m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
		m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
		m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
		m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

		m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
		m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
		m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
		m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
		m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
		m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
		m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
		m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
		m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
		m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
		m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");

		m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
		m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
		m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
		m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
		m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
		m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
		m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
		m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
		m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
		m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
		m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID, "lights[4].exponent");

		m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
		m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
		m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
		m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
		m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
		m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
		m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
		m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
		m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
		m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
		m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID, "lights[4].exponent");

		m_parameters[U_LIGHT5_POSITION] = glGetUniformLocation(m_programID, "lights[5].position_cameraspace");
		m_parameters[U_LIGHT5_COLOR] = glGetUniformLocation(m_programID, "lights[5].color");
		m_parameters[U_LIGHT5_POWER] = glGetUniformLocation(m_programID, "lights[5].power");
		m_parameters[U_LIGHT5_KC] = glGetUniformLocation(m_programID, "lights[5].kC");
		m_parameters[U_LIGHT5_KL] = glGetUniformLocation(m_programID, "lights[5].kL");
		m_parameters[U_LIGHT5_KQ] = glGetUniformLocation(m_programID, "lights[5].kQ");
		m_parameters[U_LIGHT5_TYPE] = glGetUniformLocation(m_programID, "lights[5].type");
		m_parameters[U_LIGHT5_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[5].spotDirection");
		m_parameters[U_LIGHT5_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[5].cosCutoff");
		m_parameters[U_LIGHT5_COSINNER] = glGetUniformLocation(m_programID, "lights[5].cosInner");
		m_parameters[U_LIGHT5_EXPONENT] = glGetUniformLocation(m_programID, "lights[5].exponent");

		m_parameters[U_LIGHT6_POSITION] = glGetUniformLocation(m_programID, "lights[6].position_cameraspace");
		m_parameters[U_LIGHT6_COLOR] = glGetUniformLocation(m_programID, "lights[6].color");
		m_parameters[U_LIGHT6_POWER] = glGetUniformLocation(m_programID, "lights[6].power");
		m_parameters[U_LIGHT6_KC] = glGetUniformLocation(m_programID, "lights[6].kC");
		m_parameters[U_LIGHT6_KL] = glGetUniformLocation(m_programID, "lights[6].kL");
		m_parameters[U_LIGHT6_KQ] = glGetUniformLocation(m_programID, "lights[6].kQ");
		m_parameters[U_LIGHT6_TYPE] = glGetUniformLocation(m_programID, "lights[6].type");
		m_parameters[U_LIGHT6_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[6].spotDirection");
		m_parameters[U_LIGHT6_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[6].cosCutoff");
		m_parameters[U_LIGHT6_COSINNER] = glGetUniformLocation(m_programID, "lights[6].cosInner");
		m_parameters[U_LIGHT6_EXPONENT] = glGetUniformLocation(m_programID, "lights[6].exponent");

		m_parameters[U_LIGHT7_POSITION] = glGetUniformLocation(m_programID, "lights[7].position_cameraspace");
		m_parameters[U_LIGHT7_COLOR] = glGetUniformLocation(m_programID, "lights[7].color");
		m_parameters[U_LIGHT7_POWER] = glGetUniformLocation(m_programID, "lights[7].power");
		m_parameters[U_LIGHT7_KC] = glGetUniformLocation(m_programID, "lights[7].kC");
		m_parameters[U_LIGHT7_KL] = glGetUniformLocation(m_programID, "lights[7].kL");
		m_parameters[U_LIGHT7_KQ] = glGetUniformLocation(m_programID, "lights[7].kQ");
		m_parameters[U_LIGHT7_TYPE] = glGetUniformLocation(m_programID, "lights[7].type");
		m_parameters[U_LIGHT7_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[7].spotDirection");
		m_parameters[U_LIGHT7_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[7].cosCutoff");
		m_parameters[U_LIGHT7_COSINNER] = glGetUniformLocation(m_programID, "lights[7].cosInner");
		m_parameters[U_LIGHT7_EXPONENT] = glGetUniformLocation(m_programID, "lights[7].exponent");

		m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
		m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

		m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
		m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
		m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
		m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

		glUseProgram(m_programID);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
	
	#pragma endregion

	#pragma region Initialise Lights
		const int point_power = 2;

		light[0].type = Light::LIGHT_POINT;
		light[0].position.Set(0, 0, 0);
		light[0].color.Set(1, 1, 1);
		light[0].power = point_power;
		light[0].kC = 1.f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(45));
		light[0].cosInner = cos(Math::DegreeToRadian(30));
		light[0].exponent = 3.f;
		light[0].spotDirection.Set(0.f, 1.f, 0.f);
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &light[0].spotDirection.x);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

		light[1].type = Light::LIGHT_POINT;
		light[1].position.Set(0, 0, 0);
		light[1].color.Set(1, 1, 1);
		light[1].power = point_power;
		light[1].kC = 1.f;
		light[1].kL = 0.01f;
		light[1].kQ = 0.001f;
		light[1].cosCutoff = cos(Math::DegreeToRadian(45));
		light[1].cosInner = cos(Math::DegreeToRadian(30));
		light[1].exponent = 3.f;
		light[1].spotDirection.Set(.25f, 1.f, .25f);
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
		glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
		glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &light[1].spotDirection.x);
		glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
		glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

		light[2].type = Light::LIGHT_POINT;
		light[2].position.Set(0, 0, 0);
		light[2].color.Set(1, 1, 1);
		light[2].power = point_power;
		light[2].kC = 1.f;
		light[2].kL = 0.01f;
		light[2].kQ = 0.001f;
		light[2].cosCutoff = cos(Math::DegreeToRadian(45));
		light[2].cosInner = cos(Math::DegreeToRadian(30));
		light[2].exponent = 3.f;
		light[2].spotDirection.Set(-1.f, 0.f, 0.f);
		glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
		glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
		glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
		glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
		glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &light[2].spotDirection.x);
		glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
		glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

		light[3].type = Light::LIGHT_POINT;
		light[3].position.Set(0, 0, 0);
		light[3].color.Set(1, 1, 1);
		light[3].power = point_power;
		light[3].kC = 1.f;
		light[3].kL = 0.01f;
		light[3].kQ = 0.001f;
		light[3].cosCutoff = cos(Math::DegreeToRadian(45));
		light[3].cosInner = cos(Math::DegreeToRadian(30));
		light[3].exponent = 3.f;
		light[3].spotDirection.Set(1.f, 0.f, 0.f);
		glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
		glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
		glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
		glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
		glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
		glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
		glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &light[3].spotDirection.x);
		glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], light[3].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT3_COSINNER], light[3].cosInner);
		glUniform1f(m_parameters[U_LIGHT3_EXPONENT], light[3].exponent);

		light[4].type = Light::LIGHT_POINT;
		light[4].position.Set(0, 0, 0);
		light[4].color.Set(1, 1, 1);
		light[4].power = point_power;
		light[4].kC = 1.f;
		light[4].kL = 0.01f;
		light[4].kQ = 0.001f;
		light[4].cosCutoff = cos(Math::DegreeToRadian(45));
		light[4].cosInner = cos(Math::DegreeToRadian(30));
		light[4].exponent = 3.f;
		light[4].spotDirection.Set(0.f, 1.f, 0.f);
		glUniform1i(m_parameters[U_LIGHT4_TYPE], light[4].type);
		glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &light[4].color.r);
		glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
		glUniform1f(m_parameters[U_LIGHT4_KC], light[4].kC);
		glUniform1f(m_parameters[U_LIGHT4_KL], light[4].kL);
		glUniform1f(m_parameters[U_LIGHT4_KQ], light[4].kQ);
		glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, &light[4].spotDirection.x);
		glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], light[4].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT4_COSINNER], light[4].cosInner);
		glUniform1f(m_parameters[U_LIGHT4_EXPONENT], light[4].exponent);

		light[5].type = Light::LIGHT_POINT;
		light[5].position.Set(0, 0, 0);
		light[5].color.Set(1, 1, 1);
		light[5].power = point_power;
		light[5].kC = 1.f;
		light[5].kL = 0.01f;
		light[5].kQ = 0.001f;
		light[5].cosCutoff = cos(Math::DegreeToRadian(45));
		light[5].cosInner = cos(Math::DegreeToRadian(30));
		light[5].exponent = 3.f;
		light[5].spotDirection.Set(0.f, 1.f, 0.f);
		glUniform1i(m_parameters[U_LIGHT5_TYPE], light[5].type);
		glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &light[5].color.r);
		glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
		glUniform1f(m_parameters[U_LIGHT5_KC], light[5].kC);
		glUniform1f(m_parameters[U_LIGHT5_KL], light[5].kL);
		glUniform1f(m_parameters[U_LIGHT5_KQ], light[5].kQ);
		glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, &light[5].spotDirection.x);
		glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], light[5].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT5_COSINNER], light[5].cosInner);
		glUniform1f(m_parameters[U_LIGHT5_EXPONENT], light[5].exponent);

		light[6].type = Light::LIGHT_DIRECTIONAL;
		light[6].position.Set(0, 10, 0);
		light[6].color.Set(1, 1, 1);
		light[6].power = 0.5f;
		light[6].kC = 1.f;
		light[6].kL = 0.01f;
		light[6].kQ = 0.001f;
		light[6].cosCutoff = cos(Math::DegreeToRadian(45));
		light[6].cosInner = cos(Math::DegreeToRadian(30));
		light[6].exponent = 3.f;
		light[6].spotDirection.Set(1.f, 1.f, 1.f);
		glUniform1i(m_parameters[U_LIGHT6_TYPE], light[6].type);
		glUniform3fv(m_parameters[U_LIGHT6_COLOR], 1, &light[6].color.r);
		glUniform1f(m_parameters[U_LIGHT6_POWER], light[6].power);
		glUniform1f(m_parameters[U_LIGHT6_KC], light[6].kC);
		glUniform1f(m_parameters[U_LIGHT6_KL], light[6].kL);
		glUniform1f(m_parameters[U_LIGHT6_KQ], light[6].kQ);
		glUniform3fv(m_parameters[U_LIGHT6_SPOTDIRECTION], 1, &light[6].spotDirection.x);
		glUniform1f(m_parameters[U_LIGHT6_COSCUTOFF], light[6].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT6_COSINNER], light[6].cosInner);
		glUniform1f(m_parameters[U_LIGHT6_EXPONENT], light[6].exponent);

		light[7].type = Light::LIGHT_SPOT;
		light[7].position.Set(-5, 15, -5);
		light[7].color.Set(1, 1, 1);
		light[7].power = 3;
		light[7].kC = 1.f;
		light[7].kL = 0.01f;
		light[7].kQ = 0.001f;
		light[7].cosCutoff = cos(Math::DegreeToRadian(45));
		light[7].cosInner = cos(Math::DegreeToRadian(30));
		light[7].exponent = 3.f;
		light[7].spotDirection.Set(-.5f, 1.f, -.5f);
		glUniform1i(m_parameters[U_LIGHT7_TYPE], light[7].type);
		glUniform3fv(m_parameters[U_LIGHT7_COLOR], 1, &light[7].color.r);
		glUniform1f(m_parameters[U_LIGHT7_POWER], light[7].power);
		glUniform1f(m_parameters[U_LIGHT7_KC], light[7].kC);
		glUniform1f(m_parameters[U_LIGHT7_KL], light[7].kL);
		glUniform1f(m_parameters[U_LIGHT7_KQ], light[7].kQ);
		glUniform3fv(m_parameters[U_LIGHT7_SPOTDIRECTION], 1, &light[7].spotDirection.x);
		glUniform1f(m_parameters[U_LIGHT7_COSCUTOFF], light[7].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT7_COSINNER], light[7].cosInner);
		glUniform1f(m_parameters[U_LIGHT7_EXPONENT], light[7].exponent);

		glUniform1i(m_parameters[U_NUMLIGHTS], 8);

	#pragma endregion

	#pragma region Initialise Camera
		
		camera[0].Init(Position(-20 + 130 , 12, -26), Position(0 + 130, 4, -8), Position(0, 1, 0));

	#pragma endregion

	#pragma region Initialise Mesh
	
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		meshList[GEO_TEXT]->textureID = LoadTGA("image//font.tga");
		meshList[GEO_UI] = MeshBuilder::GenerateText("UI", 16, 8);
		meshList[GEO_UI]->textureID = LoadTGA("image//UI Sheet.tga");
		
		meshList[GEO_SKYSPHERE] = MeshBuilder::GenerateOBJ("Skysphere (Space)", "obj//Skysphere (Half).obj");
		meshList[GEO_SKYSPHERE]->textureID = LoadTGA("image//Flat Space2.tga");
		meshList[GEO_FLOOR] = MeshBuilder::GenerateOBJ("Floor", "obj//Floor.obj");
		meshList[GEO_FLOOR]->textureID = LoadTGA("image//Vehicle3a.tga");

		meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 8, 16, 1.f);
	
	#pragma endregion

	#pragma region Initialise Values

		Application::GetScreenSize(screenSizeX, screenSizeY);
		
		showDebugInfo = 1;
		showBoundingBox = 0;
		inWindow = WINDOW_NONE;

		for (int i = 0; i < DEBUG_TOTAL; ++i) debugValues[i] = 0;

	#pragma endregion
}

void GameScene0::Update(double dt)
{
	CalculateFPS();

	// Debug Info
	if (showDebugInfo)
	{
		if (Application::IsKeyPressed('7')) glDisable(GL_CULL_FACE);					// Disable Cull
		if (Application::IsKeyPressed('8')) glEnable(GL_CULL_FACE);						// Enable Cull
		if (Application::IsKeyPressed('9')) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Normal
		if (Application::IsKeyPressed('0')) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Wireframe
		if (Application::IsKeyPressed('X'))
		{
			StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_FREECAM);
			camera[0].Update(0.01, Math::RadianToDegree(atan2(camera[0].target.x - camera[0].position.x, camera[0].target.z - camera[0].position.z)),
				Math::RadianToDegree(atan2(camera[0].target.y - camera[0].position.y, sqrt(pow(camera[0].position.x - camera[0].target.x, 2) + pow(camera[0].position.z - camera[0].target.z, 2)))));
		}
	}

	// Repeat for each Player
	for (int i = 0; i < Application::getPlayerNum(); ++i)
	{
		Player* tempPlayer = Application::getPlayer(i);
		Vehicle* tempVehicle = tempPlayer->getVehicle();
		
		//**************NEW*************
		if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_GAME)
		{
			for (int j = 0; j < 5; ++ j) debugValues[DEBUG_PLAYER0_UP + i * 5 + j] = 0;

			if (Application::IsKeyPressed(tempPlayer->getInput(tempPlayer->UP)))
			{
				// && Application::getBounceTime() <= 0
				//tempVehicle->position.z += dt * 10;
				
				// +Z is foward
				//Vector3 newForce = ;
	
				//tempVehicle->position.x -= dt * 20 * cos(Math::DegreeToRadian(tempVehicle->rotate.y + 90.f));
				//tempVehicle->position.z += dt * 20 * sin(Math::DegreeToRadian(tempVehicle->rotate.y + 90.f));
				/*Vector3 newForce = phys.getForce().Length();
				
				if (newForce.Length() < maxSpeed)
				{
					Vector3 aForce = accel * phys.getFront();
				}*/

				Vector3 force = tempVehicle->getRB()->getForce().Length();

				if (force.Length() < tempVehicle->maxSpeed)
				{
					Vector3 newForce = tempVehicle->accel * tempVehicle->RB.getFront();
					//tempVehicle->RB.addForce(newForce);
					tempVehicle->RB.setVelo(newForce);
				}

				if (i == 0) debugValues[DEBUG_PLAYER0_UP] = 1;
				if (i == 1) debugValues[DEBUG_PLAYER1_UP] = 1;
			}

			if (Application::IsKeyPressed(tempPlayer->getInput(tempPlayer->DOWN)))
			{
				//tempVehicle->position.z -= dt * 10;
				
				/*tempVehicle->position.x += dt * 20 * cos(Math::DegreeToRadian(tempVehicle->rotate.y + 90.f));
				tempVehicle->position.z -= dt * 20 * sin(Math::DegreeToRadian(tempVehicle->rotate.y + 90.f));*/

				Vector3 force = tempVehicle->getRB()->getForce().Length();

				if (force.Length() < tempVehicle->maxSpeed)
				{
					Vector3 newForce = -tempVehicle->accel * tempVehicle->RB.getFront();
					//tempVehicle->RB.addForce(newForce);
					tempVehicle->RB.setVelo(newForce);
				}

				if (i == 0) debugValues[DEBUG_PLAYER0_DOWN] = 1;
				if (i == 1) debugValues[DEBUG_PLAYER1_DOWN] = 1;
			}
			if (Application::IsKeyPressed(tempPlayer->getInput(tempPlayer->LEFT)))
			{
				//tempVehicle->position.x += dt * 10;

				// +X is Left
				//tempVehicle->rotate.y += dt * 30;

				if (tempVehicle->RB.getVelo().Length() < 0.5f)
				{
					return;
				}
				if (tempVehicle->vehTurningSpeed < tempVehicle->cMaxTurningSpeed)
				{
					tempVehicle->vehTurningSpeed += tempVehicle->cTurningSpeedRate;
				}

				Mtx44 rotate;
				tempVehicle->turningAngle = (tempVehicle->vehTurningSpeed * 1) * tempVehicle->RB.getVelo().Length();
				//TODO: Add rotation for reversing(DONE)

				rotate.SetToRotation(tempVehicle->turningAngle, 0, 1, 0);
				//front rotation
				Vector3 rotFront = rotate * tempVehicle->RB.getFront();
				rotFront.Normalize();
				tempVehicle->RB.setFront(rotFront);

				tempVehicle->RB.setVelo(rotate * tempVehicle->RB.getVelo());

				//rotate mesh
				tempVehicle->rotate.y += tempVehicle->turningAngle;

				if (i == 0) debugValues[DEBUG_PLAYER0_LEFT] = 1;
				if (i == 1) debugValues[DEBUG_PLAYER1_LEFT] = 1;
			}
			if (Application::IsKeyPressed(tempPlayer->getInput(tempPlayer->RIGHT)))
			{
				//tempVehicle->position.x -= dt * 10;
				//tempVehicle->rotate.y -= dt * 30;

				if (tempVehicle->RB.getVelo().Length() < 0.5f)
				{
					return;
				}
				if (tempVehicle->vehTurningSpeed < tempVehicle->cMaxTurningSpeed)
				{
					tempVehicle->vehTurningSpeed += tempVehicle->cTurningSpeedRate;
				}

				Mtx44 rotate;
				tempVehicle->turningAngle = (tempVehicle->vehTurningSpeed * -1) * tempVehicle->RB.getVelo().Length();
				//TODO: Add rotation for reversing(DONE)

				rotate.SetToRotation(tempVehicle->turningAngle, 0, 1, 0);
				//front rotation
				Vector3 rotFront = rotate * tempVehicle->RB.getFront();
				rotFront.Normalize();
				tempVehicle->RB.setFront(rotFront);

				tempVehicle->RB.setVelo(rotate * tempVehicle->RB.getVelo());

				//rotate mesh
				tempVehicle->rotate.y += tempVehicle->turningAngle;

				if (i == 0) debugValues[DEBUG_PLAYER0_RIGHT] = 1;
				if (i == 1) debugValues[DEBUG_PLAYER1_RIGHT] = 1;
			}
			if (Application::IsKeyPressed(tempPlayer->getInput(tempPlayer->ENTER)))
			{

			}

			tempVehicle->updatePos();
			camera[i].Update(dt, tempVehicle, Position(0, 16, -40), Position(0, 8, 4));
		}
		else if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_OPTIONS)
		{

		}
	}
	//**************END*************

	if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_FREECAM)
	{
		camera[0].Update(dt, 0, 0);
	}

	// Animation Logic
	if (fps >= 40 && StateManager::getInstance()->getSceneState() == StateManager::SCENE_STATES::SS_MAP0) animation[ANIS_ANY] = animate.Update(0.02);

	if (!animation[ANIS_ANY])
		for (int i = 0; i < ANIS_TOTAL; ++i)
			animation[i] = 0;
}

void GameScene0::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Viewport
	if (Application::getPlayerNum() == 4)
	{
		// Top Left
		glViewport(0, screenSizeY / 2, screenSizeX / 2, screenSizeY / 2);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[0].position.x, camera[0].position.y, camera[0].position.z, camera[0].target.x, camera[0].target.y, camera[0].target.z, camera[0].up.x, camera[0].up.y, camera[0].up.z);
		modelStack.LoadIdentity();
		CalculateLights();
		renderScene();

		// Top Right
		glViewport(screenSizeX / 2, screenSizeY / 2, screenSizeX / 2, screenSizeY / 2);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[1].position.x, camera[1].position.y, camera[1].position.z, camera[1].target.x, camera[1].target.y, camera[1].target.z, camera[1].up.x, camera[1].up.y, camera[1].up.z);
		modelStack.LoadIdentity();
		CalculateLights();
		renderScene();

		// Bottom Left
		glViewport(0, 0, screenSizeX / 2, screenSizeY / 2);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[2].position.x, camera[2].position.y, camera[2].position.z, camera[2].target.x, camera[2].target.y, camera[2].target.z, camera[2].up.x, camera[2].up.y, camera[2].up.z);
		modelStack.LoadIdentity();
		CalculateLights();
		renderScene();

		// Bottom Right
		glViewport(screenSizeX / 2, 0, screenSizeX / 2, screenSizeY / 2);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[3].position.x, camera[3].position.y, camera[3].position.z, camera[3].target.x, camera[3].target.y, camera[3].target.z, camera[3].up.x, camera[3].up.y, camera[3].up.z);
		modelStack.LoadIdentity();
		CalculateLights();
		renderScene();
	}
	else if (Application::getPlayerNum() == 3)
	{
		// Top
		glViewport(0, 2 * (screenSizeY / 3), screenSizeX, screenSizeY / 3);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[0].position.x, camera[0].position.y, camera[0].position.z, camera[0].target.x, camera[0].target.y, camera[0].target.z, camera[0].up.x, camera[0].up.y, camera[0].up.z);
		modelStack.LoadIdentity();
		CalculateLights();
		renderScene();

		// Middle
		glViewport(0, screenSizeY / 3, screenSizeX, screenSizeY / 3);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[1].position.x, camera[1].position.y, camera[1].position.z, camera[1].target.x, camera[1].target.y, camera[1].target.z, camera[1].up.x, camera[1].up.y, camera[1].up.z);
		modelStack.LoadIdentity();
		CalculateLights();
		renderScene();

		// Bottom
		glViewport(0, 0, screenSizeX, screenSizeY / 3);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[2].position.x, camera[2].position.y, camera[2].position.z, camera[2].target.x, camera[2].target.y, camera[2].target.z, camera[2].up.x, camera[2].up.y, camera[2].up.z);
		modelStack.LoadIdentity();
		CalculateLights();
		renderScene();
	}
	else if (Application::getPlayerNum() == 2)
	{
		// Top
		glViewport(0, screenSizeY / 2, screenSizeX, screenSizeY / 2);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[0].position.x, camera[0].position.y, camera[0].position.z, camera[0].target.x, camera[0].target.y, camera[0].target.z, camera[0].up.x, camera[0].up.y, camera[0].up.z);
		modelStack.LoadIdentity();
		CalculateLights();
		renderScene();

		// Bottom
		glViewport(0, 0, screenSizeX, screenSizeY / 2);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[1].position.x, camera[1].position.y, camera[1].position.z, camera[1].target.x, camera[1].target.y, camera[1].target.z, camera[1].up.x, camera[1].up.y, camera[1].up.z);
		modelStack.LoadIdentity();
		CalculateLights();
		renderScene();
	}
	else
	{
		// Full window	
		glViewport(0, 0, screenSizeX, screenSizeY);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[0].position.x, camera[0].position.y, camera[0].position.z, camera[0].target.x, camera[0].target.y, camera[0].target.z, camera[0].up.x, camera[0].up.y, camera[0].up.z);
		modelStack.LoadIdentity();
		CalculateLights();
		renderScene();
	}
}

void GameScene0::renderScene()
{
	modelStack.PushMatrix();
	
	if (Application::getPlayerNum() == 1 || Application::getPlayerNum() == 4)
		modelStack.Scale(1, 1, 1);
	else
		modelStack.Scale(1, Application::getPlayerNum(), 1);

	renderSkysphere(100);

	// Render Vehicles
	for (int i = 0; i < Application::getPlayerNum(); ++i)
	{
		Vehicle* tempVehicle = Application::getPlayer(i)->getVehicle();

		modelStack.PushMatrix();
		modelStack.Translate(tempVehicle->position.x, tempVehicle->position.y + tempVehicle->getChassis()->getMesh()->size.y / 2, tempVehicle->position.z);
		modelStack.Rotate(tempVehicle->rotate.z, 0, 0, 1);
		modelStack.Rotate(tempVehicle->rotate.y, 0, 1, 0);
		modelStack.Rotate(tempVehicle->rotate.x, 1, 0, 0);

		// Render Chassis
		RenderMesh(tempVehicle->getChassis()->getMesh(), true, tempVehicle->getChassis()->getBBScale() * showBoundingBox);
		
		// Render Wheels
		for (int i = 0; i < tempVehicle->getChassis()->wheelPos.size(); ++i)
		{
			Position tempWheelPos[2];
			tempWheelPos[0] = tempVehicle->wheelPos[i];
			tempWheelPos[1] = tempVehicle->wheelScale[i];
			modelStack.PushMatrix();
			modelStack.Translate(tempWheelPos[0].x, tempWheelPos[0].y, tempWheelPos[0].z);
			//modelStack.Rotate(aniVal[ANI_WHEEL_ROTATION] * 2 + aniVal[ANI_VEHICLE_POSITION] * 2, 1, 0, 0);
			modelStack.Scale(tempWheelPos[1].x, tempWheelPos[1].y, tempWheelPos[1].z);
			RenderMesh(tempVehicle->getWheel()->getMesh(), true, tempVehicle->getWheel()->getBBScale() * showBoundingBox);
			modelStack.PopMatrix();
		}

		// Render Weapons
		for (int i = 0; i < tempVehicle->getChassis()->weaponPos.size(); ++i)
		{
			Position tempWeaponPos[2];
			tempWeaponPos[0] = tempVehicle->weaponPos[i];
			tempWeaponPos[1] = tempVehicle->weaponScale[i];
			modelStack.PushMatrix();
			modelStack.Translate(tempWeaponPos[0].x, tempWeaponPos[0].y, tempWeaponPos[0].z);
			modelStack.Scale(tempWeaponPos[1].x, tempWeaponPos[1].y, tempWeaponPos[1].z);
			RenderMesh(tempVehicle->getWeapon()->getMesh(), true, tempVehicle->getWeapon()->getBBScale() * showBoundingBox);
			modelStack.PopMatrix();
		}

		modelStack.PopMatrix();
	}
	
	// Render Options Text
	if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_OPTIONS)
	{
		OptionMenu::getInstance()->Render();
	}

	// Render Confirmation Window
	if (inWindow)
	{
		if (Application::getPlayerNum() == 1 || Application::getPlayerNum() == 4) 
			RenderSpriteOnScreen(meshList[GEO_UI], UI_SELECTED, 40 - 20, 32 - 10, 40, 20, Color(0, 0, 1));
		else 
			RenderSpriteOnScreen(meshList[GEO_UI], UI_SELECTED, 40 - 20, 32 - 10, 40, 20 * Application::getPlayerNum(), Color(0, 0, 1));

		RenderTextOnScreen(meshList[GEO_TEXT], textWindow, Color(1, 1, 0), 3, 40, 32);

		if (inWindow == WINDOW_CONFIRM)
		{
			if (menuSelected[MENU_CONIRMATION] == 0)
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Deny", Color(1, 0, 0), 5, 32, 28);
				RenderTextOnScreen(meshList[GEO_TEXT], "Confirm", Color(1, 1, 0), 3, 45, 28);
			}
			else
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Deny", Color(1, 1, 0), 3, 32, 28);
				RenderTextOnScreen(meshList[GEO_TEXT], "Confirm", Color(1, 0, 0), 5, 45, 28);
			}
		}
		else if (inWindow == WINDOW_NOTIFY)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Okay", Color(1, 0, 0), 5, 40, 25);
		}
		else if (inWindow == WINDOW_INPUT)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], textInput, Color(1, 0, 0), 3, 40, 25);
		}
	}

	physics phys;

	// Render Debug Info
	if (showDebugInfo)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "FPS:" + std::to_string(fps), Color(0, 1, 0), 1, 0, 58, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], "Camera Position: " + std::to_string(camera[0].position.x) + ", " + std::to_string(camera[0].position.y) + 
			", " + std::to_string(camera[0].position.z), Color(0, 1, 0), 1, 0, 56, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], "Camera Target:   " + std::to_string(camera[0].target.x) + ", " + std::to_string(camera[0].target.y) +
			", " + std::to_string(camera[0].target.z), Color(0, 1, 0), 1, 0, 55, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], "Player 1 Input: UP: " + std::to_string(debugValues[DEBUG_PLAYER0_UP]) +
												", Down: " + std::to_string(debugValues[DEBUG_PLAYER0_DOWN]) + 
												", Left: " + std::to_string(debugValues[DEBUG_PLAYER0_LEFT]) + 
												", Right: " + std::to_string(debugValues[DEBUG_PLAYER0_RIGHT]) + 
												", Enter: " + std::to_string(debugValues[DEBUG_PLAYER0_ENTER]), Color(0, 1, 0), 1, 0, 53, 1);

		RenderTextOnScreen(meshList[GEO_TEXT], "Player 2 Input: UP: " + std::to_string(debugValues[DEBUG_PLAYER1_UP]) +
			", Down: " + std::to_string(debugValues[DEBUG_PLAYER1_DOWN]) +
			", Left: " + std::to_string(debugValues[DEBUG_PLAYER1_LEFT]) +
			", Right: " + std::to_string(debugValues[DEBUG_PLAYER1_RIGHT]) +
			", Enter: " + std::to_string(debugValues[DEBUG_PLAYER1_ENTER]), Color(0, 1, 0), 1, 0, 52, 1);
		if (Application::IsKeyPressed('X'))
		{
			for (int i = 0; i < 8; ++i) renderLightPos(i);
		}
	}
	
	modelStack.PopMatrix();
}

void GameScene0::renderSkysphere(int size)
{
	modelStack.PushMatrix();
	modelStack.Scale(size, size * Application::getPlayerNum(), size);

	RenderMesh(meshList[GEO_SKYSPHERE], false);
	RenderMesh(meshList[GEO_FLOOR], false);

	modelStack.PopMatrix();
}

void GameScene0::moveLight(double dt, int lightNum)
{
	if (Application::IsKeyPressed('I'))
		light[lightNum].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[lightNum].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[lightNum].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[lightNum].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[lightNum].position.y += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[lightNum].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('5')) light[lightNum].type = Light::LIGHT_POINT;
	if (Application::IsKeyPressed('6')) light[lightNum].type = Light::LIGHT_DIRECTIONAL;
	if (Application::IsKeyPressed('7')) light[lightNum].type = Light::LIGHT_SPOT;
}

void GameScene0::renderLightPos(int lightNum)
{
	modelStack.PushMatrix();
	modelStack.Translate(light[lightNum].position.x, light[lightNum].position.y, light[lightNum].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE], false);
	modelStack.PopMatrix();
}

void GameScene0::CalculateLights()
{
	for (int i = 0; i < 8; ++i)
	{
		if (light[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[i].position.x, light[i].position.y, light[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			if (i == 0) glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &lightDirection_cameraspace.x);
			if (i == 1) glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &lightDirection_cameraspace.x);
			if (i == 2) glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &lightDirection_cameraspace.x);
			if (i == 3) glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &lightDirection_cameraspace.x);
			if (i == 4) glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, &lightDirection_cameraspace.x);
			if (i == 5) glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, &lightDirection_cameraspace.x);
			if (i == 6) glUniform3fv(m_parameters[U_LIGHT6_SPOTDIRECTION], 1, &lightDirection_cameraspace.x);
			if (i == 7) glUniform3fv(m_parameters[U_LIGHT7_SPOTDIRECTION], 1, &lightDirection_cameraspace.x);
		}
		else if (light[i].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[i].position;
			if (i == 0) glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 1) glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 2) glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 3) glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 4) glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 5) glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 6) glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 7) glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, &lightPosition_cameraspace.x);

			Vector3 spotDirection_cameraspace = viewStack.Top() * light[i].spotDirection;
			if (i == 0) glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			if (i == 1) glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			if (i == 2) glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			if (i == 3) glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			if (i == 4) glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			if (i == 5) glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			if (i == 6) glUniform3fv(m_parameters[U_LIGHT6_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
			if (i == 7) glUniform3fv(m_parameters[U_LIGHT7_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[i].position;
			if (i == 0) glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 1) glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 2) glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 3) glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 4) glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 5) glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 6) glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, &lightPosition_cameraspace.x);
			if (i == 7) glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, &lightPosition_cameraspace.x);
		}
	}
}

void GameScene0::RenderMesh(Mesh* mesh, bool enableLight, float BBSize)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if(mesh->textureID > 0){ 
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);} 
	else { 
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	} 
	mesh->Render(); //this line should only be called once in the whole function
	
	if (BBSize != 0)
	{
		meshList[GEO_BOUNDINGBOX] = MeshBuilder::GenerateBoundingBox("Bounding Box", mesh->mSize.x, mesh->mSize.y, mesh->mSize.z,
			mesh->size.x + mesh->mSize.x, mesh->size.y + mesh->mSize.y, mesh->size.z + mesh->mSize.z);
		modelStack.PushMatrix();
		modelStack.Scale(BBSize, BBSize, BBSize);
		RenderMesh(meshList[GEO_BOUNDINGBOX], 0, 0);
		modelStack.PopMatrix();
	}

	if(mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}

void GameScene0::RenderSprite(Mesh* mesh, int frameCount, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check return; 
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	
	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
	mesh->Render(frameCount * 6, 6);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void GameScene0::RenderText(Mesh* mesh, std::string text, Color color, int type)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check return; 
		return;

	glDisable(GL_DEPTH_TEST); 
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
	glEnable(GL_DEPTH_TEST);
}

void GameScene0::RenderSpriteOnScreen(Mesh* mesh, int frameCount, float x, float y, float sizex, float sizey, Color color)
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

	if (Application::getPlayerNum() == 1 || Application::getPlayerNum() == 4)
		modelStack.Scale(sizex, sizex, 1);
	else
		modelStack.Scale(sizex, sizex * Application::getPlayerNum(), 1);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	mesh->Render(frameCount * 6, 6);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void GameScene0::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, int type)
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

	//modelStack.Scale(size, size, size);
	if (Application::getPlayerNum() == 1 || Application::getPlayerNum() == 4)
		modelStack.Scale(size, size, size);
	else 
		modelStack.Scale(size, size * Application::getPlayerNum(), size);

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

void GameScene0::CalculateFPS()
{
	// Fps
	float currentTime = GetTickCount64() * 0.001f;
	++framesPerSecond;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fps = (int)framesPerSecond;
		framesPerSecond = 0;
	}
}