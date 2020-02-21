#include "VehicleScene.h"
#include "GL\glew.h"
#include <GLFW/glfw3.h>
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <string>
#include <MyMath.h>

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

extern GLFWwindow* m_window;
static float framesPerSecond;
static int fps;
static float lastTime;
static bool b_Entered = 0;

VehicleScene::VehicleScene()
{
	MenuMode = 1;
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
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

}

void VehicleScene::Init()
{

	#pragma region Initialise OpenGL

		//glClearColor(68.f / 255.f, 235.f / 255.f, 201.f / 255.f, 0.0f);
		glClearColor(0, 0, 0, 0);

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

		light[0].type = Light::LIGHT_POINT;
		light[0].position.Set(50, 50, 0);
		light[0].color.Set(61.f / 255.f, 128.f / 255.f, 37.f / 255.f);
		light[0].power = 3;
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

		light[1].type = Light::LIGHT_DIRECTIONAL;
		light[1].position.Set(25, 22, 0);
		light[1].color.Set(253.f / 255.f, 252.f / 255.f, 197.f / 255.f);
		light[1].power = 1;
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

		light[2].type = Light::LIGHT_SPOT;
		light[2].position.Set(-1, 0, 0);
		light[2].color.Set(61.f / 255.f, 128.f / 255.f, 37.f / 255.f);
		light[2].power = 2;
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

		light[3].type = Light::LIGHT_SPOT;
		light[3].position.Set(1, 0, 0);
		light[3].color.Set(61.f / 255.f, 128.f / 255.f, 37.f / 255.f);
		light[3].power = 2;
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
		light[4].position.Set(25, 25, 0);
		light[4].color.Set(214.f / 255.f, 1, 1);
		light[4].power = 2;
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
		light[5].position.Set(25, 25, 0);
		light[5].color.Set(0.f / 255.f, 1, 1);
		light[5].power = 2;
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

		light[6].type = Light::LIGHT_POINT;
		light[6].position.Set(25, 25, 0);
		light[6].color.Set(216.f / 255.f, 1, 1);
		light[6].power = 2;
		light[6].kC = 1.f;
		light[6].kL = 0.01f;
		light[6].kQ = 0.001f;
		light[6].cosCutoff = cos(Math::DegreeToRadian(45));
		light[6].cosInner = cos(Math::DegreeToRadian(30));
		light[6].exponent = 3.f;
		light[6].spotDirection.Set(0.f, 1.f, 0.f);
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

		light[7].type = Light::LIGHT_POINT;
		light[7].position.Set(25, 25, 0);
		light[7].color.Set(217.f / 255.f, 1, 1);
		light[7].power = 2;
		light[7].kC = 1.f;
		light[7].kL = 0.01f;
		light[7].kQ = 0.001f;
		light[7].cosCutoff = cos(Math::DegreeToRadian(45));
		light[7].cosInner = cos(Math::DegreeToRadian(30));
		light[7].exponent = 3.f;
		light[7].spotDirection.Set(0.f, 1.f, 0.f);
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
		/*if(state == S_LOADOUT){
		camera.Init(Position(-20 + 130 , 12, -26), Position(0 + 130, 4, -8), Position(0, 1, 0));
		}
		else if (state == S_MAINMENU)
		{
			camera.Init(Position(-103.677 + 110, -5.21155 + 12, 26.995 - 26), Position(130, 4, -8), Position(0, 1, 0));
		}*/
	#pragma endregion

	#pragma region Initialise Mesh
	
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		//meshList[GEO_TEXT]->textureID = LoadTGA("image//calibri.tga");
		meshList[GEO_TEXT]->textureID = LoadTGA("image//font.tga");
		meshList[GEO_WAREHOUSE_SHELL] = MeshBuilder::GenerateOBJ("Warehouse", "obj//Building2a.obj");
		meshList[GEO_WAREHOUSE_SHELL]->textureID = LoadTGA("image//Building2.tga");
		meshList[GEO_WAREHOUSE_RING] = MeshBuilder::GenerateOBJ("Warehouse", "obj//Building2b.obj");
		meshList[GEO_WAREHOUSE_RING]->textureID = LoadTGA("image//Building2.tga");
		meshList[GEO_WAREHOUSE_CENTRE] = MeshBuilder::GenerateOBJ("Warehouse", "obj//Building2c.obj");
		meshList[GEO_WAREHOUSE_CENTRE]->textureID = LoadTGA("image//Building2.tga");
		meshList[GEO_WAREHOUSE_BRIDGE] = MeshBuilder::GenerateOBJ("Warehouse", "obj//Building2d.obj");
		meshList[GEO_WAREHOUSE_BRIDGE]->textureID = LoadTGA("image//Building2.tga");
		meshList[GEO_SKYSPHERE] = MeshBuilder::GenerateOBJ("Skysphere (Space)", "obj//Skysphere.obj");
		meshList[GEO_SKYSPHERE]->textureID = LoadTGA("image//Flat Space1.tga");

		meshList[GEO_SHOWCASEFLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1,1,1), 20, 20);
		meshList[GEO_SHOWCASEFLOOR]->textureID = LoadTGA("image//ShowcaseFloor.tga");

		meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 1.f);
	
	#pragma endregion

	#pragma region Initialise Values

		Application::GetScreenSize(screenSizeX, screenSizeY);
		mainSelected = 0;
		showBoundingBox = 0;
		state = S_MAINMENU;
		if (state == S_LOADOUT) {
			camera.Init(Position(-20 + 130, 12, -26), Position(0 + 130, 4, -8), Position(0, 1, 0));
		}
		else if (state == S_MAINMENU)
		{
			camera.Init(Position(-14.42917, 18.038116 , 7.942698), Position(-18.365185, 18.168964, 7.590656), Position(0, 1, 0));
		}
		for (int i = 0; i < 3; ++i)
		{
			menuSelected[i] = 0;
			vehiclePartSelect[i] = 0;
		}
		for (int i = 0; i < 8; ++i)
		{
			vehicle[i] = new Vehicle;
			// Read the Vehicle's Saved Loadouts
		}
		// Temp
		vehicle[0]->setChassis(0);
		vehicle[0]->setWheel(0);
		vehicle[1]->setChassis(1);
		vehicle[1]->setWheel(1);
		vehicle[2]->setChassis(2);
		vehicle[2]->setWheel(2);

		for (int i = 0; i < 10; ++i)
		{
			bounceTime[i] = 0;
			animation[i] = 0;
			for (int j = 0; j < 10; ++j) aniVal[i][j] = 0;
		}
		aniVal[2][0] = 110;
		aniVal[2][1] = 12;
		aniVal[2][2] = -26;
		aniVal[2][3] = 130;
		aniVal[2][4] = 4;
		aniVal[2][5] = -8;

	#pragma endregion


}

void VehicleScene::Update(double dt)
{
	CalculateFPS();

	// Debug Options
	{
		if (Application::IsKeyPressed('7')) glDisable(GL_CULL_FACE);					// Disable Cull
		if (Application::IsKeyPressed('8')) glEnable(GL_CULL_FACE);						// Enable Cull
		if (Application::IsKeyPressed('9')) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Normal
		if (Application::IsKeyPressed('0')) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Wireframe
	}
	if (state == S_MAINMENU)
	{
		
		
		if (MenuMode == 1) {
			if (Application::IsKeyPressed('1'))
			{
				glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), NULL, NULL, screenSizeX, screenSizeY, 120);
			}


			if ((Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP)) && bounceTime[1] <= 0)
			{
				if (mainSelected != 0)
				{
					mainSelected--;
				}
				else
					mainSelected = 3;
				bounceTime[1] = 0.25f;
			}
			else if ((Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN)) && bounceTime[1] <= 0)
			{
				if (mainSelected != 3)
				{
					mainSelected++;
				}
				else
					mainSelected = 0;


				bounceTime[1] = 0.25f;
			}
			else if (Application::IsKeyPressed(VK_RETURN) && mainSelected == 3 && bounceTime[1] <= 0)
			{
				glfwSetWindowShouldClose(m_window, GL_TRUE);
				bounceTime[1] = 0.25f;
			}
			else if (Application::IsKeyPressed(VK_RETURN) && mainSelected == 2 && bounceTime[1] <= 0)
			{
				MenuMode = 2;
				bounceTime[1] = 0.25f;
			}
			else if (Application::IsKeyPressed(VK_RETURN) && mainSelected == 0 && bounceTime[1] <= 0)
			{
				state = S_CUTSCENE;
				bounceTime[1] = 0.25f;
			}
			if (bounceTime[1] > 0)
			{
				bounceTime[1] -= dt;
			}
		}
		else if (MenuMode == 2)
		{
			if ((Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN)) && bounceTime[1] <= 0)
			{
				if (OptionSelected != 2)
					OptionSelected++;
				else
					OptionSelected = 0;


				bounceTime[1] = 0.25f;
			}
			else if ((Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP)) && bounceTime[1] <= 0)
			{
				if (OptionSelected != 0)
					OptionSelected--;
				else
					OptionSelected = 2;


				bounceTime[1] = 0.25f;
			}
			else if (Application::IsKeyPressed(VK_RETURN) && OptionSelected == 0 && bounceTime[1] <= 0 && isFullScreen == 0)
			{
				isFullScreen = 1;
				glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), NULL, NULL, screenSizeX, screenSizeY, NULL);
				bounceTime[1] = 0.25f;
			}
			else if (Application::IsKeyPressed(VK_RETURN) && OptionSelected == 1 && bounceTime[1] <= 0 && isFullScreen)
			{
				isFullScreen = 0;
				glfwSetWindowMonitor(m_window, NULL, -2, 30, screenSizeX, screenSizeY, NULL);
			}
			else if (Application::IsKeyPressed(VK_RETURN) && OptionSelected == 2 && bounceTime[1] <= 0)
			{
				MenuMode = 1;
				bounceTime[1] = 0.25f;
			}
			if (bounceTime[1] > 0)
			{
				bounceTime[1] -= dt;
			}
		}
	}
	else if (state == S_LOADOUT)
	{
		if ((Application::IsKeyPressed('A') || Application::IsKeyPressed(VK_LEFT)) && !animation[1])
		{
			--menuSelected[2];
			animation[1] = 2;
		}
		if ((Application::IsKeyPressed('D') || Application::IsKeyPressed(VK_RIGHT)) && !animation[1])
		{
			++menuSelected[2];
			animation[1] = 1;
		}
		if (Application::IsKeyPressed(VK_RETURN) && !animation[1])
		{
			state = S_CUTSCENE;
			menuSelected[0] = 0;
			animation[2] = 1;

			if (vehicle[menuSelected[2]]->getChassis() != nullptr) vehiclePartSelect[0] = vehicle[menuSelected[2]]->getChassis()->getType();
			else vehiclePartSelect[0] = -1;

			if (vehicle[menuSelected[2]]->getWheel() != nullptr) vehiclePartSelect[1] = vehicle[menuSelected[2]]->getWheel()->getType();
			else vehiclePartSelect[1] = -1;

			if (vehicle[menuSelected[2]]->getWeapon() != nullptr) vehiclePartSelect[2] = vehicle[menuSelected[2]]->getWeapon()->getType();
			else vehiclePartSelect[2] = -1;
		}

		if (menuSelected[2] > 7) menuSelected[2] = 0;
		if (menuSelected[2] < 0) menuSelected[2] = 7;
	}
	else if (state == S_CUSTOMISE)
	{
		if ((Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP)) && bounceTime[0] <= 0)
		{
			--menuSelected[0];
			if (menuSelected[0] > 4) menuSelected[0] = 0;
			if (menuSelected[0] < 0) menuSelected[0] = 4;
			if (menuSelected[0] >= 0 && menuSelected[0] <= 2) menuSelected[1] = vehiclePartSelect[menuSelected[0]];
			else if (menuSelected[0] == 3) menuSelected[1] = showBoundingBox;
			else menuSelected[1] = 0;
			bounceTime[0] = 0.25f;
		}
		if ((Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN)) && bounceTime[0] <= 0)
		{
			++menuSelected[0];
			if (menuSelected[0] > 4) menuSelected[0] = 0;
			if (menuSelected[0] < 0) menuSelected[0] = 4;
			if (menuSelected[0] >= 0 && menuSelected[0] <= 2) menuSelected[1] = vehiclePartSelect[menuSelected[0]];
			else if (menuSelected[0] == 3) menuSelected[1] = showBoundingBox;
			else menuSelected[1] = 0;
			bounceTime[0] = 0.25f;
		}
		if ((Application::IsKeyPressed('A') || Application::IsKeyPressed(VK_LEFT)) && bounceTime[0] <= 0)
		{
			--menuSelected[1];
			bounceTime[0] = 0.25f;
			if (menuSelected[0] >= 0 && menuSelected[0] <= 2)
			{
				if (menuSelected[1] > 2) menuSelected[1] = -1;
				if (menuSelected[1] < -1) menuSelected[1] = 2;
				vehiclePartSelect[menuSelected[0]] = menuSelected[1];
				if (menuSelected[0] == 0) vehicle[menuSelected[2]]->setChassis(vehiclePartSelect[0]);
				if (menuSelected[0] == 1) vehicle[menuSelected[2]]->setWheel(vehiclePartSelect[1]);
			}
			else
			{
				if (menuSelected[1] > 1) menuSelected[1] = 0;
				if (menuSelected[1] < 0) menuSelected[1] = 1;
			}
		}
		if ((Application::IsKeyPressed('D') || Application::IsKeyPressed(VK_RIGHT)) && bounceTime[0] <= 0)
		{
			++menuSelected[1];
			bounceTime[0] = 0.25f;
			if (menuSelected[0] >= 0 && menuSelected[0] <= 2)
			{
				if (menuSelected[1] > 2) menuSelected[1] = -1;
				if (menuSelected[1] < -1) menuSelected[1] = 2;
				vehiclePartSelect[menuSelected[0]] = menuSelected[1];
				if (menuSelected[0] == 0) vehicle[menuSelected[2]]->setChassis(vehiclePartSelect[0]);
				if (menuSelected[0] == 1) vehicle[menuSelected[2]]->setWheel(vehiclePartSelect[1]);
			}
			else
			{
				if (menuSelected[1] > 1) menuSelected[1] = 0;
				if (menuSelected[1] < 0) menuSelected[1] = 1;
			}
		}
		if (Application::IsKeyPressed(VK_RETURN) && menuSelected[0] == 4 && menuSelected[1] == 0)
		{
			state = S_CUTSCENE;
			animation[2] = 2;

			showBoundingBox = 0;
		}

		if (menuSelected[0] == 3 && menuSelected[1] == 1) showBoundingBox = 1;
		if (menuSelected[0] == 3 && menuSelected[1] == 0) showBoundingBox = 0;
	}
	
	else if (state == S_FREECAM)
	{
		camera.Update(dt,	Math::RadianToDegree( atan2(camera.target.x - camera.position.x, camera.target.z - camera.position.z) ),
							Math::RadianToDegree( atan2(camera.target.y - camera.position.y, sqrt(pow(camera.position.x - camera.target.x, 2) + pow(camera.position.z - camera.target.z, 2))) ) );
	}
	
	if (Application::IsKeyPressed('Z')) state = S_FREECAM;

	// Loadout Left & Right
	if (animation[1])
	{
		if (animation[1] == 1)
		{
			if (aniVal[1][0] < 360 / 8) aniVal[1][0] += dt * 90;
			else
			{
				animation[1] = 0;
				aniVal[1][0] = 0;
			}
		}
		else if (animation[1] == 2)
		{
			if (aniVal[1][0] > -360 / 8) aniVal[1][0] -= dt * 90;
			else
			{
				animation[1] = 0;
				aniVal[1][0] = 0;
			}
		}
	}

	// Loadout to Customise (Vice Versa)
	
	if (state == S_LOADOUT || state == S_CUTSCENE || state == S_CUSTOMISE )
		if (animation[2])
		{
			if (animation[2] == 1)
			{
				if (aniVal[2][0] > 0.132 + dt * 40)
				{
					//	ANI_VALUE	 += dt * ((Tar - Cur) / Dur)
					aniVal[2][0] += dt * ((.132 - 110.) / 1.);
					aniVal[2][1] += dt * ((55.6 - 12.) / 1.);
					aniVal[2][2] += dt * ((-52.9 - (-26.)) / 1.);

					aniVal[2][3] += dt * ((.714 - 130.) / 1.);
					aniVal[2][4] += dt * ((54.9 - 4.) / 1.);
					aniVal[2][5] += dt * ((-52.4 - (-8.)) / 1.);
					aniVal[0][9] += dt * 120;
					aniVal[0][8] += dt * 90;
				}
				else if (aniVal[0][9] < 180)
				{
					aniVal[2][0] = .132;
					aniVal[2][1] = 55.6;
					aniVal[2][2] = -52.9;
					aniVal[2][3] = .714;
					aniVal[2][4] = 54.9;
					aniVal[2][5] = -52.4;

					aniVal[0][9] += dt * 120;
					aniVal[0][8] += dt * 90;
				}
				else if (aniVal[0][8] < 270)
				{
					aniVal[0][9] = 180;

					aniVal[0][8] += dt * 120;
					aniVal[0][7] += dt * 10;
				}
				else if (aniVal[0][7] < 20)
				{
					aniVal[0][8] = 270;

					aniVal[0][7] += dt * 20;
					aniVal[0][6] += dt * 100;
				}
				else if (aniVal[0][6] < 130)
				{
					aniVal[0][7] = 20;

					aniVal[0][6] += dt * 65;
				}
				else if (aniVal[0][5] < 20)
				{
					aniVal[0][6] = 130;

					aniVal[0][5] += dt * 80;

					aniVal[2][0] -= dt * ((.132 - (-20.)) / 1.);
					aniVal[2][1] -= dt * ((55.6 - 12.) / 1.);
					aniVal[2][2] -= dt * ((-52.9 - (-26.)) / 1.);

					aniVal[2][3] -= dt * ((.714 - 0.) / 1.);
					aniVal[2][4] -= dt * ((54.9 - 4.) / 1.);
					aniVal[2][5] -= dt * ((-52.4 - (-4.)) / 1.);
				}
				else if (aniVal[2][0] > -20 + dt * 40)
				{
					aniVal[0][5] = 20;

					//	ANI_VALUE	 -= dt * ((Cur - Tar) / Dur)
					aniVal[2][0] -= dt * ((.132 - (-20.)) / 1.);
					aniVal[2][1] -= dt * ((55.6 - 12.) / 1.);
					aniVal[2][2] -= dt * ((-52.9 - (-26.)) / 1.);

					aniVal[2][3] -= dt * ((.714 - 0.) / 1.);
					aniVal[2][4] -= dt * ((54.9 - 4.) / 1.);
					aniVal[2][5] -= dt * ((-52.4 - (-4.)) / 1.);
				}
				else
				{
					aniVal[2][0] = -20;
					aniVal[2][1] = 12;
					aniVal[2][2] = -26;

					aniVal[2][3] = 0;
					aniVal[2][4] = 4;
					aniVal[2][5] = 4;

					animation[2] = 0;
					state = S_CUSTOMISE;
				}
			}

			if (animation[2] == 2)
			{
				if (aniVal[0][6] > 0)
				{
					aniVal[0][6] -= dt * 65;

					//	ANI_VALUE	 -= dt * ((Cur - Tar) / Dur)
					aniVal[2][0] -= dt * ((-20 - 110) / 2.);
					aniVal[2][1] -= dt * ((12 - 12) / 2.);
					aniVal[2][2] -= dt * ((-26 - (-26)) / 2.);
					aniVal[2][3] -= dt * ((0 - 130) / 2.);
					aniVal[2][4] -= dt * ((4 - 4) / 2.);
					aniVal[2][5] -= dt * ((4 - (-8)) / 2.);
				}
				else if (aniVal[0][8] > 0)
				{
					aniVal[0][6] = 0;
					aniVal[2][0] = 110;
					aniVal[2][1] = 12;
					aniVal[2][2] = -26;
					aniVal[2][3] = 130;
					aniVal[2][4] = 4;
					aniVal[2][5] = -8;

					aniVal[0][9] -= dt * 180;
					aniVal[0][8] -= dt * 270;
				}
				else
				{
					for (int i = 1; i < 10; ++i) aniVal[0][i] = 0;

					animation[2] = 0;
					state = S_LOADOUT;
				}
			}
			camera.Init(Position(aniVal[2][0], aniVal[2][1], aniVal[2][2]), Position(aniVal[2][3], aniVal[2][4], aniVal[2][5]), Position(0, 1, 0));

		}
		if (state == S_MAINMENU || state == S_CUTSCENE)
		{
			if (animation[3])
			{

			}
		}
	
		aniVal[0][0] += dt * 10;
		if (bounceTime > 0) bounceTime[0] -= dt;
	
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

	//std::cout << camera.position.x - 110 << '-' << camera.position.y - 12 << '-' << camera.position.z + 26 <<'N';
	modelStack.Translate(0, 0, 0);
	if (animation[1] == 1) modelStack.Rotate(aniVal[1][0] + (menuSelected[2] - 1) * 360 / 8, 0, 1, 0);
	else if (animation[1] == 2) modelStack.Rotate(aniVal[1][0] + (menuSelected[2] + 1) * 360 / 8, 0, 1, 0);
	else modelStack.Rotate(aniVal[1][0] + menuSelected[2] * 360 / 8, 0, 1, 0);

	// Render Vehicles
	for (int j = 0, angle = 0; j < 8; ++j, angle += 360 / 8)
	{
		modelStack.PushMatrix();

		// If vehicle is not empty:
		vehicle[j]->position.Set(130 * cos(Math::DegreeToRadian(angle)), 2, 130 * sin(Math::DegreeToRadian(angle)));
		vehicle[j]->rotate.Set(0, 180 - angle, 0);
		
		vehicle[menuSelected[2]]->position.Set((130 - aniVal[0][6]) * cos(Math::DegreeToRadian(angle)), 2, (130 - aniVal[0][6]) * sin(Math::DegreeToRadian(angle)));
		vehicle[menuSelected[2]]->rotate.Set(0, 180 - angle - aniVal[0][8], aniVal[0][5] - aniVal[0][7]);

		if (!(vehicle[j]->getChassis() == nullptr && vehicle[j]->getWheel() == nullptr && vehicle[j]->getWeapon() == nullptr))
		{
			modelStack.Translate(vehicle[j]->position.x, vehicle[j]->position.y, vehicle[j]->position.z);
			modelStack.Rotate(vehicle[j]->rotate.x, 0, 0, 1);
			modelStack.Rotate(vehicle[j]->rotate.y, 0, 1, 0);
			modelStack.Rotate(vehicle[j]->rotate.z, 1, 0, 0);
			
			modelStack.PushMatrix();

			if (vehicle[j]->getChassis() != nullptr)
			{
				modelStack.Translate(0, vehicle[j]->getChassis()->getMesh()->size.y / 2, 0);
				RenderMesh(vehicle[j]->getChassis()->getMesh(), false, vehicle[j]->getChassis()->getBBScale() * showBoundingBox);
			}
			if (vehicle[j]->getWheel() != nullptr)
				for (int i = 0; i < vehicle[j]->getChassis()->wheelPos.size(); ++i)
				{
					modelStack.PushMatrix();
					modelStack.Translate(vehicle[j]->wheelPos[i].x, vehicle[j]->wheelPos[i].y, vehicle[j]->wheelPos[i].z);
					modelStack.Rotate(aniVal[0][0] * 2 + aniVal[0][6] * 2, 1, 0, 0);
					modelStack.Scale(vehicle[j]->wheelScale[i].x, vehicle[j]->wheelScale[i].y, vehicle[j]->wheelScale[i].z);
					RenderMesh(vehicle[j]->getWheel()->getMesh(), false, vehicle[j]->getWheel()->getBBScale() * showBoundingBox);
					modelStack.PopMatrix();
				}
			if (vehicle[j]->getWeapon() != nullptr)
			{

			}

			modelStack.PopMatrix();
			
		}
		modelStack.PopMatrix();
	}
	
	renderSkysphere(26.5);

	// Render Text
	if (state != S_CUSTOMISE && state != S_MAINMENU) {
		for (int j = 0, angle = 0; j < 8; ++j, angle += 360 / 8)
		{
			modelStack.PushMatrix();
			modelStack.Translate(140 * cos(Math::DegreeToRadian(angle)), 11, 140 * sin(Math::DegreeToRadian(angle)));
			modelStack.Rotate(-angle - 90, 0, 1, 0);
			modelStack.Scale(5, 5, 5);
			if (menuSelected[2] == j) RenderText(meshList[GEO_TEXT], "Slot " + std::to_string(j + 1), Color(1, 0, 0));
			else RenderText(meshList[GEO_TEXT], "Slot " + std::to_string(j + 1), Color(1, 1, 1));

			if (vehicle[j]->getChassis() == nullptr && vehicle[j]->getWheel() == nullptr && vehicle[j]->getWeapon() == nullptr)
			{
				modelStack.Translate(0, -1, 0);
				RenderText(meshList[GEO_TEXT], "Empty", Color(1, 0, 0));
			}
			modelStack.PopMatrix();
		}
	}
	
		modelStack.PopMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Camera Position: " + std::to_string(camera.position.x) + ", " + std::to_string(camera.position.y) + ", " + std::to_string(camera.position.z), Color(0, 1, 0), 1, 0, 55);
		RenderTextOnScreen(meshList[GEO_TEXT], "Camera Target:   " + std::to_string(camera.target.x) + ", " + std::to_string(camera.target.y) + ", " + std::to_string(camera.target.z), Color(0, 1, 0), 1, 0, 54);
#pragma region Bridge

	modelStack.PushMatrix();
	modelStack.Rotate(-90 + aniVal[0][9], 0, 1, 0);
	modelStack.Scale(26.5, 26.5, 26.5);
	RenderMesh(meshList[GEO_WAREHOUSE_BRIDGE], false);

	modelStack.PopMatrix();

#pragma endregion
	if (state == S_MAINMENU && MenuMode == 1)
	{
		Color temp[4] = { Color(1,1,1),Color(1,1,1),Color(1,1,1),Color(1,1,1) };
		temp[mainSelected] = Color(1, 0, 0);

		int Size[4] = { 3,3,3,3 };

		Size[mainSelected] = 5;
		RenderTextOnScreen(meshList[GEO_TEXT], "New Game", temp[0], Size[0], 3, 15);
		RenderTextOnScreen(meshList[GEO_TEXT], "Load Game", temp[1], Size[1], 3, 11);
		RenderTextOnScreen(meshList[GEO_TEXT], "Options", temp[2], Size[2], 3, 7);
		RenderTextOnScreen(meshList[GEO_TEXT], "Exit", temp[3], Size[3], 3, 3);
	}
	else if (state == S_MAINMENU && MenuMode == 2)
	{
		Color temp[3] = { Color(1,1,1),Color(1,1,1),Color(1,1,1) };
		temp[OptionSelected] = Color(1, 0, 0);

		int Size[4] = { 3,3,3,3 };

		Size[OptionSelected] = 5;
		RenderTextOnScreen(meshList[GEO_TEXT], "Set to Full Screen", temp[0], Size[0], 3, 15);
		RenderTextOnScreen(meshList[GEO_TEXT], "Set to Window mode", temp[1], Size[1], 3, 11);
		RenderTextOnScreen(meshList[GEO_TEXT], "Back", temp[2], Size[2], 3, 7);
	}
	/*
	// Render Text on Screen
	if (state == S_CUSTOMISE)
	{
		Color tempCol[6] = { Color(0, 0, 0), Color(0, 0, 0), Color(0, 0, 0), Color(0, 0, 0), Color(0, 0, 0), Color(0, 0, 0) };
		if (menuSelected[0] != 4) tempCol[menuSelected[0]] = Color(1, 0, 0);
		else tempCol[menuSelected[1] + 4] = Color(1, 0, 0);
		RenderTextOnScreen(meshList[GEO_TEXT], "Chassis:" + std::to_string(vehiclePartSelect[0]), tempCol[0], 2, 0, 50);
		RenderTextOnScreen(meshList[GEO_TEXT], "Wheels:" + std::to_string(vehiclePartSelect[1]), tempCol[1], 2, 0, 40);
		RenderTextOnScreen(meshList[GEO_TEXT], "Weapons:" + std::to_string(vehiclePartSelect[2]), tempCol[2], 2, 0, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "BoundingBox:" + std::to_string(showBoundingBox), tempCol[3], 2, 0, 20);
		RenderTextOnScreen(meshList[GEO_TEXT], "Confirm", tempCol[4], 3, 0, 2);
		RenderTextOnScreen(meshList[GEO_TEXT], "Escape", tempCol[5], 3, 62, 2);
	}
	*/

	{
		Color tempCol[6] = { Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };
		if (menuSelected[0] != 4) tempCol[menuSelected[0]] = Color(1, 0, 0);
		else tempCol[menuSelected[1] + 4] = Color(1, 0, 0);

		modelStack.PushMatrix();
		modelStack.Rotate(-145, 0, 1, 0);
		modelStack.Rotate(-8, 1, 0, 0);
		modelStack.Translate(-8, 12, 10);	RenderText(meshList[GEO_TEXT], "Chassis:" + std::to_string(vehiclePartSelect[0]), tempCol[0]);
		modelStack.Translate(0, -1.5, 0);	RenderText(meshList[GEO_TEXT], "Wheels:" + std::to_string(vehiclePartSelect[1]), tempCol[1]);
		modelStack.Translate(0, -1.5, 0);	RenderText(meshList[GEO_TEXT], "Weapons:" + std::to_string(vehiclePartSelect[2]), tempCol[2]);
		modelStack.Translate(1, -1.5, 0);	RenderText(meshList[GEO_TEXT], "BoundingBox:" + std::to_string(showBoundingBox), tempCol[3]);
		modelStack.Translate(-1, -8, 0);	RenderText(meshList[GEO_TEXT], "Confirm", tempCol[4]);
		modelStack.Translate(20, 0, 0);		RenderText(meshList[GEO_TEXT], "Escape", tempCol[5]);
		modelStack.PopMatrix();
	}
	RenderTextOnScreen(meshList[GEO_TEXT], "FPS:" + std::to_string(fps), Color(0, 1, 0), 1, 0, 57);
	//RenderSpriteOnScreen(meshList[GEO_LOGO], 15, 60, 0, 20, 20);
}

void VehicleScene::renderSkysphere(int size)
{
	modelStack.PushMatrix();
	modelStack.Rotate(aniVal[0][0] * 0.05, 0, 1, 0);
	modelStack.Scale(size * 20, size * 20, size * 20);
	RenderMesh(meshList[GEO_SKYSPHERE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(size, size, size);

	RenderMesh(meshList[GEO_WAREHOUSE_RING], false);
	RenderMesh(meshList[GEO_WAREHOUSE_CENTRE], false);
	RenderMesh(meshList[GEO_WAREHOUSE_SHELL], false);

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
			if (i == 0) glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
			if (i == 1) glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
			if (i == 2) glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDirection_cameraspace.x);
			if (i == 3) glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightDirection_cameraspace.x);
			if (i == 4) glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightDirection_cameraspace.x);
			if (i == 5) glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightDirection_cameraspace.x);
			if (i == 6) glUniform3fv(m_parameters[U_LIGHT6_POSITION], 1, &lightDirection_cameraspace.x);
			if (i == 7) glUniform3fv(m_parameters[U_LIGHT7_POSITION], 1, &lightDirection_cameraspace.x);
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

void VehicleScene::RenderSprite(Mesh* mesh, int frameCount)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check return; 
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
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

void VehicleScene::RenderText(Mesh* mesh, std::string text, Color color)
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
		characterSpacing.SetToTranslation(i * .5f - (text.length() * 0.5f) / 2, 0, 0); //1.0f is the spacing of each character, you may change this value 
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing; 
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	} 
	glBindTexture(GL_TEXTURE_2D, 0); 
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0); 
	glEnable(GL_DEPTH_TEST);
}

void VehicleScene::RenderSpriteOnScreen(Mesh* mesh, int frameCount, float x, float y, float sizex, float sizey)
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

void VehicleScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
		characterSpacing.SetToTranslation(i * .5f, 0, 0); //1.0f is the spacing of each character, you may change this value 
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
