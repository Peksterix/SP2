#include "VehicleScene.h"
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

extern bool isFullscreen;	// Try to use accessor instead
extern GLFWwindow* m_window;	// Try to use accessor instead
extern float bounceTime;	// Try to use accessor instead
//static bool isFullscreen = 1;

VehicleScene::VehicleScene()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

VehicleScene::~VehicleScene()
{
}

void VehicleScene::Exit()
{
	// Cleanup here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	for (int i = 0; i < 4; ++i)
	{
		delete custChassis[i];
		delete custWheel[i];
		delete custWeapon[i];
	}
	for (int i = 0; i < 8; ++i) delete vehicle[i];
	
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}

void VehicleScene::Init()
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
		
		camera.Init(Position(-20 + 130 , 12, -26), Position(0 + 130, 4, -8), Position(0, 1, 0));

	#pragma endregion

	#pragma region Initialise Mesh
	
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		meshList[GEO_TEXT]->textureID = LoadTGA("image//font.tga");
		meshList[GEO_UI] = MeshBuilder::GenerateText("UI", 16, 8);
		meshList[GEO_UI]->textureID = LoadTGA("image//UI Sheet.tga");
		meshList[GEO_WAREHOUSE_SHELL] = MeshBuilder::GenerateOBJ("Warehouse", "obj//Building2a.obj");
		meshList[GEO_WAREHOUSE_SHELL]->textureID = LoadTGA("image//Building2.tga");
		meshList[GEO_WAREHOUSE_RING] = MeshBuilder::GenerateOBJ("Warehouse", "obj//Building2b.obj");
		meshList[GEO_WAREHOUSE_RING]->textureID = LoadTGA("image//Building2.tga");
		meshList[GEO_WAREHOUSE_CENTRE] = MeshBuilder::GenerateOBJ("Warehouse", "obj//Building2c.obj");
		meshList[GEO_WAREHOUSE_CENTRE]->textureID = LoadTGA("image//Building2.tga");
		meshList[GEO_WAREHOUSE_BRIDGE] = MeshBuilder::GenerateOBJ("Warehouse", "obj//Building2d.obj");
		meshList[GEO_WAREHOUSE_BRIDGE]->textureID = LoadTGA("image//Building2.tga");
		meshList[GEO_WAREHOUSE_TEETH_TOP] = MeshBuilder::GenerateOBJ("Warehouse", "obj//Building2e.obj");
		meshList[GEO_WAREHOUSE_TEETH_TOP]->textureID = LoadTGA("image//Building2.tga");
		meshList[GEO_WAREHOUSE_TEETH_BOTTOM] = MeshBuilder::GenerateOBJ("Warehouse", "obj//Building2f.obj");
		meshList[GEO_WAREHOUSE_TEETH_BOTTOM]->textureID = LoadTGA("image//Building2.tga");
		meshList[GEO_SKYSPHERE] = MeshBuilder::GenerateOBJ("Skysphere (Space)", "obj//Skysphere.obj");
		meshList[GEO_SKYSPHERE]->textureID = LoadTGA("image//Flat Space2.tga");

		meshList[GEO_SHOWCASEFLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1,1,1), 20, 20);
		meshList[GEO_SHOWCASEFLOOR]->textureID = LoadTGA("image//ShowcaseFloor.tga");

		meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 8, 16, 1.f);
	
	#pragma endregion

	#pragma region Initialise Values

		Application::GetScreenSize(screenSizeX, screenSizeY);
		
		showDebugInfo = 1;
		showBoundingBox = 0;
		inWindow = WINDOW_NONE;
		for (int i = 0; i < 3; ++i)
		{
			vehiclePartSelect[i] = 0;
		}
		for (int i = 0; i < 5; ++i)
		{
			custChassis[i] = new Chassis(i - 1);
			custWheel[i] = new Wheel(i - 1);
			custWeapon[i] = new Weapon(i - 1);
		}
		for (int i = 0; i < 8; ++i)
		{
			vehicle[i] = new Vehicle;
			// Read the Vehicle's Saved Loadouts
		}
		// Temp
		vehicle[0]->setChassis(0);
		vehicle[0]->setWheel(0);
		vehicle[0]->setWeapon(0);
		vehicle[1]->setChassis(1);
		vehicle[1]->setWheel(1);
		vehicle[1]->setWeapon(1);
		vehicle[2]->setChassis(2);
		vehicle[2]->setWheel(2);
		vehicle[2]->setWeapon(2);
		vehicle[3]->setChassis(3);
		vehicle[3]->setWheel(3);
		vehicle[3]->setWeapon(3);

		for (int i = 0; i < ANI_TOTAL; ++i) aniVal[i] = 0;
		for (int i = 0; i < ANIS_TOTAL; ++i) animation[i] = 0;
		for (int i = 0; i < MENU_TOTAL; ++i) menuSelected[i] = 0;
		for (int i = 0; i < DEBUG_TOTAL; ++i) debugValues[i] = 0;

		aniVal[ANI_CAMERA_POSITION_X] = -43.996;
		aniVal[ANI_CAMERA_POSITION_Y] = 58.517;
		aniVal[ANI_CAMERA_POSITION_Z] = 45.608;
		aniVal[ANI_CAMERA_TARGET_X] = -43.164;
		aniVal[ANI_CAMERA_TARGET_Y] = 58.011;
		aniVal[ANI_CAMERA_TARGET_Z] = 45.053;

	#pragma endregion

	sound.play2DSound("Vivaldi - Four Seasons - Winter");
}

void VehicleScene::Update(double dt)
{
	CalculateFPS();

	// Debug Info
	if (showDebugInfo)
	{
		if (Application::IsKeyPressed('7')) glDisable(GL_CULL_FACE);					// Disable Cull
		if (Application::IsKeyPressed('8')) glEnable(GL_CULL_FACE);						// Enable Cull
		if (Application::IsKeyPressed('9')) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Normal
		if (Application::IsKeyPressed('0')) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Wireframe
		if (Application::IsKeyPressed('Z'))
		{
			StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_FREECAM;
			camera.Update(0.01, Math::RadianToDegree(atan2(camera.target.x - camera.position.x, camera.target.z - camera.position.z)),
				Math::RadianToDegree(atan2(camera.target.y - camera.position.y, sqrt(pow(camera.position.x - camera.target.x, 2) + pow(camera.position.z - camera.target.z, 2)))));
		}
	}
	
	if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_MAINMENU)
	{
		if ((Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP)) && bounceTime <= 0)
		{
			if (menuSelected[MENU_MAIN] != 0)
			{
				menuSelected[MENU_MAIN]--;
			}
			else
				menuSelected[MENU_MAIN] = 3;

			bounceTime = 0.25f;
		}
		else if ((Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN)) && bounceTime <= 0)
		{
			if (menuSelected[MENU_MAIN] != 3)
			{
				menuSelected[MENU_MAIN]++;
			}
			else
				menuSelected[MENU_MAIN] = 0;


			bounceTime = 0.25f;
		}
		else if (Application::IsKeyPressed(VK_RETURN) && bounceTime <= 0)
		{
			bounceTime = 0.25f;

			if (menuSelected[MENU_MAIN] == 0 || menuSelected[MENU_MAIN] == 1)
			{
				if (menuSelected[MENU_MAIN] == 0)
				{
					// Check if slot is available
					StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_LOADOUT_PLAY);
				}
				else StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_LOADOUT_CUSTOM);

				animation[ANIS_ANY] = 1;
				animation[ANIS_MENU_TO_LOADOUT] = 1;

				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_X], -43.164, 45, 0.5, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Y], 58.011, 30, 0.5, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Z], 45.053, 30, 0.5, 1);

				animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_X], -43.996, 110, 1, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Y], 58.517, 12, 1, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Z], 45.608, -26, 1, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_X], 45, 130, 1, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Y], 30, 4, 1, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Z], 30, -8, 1, 0);

			}
			else if (menuSelected[MENU_MAIN] == 2)
			{
				StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_OPTIONS);
			}
			else if (menuSelected[MENU_MAIN] == 3)
			{
				glfwSetWindowShouldClose(m_window, GL_TRUE);
			}
		}
	}
	if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_OPTIONS)
	{
		/*
		if ((Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP)) && bounceTime <= 0)
		{
			if (menuSelected[MENU_OPTIONS] != 0)
				menuSelected[MENU_OPTIONS]--;
			else
				menuSelected[MENU_OPTIONS] = 1;

			bounceTime = 0.25f;
		}
		if ((Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN)) && bounceTime <= 0)
		{
			if (menuSelected[MENU_OPTIONS] != 1)
				menuSelected[MENU_OPTIONS]++;
			else
				menuSelected[MENU_OPTIONS] = 0;

			bounceTime = 0.25f;
		}
		else if (Application::IsKeyPressed(VK_RETURN) && menuSelected[MENU_OPTIONS] == 0 && bounceTime <= 0)
		{
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
		else if (Application::IsKeyPressed(VK_RETURN) && menuSelected[MENU_OPTIONS] == 1 && bounceTime <= 0)
		{
			// State check for game?
			StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_MAINMENU);

			bounceTime = 0.25f;
		}
		*/
		OptionMenu::getInstance()->Update(dt);
	}
	if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_LOADOUT_CUSTOM || StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_LOADOUT_PLAY)
	{
		if ((Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP)) && !animation[ANIS_ANY] && bounceTime <= 0)
		{
			bounceTime = 0.25f;
			--menuSelected[MENU_LOADOUT_Y];

			if (menuSelected[MENU_LOADOUT_Y] > 1) menuSelected[MENU_LOADOUT_Y] = 0;
			if (menuSelected[MENU_LOADOUT_Y] < 0) menuSelected[MENU_LOADOUT_Y] = 1;
		}
		if ((Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN)) && !animation[ANIS_ANY] && bounceTime <= 0)
		{
			bounceTime = 0.25f;
			++menuSelected[MENU_LOADOUT_Y];

			if (menuSelected[MENU_LOADOUT_Y] > 1) menuSelected[MENU_LOADOUT_Y] = 0;
			if (menuSelected[MENU_LOADOUT_Y] < 0) menuSelected[MENU_LOADOUT_Y] = 1;
		}
		if ((Application::IsKeyPressed('A') || Application::IsKeyPressed(VK_LEFT)) && !animation[ANIS_ANY] && bounceTime <= 0)
		{
			bounceTime = 0.25f;

			if (inWindow == WINDOW_CONFIRM)
			{
				--menuSelected[MENU_CONIRMATION];

				if (menuSelected[MENU_CONIRMATION] > 1) menuSelected[MENU_CONIRMATION] = 0;
				if (menuSelected[MENU_CONIRMATION] < 0) menuSelected[MENU_CONIRMATION] = 1;
			}
			else if (!inWindow)
			{
				--menuSelected[MENU_LOADOUT_X];
				animate.AddToBuffer(aniVal[ANI_SHIP_ROTATION], aniVal[ANI_SHIP_ROTATION], aniVal[ANI_SHIP_ROTATION] - 45, 0.5, 1);
				animation[ANIS_ANY] = 1;
			}
		}
		if ((Application::IsKeyPressed('D') || Application::IsKeyPressed(VK_RIGHT)) && !animation[ANIS_ANY] && bounceTime <= 0)
		{
			bounceTime = 0.25f;
			
			if (inWindow == WINDOW_CONFIRM)
			{
				++menuSelected[MENU_CONIRMATION];

				if (menuSelected[MENU_CONIRMATION] > 1) menuSelected[MENU_CONIRMATION] = 0;
				if (menuSelected[MENU_CONIRMATION] < 0) menuSelected[MENU_CONIRMATION] = 1;
			}
			else if (!inWindow)
			{
				++menuSelected[MENU_LOADOUT_X];
				animate.AddToBuffer(aniVal[ANI_SHIP_ROTATION], aniVal[ANI_SHIP_ROTATION], aniVal[ANI_SHIP_ROTATION] + 45, 0.5, 1);
				animation[ANIS_ANY] = 1;
			}
		}
		if (Application::IsKeyPressed(VK_RETURN) && !animation[ANIS_ANY] && bounceTime <= 0)
		{
			bounceTime = 0.25f;
			Chassis* tempChassis = vehicle[menuSelected[MENU_LOADOUT_X]]->getChassis();

			if (inWindow)
			{
				// Edit || Play with Vehicle
				if (menuSelected[MENU_LOADOUT_Y] == 0 && menuSelected[MENU_CONIRMATION] == 1)
				{
					// Edit Vehicle
					if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_LOADOUT_CUSTOM)
					{
						StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_CUSTOMISE);
						animation[ANIS_ANY] = 1;
						animation[ANIS_LOADOUT_CUSTOM_TO_CUSTOMISE] = 1;
						menuSelected[MENU_CUSTOMISATION_Y] = 0;

						if (vehicle[menuSelected[MENU_LOADOUT_X]]->getChassis() != nullptr) menuSelected[MENU_CUSTOMISATION_X] = vehiclePartSelect[0] = vehicle[menuSelected[MENU_LOADOUT_X]]->getChassis()->getType();
						else menuSelected[MENU_CUSTOMISATION_X] = vehiclePartSelect[0] = -1;

						if (vehicle[menuSelected[MENU_LOADOUT_X]]->getWheel() != nullptr) vehiclePartSelect[1] = vehicle[menuSelected[MENU_LOADOUT_X]]->getWheel()->getType();
						else vehiclePartSelect[1] = -1;

						if (vehicle[menuSelected[MENU_LOADOUT_X]]->getWeapon() != nullptr) vehiclePartSelect[2] = vehicle[menuSelected[MENU_LOADOUT_X]]->getWeapon()->getType();
						else vehiclePartSelect[2] = -1;

						if (vehiclePartSelect[0] == -1)
						{
							animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_X], 110, -20.9, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Y], 12, 12, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Z], -26, -25.1, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_X], 130, -20.3, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Y], 4, 11.7, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Z], -8, -24.3, 1, 0);
							animate.AddToBuffer(aniVal[ANI_VEHICLE_ROTATION_Y], 0, 270, 1, 0);
							animate.AddToBuffer(aniVal[ANI_VEHICLE_POSITION], 0, 130, 1, 0);
							animate.AddToBuffer(aniVal[ANI_SHIP_BRIDGE_ROTATION], 0, 180, 1, 1);
						}
						else
						{
							animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_X], 110, .132, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Y], 12, 55.6, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Z], -26, -52.9, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_X], 130, .714, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Y], 4, 54.9, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Z], -8, -52.4, 1, 0);
							animate.AddToBuffer(aniVal[ANI_VEHICLE_ROTATION_Y], 0, 270, 3, 0);
							animate.AddToBuffer(aniVal[ANI_SHIP_BRIDGE_ROTATION], 0, 120, 1, 1);

							animate.AddToBuffer(aniVal[ANI_SHIP_BRIDGE_ROTATION], 120, 180, .5, 0);
							animate.AddToBuffer(aniVal[ANI_VEHICLE_ROTATION_Z], 0, -10, 2, 1);

							animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_X], .132, -20.9, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Y], 55.6, 12, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Z], -52.9, -25.1, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_X], .714, -20.3, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Y], 54.9, 11.7, 1, 0);
							animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Z], -52.4, -24.3, 1, 0);
							animate.AddToBuffer(aniVal[ANI_VEHICLE_ROTATION_Z], -10, -20, .25, 1);
							animate.AddToBuffer(aniVal[ANI_VEHICLE_POSITION], 0, 130, 1.2, 1);

							animate.AddToBuffer(aniVal[ANI_VEHICLE_ROTATION_Z], -20, 0, 0.25, 1);
						}
					}
					
					// Select Vehicle
					else if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_LOADOUT_PLAY)
					{
						// Play the Game
						StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_FREECAM); // Temp
					}
				}
				
				// Back to main menu
				else if (menuSelected[MENU_CONIRMATION] == 1)
				{
					StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_MAINMENU);
					animation[ANIS_ANY] = 1;
					animation[ANIS_LOADOUT_TO_MENU] = 1;

					animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_X], 110, -43.996, 1, 0);
					animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Y], 12, 58.517, 1, 0);
					animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Z], -26, 45.608, 1, 1);
					animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_X], 130, -43.164, 0.5, 0);
					animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Y], 4, 58.011, 0.5, 0);
					animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Z], -8, 45.053, 0.5, 0);
					
				}
				inWindow = WINDOW_NONE;
			}
			else
			{
				menuSelected[MENU_CONIRMATION] = 0;
				
				if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_LOADOUT_CUSTOM)
				{
					inWindow = WINDOW_CONFIRM;
					if (menuSelected[MENU_LOADOUT_Y] == 0) textWindow = "Edit Vehicle in Slot " + std::to_string(menuSelected[MENU_LOADOUT_X] + 1) + "?";
					else if (menuSelected[MENU_LOADOUT_Y] == 1) textWindow = "Exit to Main Menu?";
				}
				if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_LOADOUT_PLAY)
				{
					inWindow = WINDOW_CONFIRM;

					if (tempChassis == nullptr && menuSelected[MENU_LOADOUT_Y] == 0)
					{
						inWindow = WINDOW_NOTIFY;
						textWindow = "Vehicle is Empty";
					}
					else if (menuSelected[MENU_LOADOUT_Y] == 0) textWindow = "Play as Vechicle " + std::to_string(menuSelected[MENU_LOADOUT_X] + 1) + "?";
					else if (menuSelected[MENU_LOADOUT_Y] == 1) textWindow = "Exit to Main Menu?";
					
				}
			}
		}

		if (menuSelected[MENU_LOADOUT_X] > 7) menuSelected[MENU_LOADOUT_X] = 0;
		if (menuSelected[MENU_LOADOUT_X] < 0) menuSelected[MENU_LOADOUT_X] = 7;
	}
	if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_CUSTOMISE)
	{
		if ((Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP)) && bounceTime <= 0 && !animation[ANIS_ANY] && !inWindow)
		{
			--menuSelected[MENU_CUSTOMISATION_Y];
			bounceTime = 0.25f;
			if (menuSelected[MENU_CUSTOMISATION_Y] > 4) menuSelected[MENU_CUSTOMISATION_Y] = 0;
			if (menuSelected[MENU_CUSTOMISATION_Y] < 0) menuSelected[MENU_CUSTOMISATION_Y] = 4;
			if (menuSelected[MENU_CUSTOMISATION_Y] >= 0 && menuSelected[MENU_CUSTOMISATION_Y] <= 2) menuSelected[MENU_CUSTOMISATION_X] = vehiclePartSelect[menuSelected[MENU_CUSTOMISATION_Y]];
			//else if (menuSelected[MENU_CUSTOMISATION_Y] == 3) menuSelected[MENU_CUSTOMISATION_X] = showBoundingBox;
			else menuSelected[MENU_CUSTOMISATION_X] = 0;
		}
		if ((Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN)) && bounceTime <= 0 && !animation[ANIS_ANY] && !inWindow)
		{
			++menuSelected[MENU_CUSTOMISATION_Y];
			bounceTime = 0.25f;
			if (menuSelected[MENU_CUSTOMISATION_Y] > 4) menuSelected[MENU_CUSTOMISATION_Y] = 0;
			if (menuSelected[MENU_CUSTOMISATION_Y] < 0) menuSelected[MENU_CUSTOMISATION_Y] = 4;
			if (menuSelected[MENU_CUSTOMISATION_Y] >= 0 && menuSelected[MENU_CUSTOMISATION_Y] <= 2) menuSelected[MENU_CUSTOMISATION_X] = vehiclePartSelect[menuSelected[MENU_CUSTOMISATION_Y]];
			//else if (menuSelected[MENU_CUSTOMISATION_Y] == 3) menuSelected[MENU_CUSTOMISATION_X] = showBoundingBox;
			else menuSelected[MENU_CUSTOMISATION_X] = 0;
		}		
		if ((Application::IsKeyPressed('A') || Application::IsKeyPressed(VK_LEFT)) && bounceTime <= 0 && !animation[ANIS_ANY] && inWindow != WINDOW_NOTIFY)
		{
			bounceTime = 0.25f;

			if (inWindow == WINDOW_CONFIRM)
			{
				--menuSelected[MENU_CONIRMATION];
				
				if (menuSelected[MENU_CONIRMATION] > 1) menuSelected[MENU_CONIRMATION] = 0;
				if (menuSelected[MENU_CONIRMATION] < 0) menuSelected[MENU_CONIRMATION] = 1;
			}
			else
			{
				--menuSelected[MENU_CUSTOMISATION_X];
				animation[ANIS_ANY] = 1;

				if (menuSelected[MENU_CUSTOMISATION_Y] >= 0 && menuSelected[MENU_CUSTOMISATION_Y] <= 2)
				{
					animation[ANIS_CUSTOMISE_PARTS] = 1;

					if (menuSelected[MENU_CUSTOMISATION_X] > 3) menuSelected[MENU_CUSTOMISATION_X] = -1;
					if (menuSelected[MENU_CUSTOMISATION_X] < -1) menuSelected[MENU_CUSTOMISATION_X] = 3;

					animate.AddToBuffer(aniVal[ANI_SHIP_TEETH_POSITION], 0, 2, 0.25, 1);
					animate.AddToBuffer(aniVal[ANI_BUFFER], 0, 1, 0.5, 1);

					animate.AddToBuffer(aniVal[ANI_SHIP_TEETH_POSITION], 2, 0, 0.25, 0);
					animate.AddToBuffer(aniVal[ANI_BUFFER], 1, 0, 0.25, 1);
				}
				else if (menuSelected[MENU_CUSTOMISATION_Y] == 3)
				{
					aniVal[ANI_VEHICLE_ROTATION_Y] += dt * 40;
					bounceTime = 0.f;
				}
				else
				{
					if (menuSelected[MENU_CUSTOMISATION_X] > 1) menuSelected[MENU_CUSTOMISATION_X] = 0;
					if (menuSelected[MENU_CUSTOMISATION_X] < 0) menuSelected[MENU_CUSTOMISATION_X] = 1;
				}
			}
		}
		if ((Application::IsKeyPressed('D') || Application::IsKeyPressed(VK_RIGHT)) && bounceTime <= 0 && !animation[ANIS_ANY] && inWindow != WINDOW_NOTIFY)
		{
			bounceTime = 0.25f;

			if (inWindow == WINDOW_CONFIRM)
			{
				++menuSelected[MENU_CONIRMATION];

				if (menuSelected[MENU_CONIRMATION] > 1) menuSelected[MENU_CONIRMATION] = 0;
				if (menuSelected[MENU_CONIRMATION] < 0) menuSelected[MENU_CONIRMATION] = 1;
				// ??
			}
			else
			{
				++menuSelected[MENU_CUSTOMISATION_X];
				animation[ANIS_ANY] = 1;

				if (menuSelected[MENU_CUSTOMISATION_Y] >= 0 && menuSelected[MENU_CUSTOMISATION_Y] <= 2)
				{
					animation[ANIS_CUSTOMISE_PARTS] = 1;

					if (menuSelected[MENU_CUSTOMISATION_X] > 3) menuSelected[MENU_CUSTOMISATION_X] = -1;
					if (menuSelected[MENU_CUSTOMISATION_X] < -1) menuSelected[MENU_CUSTOMISATION_X] = 3;

					animate.AddToBuffer(aniVal[ANI_SHIP_TEETH_POSITION], 0, 2, 0.25, 1);
					animate.AddToBuffer(aniVal[ANI_BUFFER], 0, 1, 0.5, 1);

					animate.AddToBuffer(aniVal[ANI_SHIP_TEETH_POSITION], 2, 0, 0.25, 0);
					animate.AddToBuffer(aniVal[ANI_BUFFER], 1, 0, 0.25, 1);
				}
				else if (menuSelected[MENU_CUSTOMISATION_Y] == 3)
				{
					aniVal[ANI_VEHICLE_ROTATION_Y] -= dt * 40;
					bounceTime = 0.f;
				}
				else
				{
					if (menuSelected[MENU_CUSTOMISATION_X] > 1) menuSelected[MENU_CUSTOMISATION_X] = 0;
					if (menuSelected[MENU_CUSTOMISATION_X] < 0) menuSelected[MENU_CUSTOMISATION_X] = 1;
				}
			}
		}
		if (Application::IsKeyPressed(VK_RETURN) && !animation[ANIS_ANY] && menuSelected[MENU_CUSTOMISATION_Y] == 4 && bounceTime <= 0)
		{
			bounceTime = 0.25f;
			Weapon* tempWeapon = vehicle[menuSelected[MENU_LOADOUT_X]]->getWeapon();
			Wheel* tempWheel = vehicle[menuSelected[MENU_LOADOUT_X]]->getWheel();
			Chassis* tempChassis = vehicle[menuSelected[MENU_LOADOUT_X]]->getChassis();
			bool somethingChanged =
				(tempWeapon == nullptr || tempWeapon->getType() != vehiclePartSelect[2]) ||
				(tempWheel == nullptr || tempWheel->getType() != vehiclePartSelect[1]) ||
				(tempChassis == nullptr || tempChassis->getType() != vehiclePartSelect[0]);
			
			bool emptyIsEmplty =
				tempWeapon == nullptr && vehiclePartSelect[2] == -1 &&
				tempWheel == nullptr && vehiclePartSelect[1] == -1 &&
				tempChassis == nullptr && vehiclePartSelect[0] == -1;

			if (inWindow == WINDOW_CONFIRM || inWindow == WINDOW_NOTIFY)
			{
				if (menuSelected[MENU_CONIRMATION] == 1)
				{
					animation[ANIS_ANY] = 1;
					animation[ANIS_CUSTOMISE_TO_LOADOUT] = 1;
					//showBoundingBox = 0;

					animate.AddToBuffer(aniVal[ANI_SHIP_TEETH_POSITION], 0, 2, 0.25, 1);
					animate.AddToBuffer(aniVal[ANI_BUFFER], 0, 1, 0.5, 1);
					animate.AddToBuffer(aniVal[ANI_SHIP_TEETH_POSITION], 2, 0, 0.25, 0);
					animate.AddToBuffer(aniVal[ANI_BUFFER], 1, 0, 0.25, 1);

					animate.AddToBuffer(aniVal[ANI_VEHICLE_ROTATION_Y], aniVal[ANI_VEHICLE_ROTATION_Y], 270, 1, 1);
					animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_X], -20.9, 110, 2, 0);
					animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Y], 12, 12, 2, 0);
					animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Z], -26, -26, 1, 0);
					animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_X], -20.3, 130, 2, 0);
					animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Y], 11.7, 4, 2, 0);
					animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Z], -24.3, -8, 2, 0);
					animate.AddToBuffer(aniVal[ANI_VEHICLE_POSITION], 130, 0, 2, 1);

					animate.AddToBuffer(aniVal[ANI_SHIP_BRIDGE_ROTATION], 180, 0, 2, 0);
					animate.AddToBuffer(aniVal[ANI_VEHICLE_ROTATION_Y], 270, 0, 2, 1);
				}
				inWindow = WINDOW_NONE;
			}
			else if (!somethingChanged)
			{
				StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_LOADOUT_CUSTOM);
				animation[ANIS_ANY] = 1;
				animation[ANIS_CUSTOMISE_TO_LOADOUT] = 1;

				animate.AddToBuffer(aniVal[ANI_VEHICLE_ROTATION_Y], aniVal[ANI_VEHICLE_ROTATION_Y], 270, 1, 1);
				animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_X], -20.9, 110, 2, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Y], 12, 12, 2, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Z], -26, -26, 1, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_X], -20.3, 130, 2, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Y], 11.7, 4, 2, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Z], -24.3, -8, 2, 0);
				animate.AddToBuffer(aniVal[ANI_VEHICLE_POSITION], 130, 0, 2, 1);
				animate.AddToBuffer(aniVal[ANI_SHIP_BRIDGE_ROTATION], 180, 0, 2, 0);
				animate.AddToBuffer(aniVal[ANI_VEHICLE_ROTATION_Y], 270, 0, 2, 1);
			}
			else if (emptyIsEmplty)
			{
				StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_LOADOUT_CUSTOM);
				animation[ANIS_ANY] = 1;
				animation[ANIS_CUSTOMISE_TO_LOADOUT] = 1;

				animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_X], -20.9, 110, 2, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Y], 12, 12, 2, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_POSITION_Z], -26, -26, 1, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_X], -20.3, 130, 2, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Y], 11.7, 4, 2, 0);
				animate.AddToBuffer(aniVal[ANI_CAMERA_TARGET_Z], -24.3, -8, 2, 0);

				animate.AddToBuffer(aniVal[ANI_SHIP_BRIDGE_ROTATION], 180, 0, 2, 0);
				aniVal[ANI_VEHICLE_ROTATION_Y] = 0;
				aniVal[ANI_VEHICLE_POSITION] = 0;
			}
			else
			{
				inWindow = WINDOW_CONFIRM;
				menuSelected[MENU_CONIRMATION] = 0;

				if (menuSelected[MENU_CUSTOMISATION_X] == 0)
				{

					if (vehiclePartSelect[0] == -1 || vehiclePartSelect[1] == -1 || vehiclePartSelect[2] == -1)
					{
						inWindow = WINDOW_NOTIFY;
						textWindow = "There are missing Parts";
					}
					else 
						textWindow = "Save Current Vehicle to Slot " + std::to_string(menuSelected[MENU_LOADOUT_X] + 1) + "?";
					
				}
				if (menuSelected[MENU_CUSTOMISATION_X] == 1)
				{
					textWindow = "Exit without saving Vehicle to Slot " + std::to_string(menuSelected[MENU_LOADOUT_X] + 1) + "?";
				}
			}
			
		} 

		//if (menuSelected[MENU_CUSTOMISATION_Y] == 3 && menuSelected[MENU_CUSTOMISATION_X] == 1) showBoundingBox = 1;
		//if (menuSelected[MENU_CUSTOMISATION_Y] == 3 && menuSelected[MENU_CUSTOMISATION_X] == 0) showBoundingBox = 0;

		if (aniVal[ANI_BUFFER] > .9 && animation[ANIS_CUSTOMISE_PARTS])
		{
			vehiclePartSelect[menuSelected[MENU_CUSTOMISATION_Y]] = menuSelected[MENU_CUSTOMISATION_X];
		}
		if (aniVal[ANI_BUFFER] > .9 && animation[ANIS_CUSTOMISE_TO_LOADOUT])
		{
			StateManager::getInstance()->setGameState(StateManager::GAME_STATES::S_LOADOUT_CUSTOM);
			
			if (menuSelected[MENU_CUSTOMISATION_X] == 0)
			{
				// Call Class once and store in temp var to decrease the number of time the program has to access the class
				Weapon* tempWeapon = vehicle[menuSelected[MENU_LOADOUT_X]]->getWeapon();
				Wheel* tempWheel = vehicle[menuSelected[MENU_LOADOUT_X]]->getWheel();
				Chassis* tempChassis = vehicle[menuSelected[MENU_LOADOUT_X]]->getChassis();

				// If part didnt change, then dont change it
				if (tempWeapon == nullptr || tempWeapon->getType() != vehiclePartSelect[2])
					vehicle[menuSelected[MENU_LOADOUT_X]]->setWeapon(vehiclePartSelect[2]);

				if (tempWheel == nullptr || tempWheel->getType() != vehiclePartSelect[1])
					vehicle[menuSelected[MENU_LOADOUT_X]]->setWheel(vehiclePartSelect[1]);

				if (tempChassis == nullptr || tempChassis->getType() != vehiclePartSelect[0])
					vehicle[menuSelected[MENU_LOADOUT_X]]->setChassis(vehiclePartSelect[0]);
				
				// Save data
			}
		}
	}
	
	if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_FREECAM)
	{
		camera.Update(dt);
	}
	else
	{
		camera.Init(Position(aniVal[ANI_CAMERA_POSITION_X], aniVal[ANI_CAMERA_POSITION_Y], aniVal[ANI_CAMERA_POSITION_Z]), 
					Position(aniVal[ANI_CAMERA_TARGET_X], aniVal[ANI_CAMERA_TARGET_Y], aniVal[ANI_CAMERA_TARGET_Z]), 
					Position(0, 1, 0));
	}

	// Lights
	for (int i = 0, angle = -5 - aniVal[ANI_SHIP_ROTATION]; i < 6; ++i, angle += 360 / 6)
	{
		light[i].position.Set(120 * cos(Math::DegreeToRadian(angle)), 10, 120 * sin(Math::DegreeToRadian(angle)));
	}

	// Animation Logic
	// animation[ANIS_ANY] = animate.Update(0.02);
	if (fps >= 40) animation[ANIS_ANY] = animate.Update(0.02);

	if (!animation[ANIS_ANY])
		for (int i = 0; i < ANIS_TOTAL; ++i)
			animation[i] = 0;
	
	aniVal[ANI_SKYBOX_ROTATION] += dt * 0.5;
	if (aniVal[ANI_SKYBOX_ROTATION] > 360) aniVal[ANI_SKYBOX_ROTATION] = 0;

	aniVal[ANI_SELECTED_UI] += dt * 10;
	if (aniVal[ANI_SELECTED_UI] > 4) aniVal[ANI_SELECTED_UI] = 0;	
}

void VehicleScene::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Full window	
	glViewport(0, 0, screenSizeX, screenSizeY);
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	CalculateLights();

	renderScene();
	
}

void VehicleScene::renderScene()
{
	modelStack.PushMatrix();
	modelStack.Rotate(aniVal[ANI_SHIP_ROTATION], 0, 1, 0);

	// Render Vehicles
	for (int j = 0, angle = 0; j < 8; ++j, angle += 360 / 8)
	{
		modelStack.PushMatrix();

		// If vehicle is not empty:
		vehicle[j]->position.Set(130 * cos(Math::DegreeToRadian(angle)), 2, 130 * sin(Math::DegreeToRadian(angle)));
		vehicle[j]->rotate.Set(0, 180 - angle, 0);
		
		vehicle[menuSelected[MENU_LOADOUT_X]]->position.Set((130 - aniVal[ANI_VEHICLE_POSITION]) * cos(Math::DegreeToRadian(angle)), 2, (130 - aniVal[ANI_VEHICLE_POSITION]) * sin(Math::DegreeToRadian(angle)));
		vehicle[menuSelected[MENU_LOADOUT_X]]->rotate.Set(0, 180 - angle - aniVal[ANI_VEHICLE_ROTATION_Y], aniVal[ANI_VEHICLE_ROTATION_Z]);
		
		// Old code
		/*
		if (!(vehicle[j]->getChassis() == nullptr && vehicle[j]->getWheel() == nullptr && vehicle[j]->getWeapon() == nullptr))
		{
			// Call positions once and store in temp var to decrease weight on program
			Position tempVehPos[2] = { vehicle[j]->position, vehicle[j]->rotate };
			modelStack.Translate(tempVehPos[0].x, tempVehPos[0].y, tempVehPos[0].z);
			modelStack.Rotate(tempVehPos[1].x, 0, 0, 1);
			modelStack.Rotate(tempVehPos[1].y, 0, 1, 0);
			modelStack.Rotate(tempVehPos[1].z, 1, 0, 0);
			
			modelStack.PushMatrix();
			
			if (vehicle[j]->getChassis() != nullptr)
			{
				// Render Pre-Loaded Parts for Customisation
				if (j == menuSelected[MENU_LOADOUT_X] && state == S_CUSTOMISE && !animation[ANIS_LOADOUT_CUSTOM_TO_CUSTOMISE])
				{
					if (vehiclePartSelect[0] != -1)
					{
						Mesh* tempChaMesh = custChassis[vehiclePartSelect[0] + 1]->getMesh();
						modelStack.Translate(0, tempChaMesh->size.y / 2, 0);
						RenderMesh(custChassis[vehiclePartSelect[0] + 1]->getMesh(), true, custChassis[vehiclePartSelect[0] + 1]->getBBScale() * showBoundingBox);
					}
				}
				// Render Vehicle's Parts
				else
				{
					Mesh* tempChaMesh = vehicle[j]->getChassis()->getMesh();
					modelStack.Translate(0, tempChaMesh->size.y / 2, 0);
					RenderMesh(tempChaMesh, true, vehicle[j]->getChassis()->getBBScale() * showBoundingBox);
				}
			}
			if (vehicle[j]->getWheel() != nullptr)
			{
				// Render Pre-Loaded Parts for Customisation
				if (j == menuSelected[MENU_LOADOUT_X] && state == S_CUSTOMISE && !animation[ANIS_LOADOUT_CUSTOM_TO_CUSTOMISE])
				{
					if (vehiclePartSelect[1] != -1)
						for (int i = 0; i < custChassis[vehiclePartSelect[0] + 1]->wheelPos.size(); ++i)
						{
							Position tempWheelPos[2];
							tempWheelPos[0] = custChassis[vehiclePartSelect[0] + 1]->wheelPos[i]; tempWheelPos[1] = custChassis[vehiclePartSelect[0] + 1]->wheelScale[i];
							modelStack.PushMatrix();
							modelStack.Translate(tempWheelPos[0].x, tempWheelPos[0].y, tempWheelPos[0].z);
							modelStack.Rotate(aniVal[ANI_WHEEL_ROTATION] * 2 + aniVal[ANI_VEHICLE_POSITION] * 2, 1, 0, 0);
							modelStack.Scale(tempWheelPos[1].x, tempWheelPos[1].y, tempWheelPos[1].z);
							RenderMesh(custWheel[vehiclePartSelect[1] + 1]->getMesh(), true, custWheel[vehiclePartSelect[1] + 1]->getBBScale() * showBoundingBox);
							modelStack.PopMatrix();
						}
				}
				// Render Vehicle's Parts
				else
				{
					for (int i = 0; i < vehicle[j]->getChassis()->wheelPos.size(); ++i)
					{
						Position tempWheelPos[2];
						tempWheelPos[0] = vehicle[j]->wheelPos[i]; tempWheelPos[1] = vehicle[j]->wheelScale[i];
						modelStack.PushMatrix();
						modelStack.Translate(tempWheelPos[0].x, tempWheelPos[0].y, tempWheelPos[0].z);
						modelStack.Rotate(aniVal[ANI_WHEEL_ROTATION] * 2 + aniVal[ANI_VEHICLE_POSITION] * 2, 1, 0, 0);
						modelStack.Scale(tempWheelPos[1].x, tempWheelPos[1].y, tempWheelPos[1].z);
						RenderMesh(vehicle[j]->getWheel()->getMesh(), true, vehicle[j]->getWheel()->getBBScale() * showBoundingBox);
						modelStack.PopMatrix();
					}
				}
			}
			if (vehicle[j]->getWeapon() != nullptr)
			{

			}

			modelStack.PopMatrix();
			
		}
		modelStack.PopMatrix();
		*/

		// Call positions once and store in temp var to decrease weight on program
		Position tempVehPos[2] = { vehicle[j]->position, vehicle[j]->rotate };
		modelStack.Translate(tempVehPos[0].x, tempVehPos[0].y, tempVehPos[0].z);
		modelStack.Rotate(tempVehPos[1].x, 0, 0, 1);
		modelStack.Rotate(tempVehPos[1].y, 0, 1, 0);
		modelStack.Rotate(tempVehPos[1].z, 1, 0, 0);

		modelStack.PushMatrix();

		#pragma region Render Chassis
			
		// Render Pre-Loaded Parts for Customisation
		if (j == menuSelected[MENU_LOADOUT_X] && StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_CUSTOMISE && !animation[ANIS_LOADOUT_CUSTOM_TO_CUSTOMISE])
		{
			if (vehiclePartSelect[0] != -1)
			{
				Mesh* tempChaMesh = custChassis[vehiclePartSelect[0] + 1]->getMesh();
				modelStack.Translate(0, tempChaMesh->size.y / 2, 0);
				RenderMesh(custChassis[vehiclePartSelect[0] + 1]->getMesh(), true, custChassis[vehiclePartSelect[0] + 1]->getBBScale() * showBoundingBox);
			}
		}
		// Render Vehicle's Parts
		else
		{
			if (vehicle[j]->getChassis() != nullptr)
			{
				Mesh* tempChaMesh = vehicle[j]->getChassis()->getMesh();
				modelStack.Translate(0, tempChaMesh->size.y / 2, 0);
				RenderMesh(tempChaMesh, true, vehicle[j]->getChassis()->getBBScale() * showBoundingBox);
			}
		}	

		#pragma endregion
			
		#pragma region Render Wheels
			
		// Render Pre-Loaded Parts for Customisation
		if (j == menuSelected[MENU_LOADOUT_X] && StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_CUSTOMISE && !animation[ANIS_LOADOUT_CUSTOM_TO_CUSTOMISE])
		{
			if (vehiclePartSelect[1] != -1)
				for (int i = 0; i < custChassis[vehiclePartSelect[0] + 1]->wheelPos.size(); ++i)
				{
					Position tempWheelPos[2];
					tempWheelPos[0] = custChassis[vehiclePartSelect[0] + 1]->wheelPos[i]; 
					tempWheelPos[1] = custChassis[vehiclePartSelect[0] + 1]->wheelScale[i];
					modelStack.PushMatrix();
					modelStack.Translate(tempWheelPos[0].x, tempWheelPos[0].y, tempWheelPos[0].z);
					modelStack.Rotate(aniVal[ANI_WHEEL_ROTATION] * 2 + aniVal[ANI_VEHICLE_POSITION] * 2, 1, 0, 0);
					modelStack.Scale(tempWheelPos[1].x, tempWheelPos[1].y, tempWheelPos[1].z);
					RenderMesh(custWheel[vehiclePartSelect[1] + 1]->getMesh(), true, custWheel[vehiclePartSelect[1] + 1]->getBBScale() * showBoundingBox);
					modelStack.PopMatrix();
				}
		}
		// Render Vehicle's Parts
		else
		{
			if (vehicle[j]->getWheel() != nullptr)
				for (int i = 0; i < vehicle[j]->getChassis()->wheelPos.size(); ++i)
				{
					Position tempWheelPos[2];
					tempWheelPos[0] = vehicle[j]->wheelPos[i]; 
					tempWheelPos[1] = vehicle[j]->wheelScale[i];
					modelStack.PushMatrix();
					modelStack.Translate(tempWheelPos[0].x, tempWheelPos[0].y, tempWheelPos[0].z);
					modelStack.Rotate(aniVal[ANI_WHEEL_ROTATION] * 2 + aniVal[ANI_VEHICLE_POSITION] * 2, 1, 0, 0);
					modelStack.Scale(tempWheelPos[1].x, tempWheelPos[1].y, tempWheelPos[1].z);
					RenderMesh(vehicle[j]->getWheel()->getMesh(), true, vehicle[j]->getWheel()->getBBScale() * showBoundingBox);
					modelStack.PopMatrix();
				}
		}

		#pragma endregion

		#pragma region Render Weapons
			
		// Render Pre-Loaded Parts for Customisation
		if (j == menuSelected[MENU_LOADOUT_X] && StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_CUSTOMISE && !animation[ANIS_LOADOUT_CUSTOM_TO_CUSTOMISE])
		{
			if (vehiclePartSelect[2] != -1)
				for (int i = 0; i < custChassis[vehiclePartSelect[0] + 1]->weaponPos.size(); ++i)
				{
					Position tempWeaponPos[2];
					tempWeaponPos[0] = custChassis[vehiclePartSelect[0] + 1]->weaponPos[i];
					tempWeaponPos[1] = custChassis[vehiclePartSelect[0] + 1]->weaponScale[i];
					modelStack.PushMatrix();
					modelStack.Translate(tempWeaponPos[0].x, tempWeaponPos[0].y, tempWeaponPos[0].z);
					modelStack.Scale(tempWeaponPos[1].x, tempWeaponPos[1].y, tempWeaponPos[1].z);
					RenderMesh(custWeapon[vehiclePartSelect[2] + 1]->getMesh(), true, custWeapon[vehiclePartSelect[2] + 1]->getBBScale() * showBoundingBox);
					modelStack.PopMatrix();
				}
		}
		// Render Vehicle's Parts
		else
		{
			if (vehicle[j]->getWeapon() != nullptr)
				for (int i = 0; i < vehicle[j]->getChassis()->weaponPos.size(); ++i)
				{
					Position tempWeaponPos[2];
					tempWeaponPos[0] = vehicle[j]->weaponPos[i];
					tempWeaponPos[1] = vehicle[j]->weaponScale[i];
					modelStack.PushMatrix();
					modelStack.Translate(tempWeaponPos[0].x, tempWeaponPos[0].y, tempWeaponPos[0].z);
					modelStack.Scale(tempWeaponPos[1].x, tempWeaponPos[1].y, tempWeaponPos[1].z);
					RenderMesh(vehicle[j]->getWeapon()->getMesh(), true, vehicle[j]->getWeapon()->getBBScale() * showBoundingBox);
					modelStack.PopMatrix();
				}
		}

		#pragma endregion

		modelStack.PopMatrix();
		
		modelStack.PopMatrix();
	}

	renderSkysphere(26.5);

	// Render Loadout Text
	if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_LOADOUT_CUSTOM || StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_LOADOUT_PLAY)
		for (int j = 0, angle = 0; j < 8; ++j, angle += 360 / 8)
		{
			modelStack.PushMatrix();
			modelStack.Translate(140 * cos(Math::DegreeToRadian(angle)), 10, 140 * sin(Math::DegreeToRadian(angle)));
			modelStack.Rotate(-angle - 90, 0, 1, 0);
			modelStack.Scale(5, 5, 5);

			// Render Slot
			modelStack.PushMatrix();
			if (menuSelected[MENU_LOADOUT_X] != j ||  menuSelected[MENU_LOADOUT_Y] == 1) RenderText(meshList[GEO_TEXT], "Slot " + std::to_string(j + 1), Color(1, 1, 1));
			else { modelStack.Scale(1.1, 1.1, 1.1); RenderText(meshList[GEO_TEXT], "Slot " + std::to_string(j + 1), Color(1, 0, 0)); }
			
			modelStack.PopMatrix();

			// Render Exit
			modelStack.PushMatrix();
			modelStack.Translate(0, -1.8, 4);
			modelStack.Rotate(-90, 1, 0, 0);	
			if (menuSelected[MENU_LOADOUT_Y] == 0) RenderText(meshList[GEO_TEXT], "Exit", Color(1, 1, 1));
			else { modelStack.Scale(1.1, 1.1, 1.1); RenderText(meshList[GEO_TEXT], "Exit", Color(1, 0, 0)); }

			modelStack.PopMatrix();

			// Render Empty
			if (vehicle[j]->getChassis() == nullptr && vehicle[j]->getWheel() == nullptr && vehicle[j]->getWeapon() == nullptr)
			{
				modelStack.Translate(-2, -1.5 - 0.5, 0);
				modelStack.Scale(4, 3, 1);
				RenderSprite(meshList[GEO_UI], UI_EMPTY, Color(1, 0, 0));
			}
			
			// Render Stats
			else 
			{	
				modelStack.PushMatrix();
				modelStack.Translate(-1.25, -1.8, 0);
				modelStack.Scale(.5, .5, .5);
				modelStack.Rotate(-90, 1, 0, 0);
				modelStack.Rotate(-90, 0, 0, 1);
				modelStack.Translate(-1, -3, 0); RenderText(meshList[GEO_TEXT], "Chassis: " + vehicle[j]->getChassis()->getName(), Color(1, 1, 1), 1);
				modelStack.Translate(1, -1, 0); RenderText(meshList[GEO_TEXT], "Wheel: " + vehicle[j]->getWheel()->getName(), Color(1, 1, 1), 1);
				modelStack.Translate(-0.5, -1, 0); RenderText(meshList[GEO_TEXT], "Weapon: " + vehicle[j]->getWeapon()->getName(), Color(1, 1, 1), 1);
				modelStack.PopMatrix();
				
			}

			modelStack.PopMatrix();

			// Sprites
			/*modelStack.PushMatrix();
			modelStack.Translate(140 * cos(Math::DegreeToRadian(angle)), 11, 140 * sin(Math::DegreeToRadian(angle)));
			modelStack.Rotate(-angle - 90, 0, 1, 0);
			modelStack.Scale(5, 5, 5);
			
			if (menuSelected[MENU_LOADOUT_X] == j) 
			{
				modelStack.PushMatrix();
				modelStack.Translate(-2, 0, 0);
				modelStack.Scale(4, 1, 1);
				RenderSprite(meshList[GEO_UI], UI_SELECTED + aniVal[ANI_SELECTED_UI]);
				modelStack.PopMatrix();
			}
			
			RenderText(meshList[GEO_TEXT], "Slot " + std::to_string(j + 1), Color(1, 1, 1));

			if (vehicle[j]->getChassis() == nullptr && vehicle[j]->getWheel() == nullptr && vehicle[j]->getWeapon() == nullptr)
			{
				modelStack.Translate(-2, -1.5 - 0.5, 0);
				modelStack.Scale(4, 3, 1);
				RenderSprite(meshList[GEO_UI], UI_EMPTY);
			}
			modelStack.PopMatrix();*/
		}
	
	modelStack.PopMatrix();

	// Bridge
	modelStack.PushMatrix();
	modelStack.Rotate(-90 + aniVal[ANI_SHIP_BRIDGE_ROTATION], 0, 1, 0);
	modelStack.Scale(26.5, 26.5, 26.5);
	RenderMesh(meshList[GEO_WAREHOUSE_BRIDGE], false);

	modelStack.PopMatrix();

	// Render Customise Text
	{
		Color tempCol[6] = { Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };
		if (menuSelected[MENU_CUSTOMISATION_Y] == 4) tempCol[menuSelected[MENU_CUSTOMISATION_X] + 4] = Color(1, 0, 0);
		else tempCol[menuSelected[MENU_CUSTOMISATION_Y]] = Color(1, 0, 0);

		int Size[6] = { 3,3,3,3 };

		modelStack.PushMatrix();
		modelStack.Rotate(-145, 0, 1, 0);
		modelStack.Rotate(-8, 1, 0, 0);
		
		modelStack.Translate(-8, 12, 10);	
		RenderText(meshList[GEO_TEXT], "Chassis: " + custChassis[vehiclePartSelect[0] + 1]->getName(), tempCol[0]);
		
		modelStack.Translate(0, -1.5, 0);	
		RenderText(meshList[GEO_TEXT], "Wheels: " + custWheel[vehiclePartSelect[1] + 1]->getName(), tempCol[1]);
		
		modelStack.Translate(0, -1.5, 0);	
		RenderText(meshList[GEO_TEXT], "Weapons: " + custWeapon[vehiclePartSelect[2] + 1]->getName(), tempCol[2]);

		modelStack.Translate(.25, -1.5, 0);	RenderText(meshList[GEO_TEXT], "Rotate <>", tempCol[3]);
		modelStack.Translate(-.25, -8, 0);	RenderText(meshList[GEO_TEXT], "Confirm", tempCol[4]);
		modelStack.Translate(20, 0, 0);		RenderText(meshList[GEO_TEXT], "Escape", tempCol[5]);

		modelStack.PopMatrix();

		// Show stats
		modelStack.PushMatrix();
		if (menuSelected[MENU_CUSTOMISATION_Y] == 0)
		{
			
		}
		else if (menuSelected[MENU_CUSTOMISATION_Y] == 1)
		{

		}
		else if (menuSelected[MENU_CUSTOMISATION_Y] == 2)
		{

		}
		else
		{

		}

		modelStack.PopMatrix();
	}
	
	// Render Menu Text
	if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_MAINMENU)
	{
		Color temp[4] = { Color(1,1,1),Color(1,1,1),Color(1,1,1),Color(1,1,1) };
		temp[menuSelected[MENU_MAIN]] = Color(1, 0, 0);

		int Size[4] = { 3,3,3,3 };

		Size[menuSelected[MENU_MAIN]] = 5;
		RenderTextOnScreen(meshList[GEO_TEXT], "Play", temp[0], Size[0], 3, 15, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], "Customise Vehicle", temp[1], Size[1], 3, 11, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], "Options", temp[2], Size[2], 3, 7, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], "Exit", temp[3], Size[3], 3, 3, 1);
	}

	// Render Options Text
	else if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_OPTIONS)
	{
		/*
		Color temp[2] = { Color(1,1,1), Color(1,1,1) };
		temp[menuSelected[MENU_OPTIONS]] = Color(1, 0, 0);

		int Size[2] = { 3,3 };

		Size[menuSelected[MENU_OPTIONS]] = 5;
		RenderTextOnScreen(meshList[GEO_TEXT], "Toggle Fullscreen", temp[0], Size[0], 3, 15, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], "Back", temp[1], Size[1], 3, 7, 1);
		*/
		OptionMenu::getInstance()->Render();
	}

	// Render Confirmation Window
	if (inWindow)
	{
		RenderSpriteOnScreen(meshList[GEO_UI], UI_SELECTED, 40 - 20, 32 - 10, 40, 20, Color(0, 0, 1));

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

	// Render Loading Icon
	if (fps < 40)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Loading...", Color(1, 0, 0), 5, 0, 1, 1);
	}

	// Render Debug Info
	if (showDebugInfo)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "FPS:" + std::to_string(fps), Color(0, 1, 0), 1, 0, 58, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], "Camera Position: " + std::to_string(camera.position.x) + ", " + std::to_string(camera.position.y) + 
			", " + std::to_string(camera.position.z), Color(0, 1, 0), 1, 0, 56, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], "Camera Target:   " + std::to_string(camera.target.x) + ", " + std::to_string(camera.target.y) +
			", " + std::to_string(camera.target.z), Color(0, 1, 0), 1, 0, 55, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], "Val:" + std::to_string(aniVal[ANI_BUFFER]), Color(0, 1, 0), 1, 0, 53, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], "Animation:" + std::to_string(animation[ANIS_ANY]), Color(0, 1, 0), 1, 0, 52, 1);
		
		if (Application::IsKeyPressed('X'))
		{
			for (int i = 0; i < 8; ++i) renderLightPos(i);
		}
	}
}

void VehicleScene::renderSkysphere(int size)
{
	modelStack.PushMatrix();
	modelStack.Rotate(aniVal[ANI_SKYBOX_ROTATION], 0, 1, 0);
	modelStack.Scale(size * 20, size * 20, size * 20);
	RenderMesh(meshList[GEO_SKYSPHERE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(size, size, size);

	RenderMesh(meshList[GEO_WAREHOUSE_RING], true);
	RenderMesh(meshList[GEO_WAREHOUSE_CENTRE], true);
	RenderMesh(meshList[GEO_WAREHOUSE_SHELL], true);
	
	modelStack.PushMatrix(); modelStack.Translate(0, 2 - aniVal[ANI_SHIP_TEETH_POSITION], 0); RenderMesh(meshList[GEO_WAREHOUSE_TEETH_TOP], true); modelStack.PopMatrix();
	modelStack.PushMatrix(); modelStack.Translate(0, aniVal[ANI_SHIP_TEETH_POSITION] - 2, 0); RenderMesh(meshList[GEO_WAREHOUSE_TEETH_BOTTOM], true); modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void VehicleScene::moveLight(double dt, int lightNum)
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

void VehicleScene::renderLightPos(int lightNum)
{
	modelStack.PushMatrix();
	modelStack.Translate(light[lightNum].position.x, light[lightNum].position.y, light[lightNum].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE], false);
	modelStack.PopMatrix();
}

void VehicleScene::CalculateLights()
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

void VehicleScene::RenderMesh(Mesh* mesh, bool enableLight, float BBSize)
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

void VehicleScene::RenderSprite(Mesh* mesh, int frameCount, Color color)
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

void VehicleScene::RenderText(Mesh* mesh, std::string text, Color color, int type)
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

void VehicleScene::RenderSpriteOnScreen(Mesh* mesh, int frameCount, float x, float y, float sizex, float sizey, Color color)
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
	modelStack.Scale(sizex, sizey, 1);

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

void VehicleScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, int type)
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

void VehicleScene::CalculateFPS()
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