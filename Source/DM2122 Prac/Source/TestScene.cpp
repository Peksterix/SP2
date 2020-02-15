#include "TestScene.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <string>

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

static float framesPerSecond;
static int fps;
static float lastTime;

TestScene::TestScene()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

TestScene::~TestScene()
{
}

void TestScene::Exit()
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

void TestScene::Init()
{

	#pragma region Initialise OpenGL

		glClearColor(68.f / 255.f, 235.f / 255.f, 201.f / 255.f, 0.0f);

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

	#pragma region Initialise Mesh
	
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		meshList[GEO_TEXT]->textureID = LoadTGA("image//calibri.tga");
		//meshList[GEO_LOGO] = MeshBuilder::GenerateText("Logo", 4, 4);
		//meshList[GEO_LOGO]->textureID = LoadTGA("Image//Infinity Train Logo.tga");

		meshList[GEO_SKY] = MeshBuilder::GenerateOBJ("Skysphere", "obj//Skysphere.obj");
		meshList[GEO_SKY]->textureID = LoadTGA("image//Skysphere.tga");
		meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1,1,1), 1, 1);

		meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 1.f);
	
	#pragma endregion

	#pragma region Initialise Values
		
		splitScreen = 0;
		Application::GetScreenSize(screenSizeX, screenSizeY);
		currentCam = 0;

	#pragma endregion

}

void TestScene::Update(double dt)
{
	CalculateFPS();

	// Debug Options
	{
		if (Application::IsKeyPressed('7')) glDisable(GL_CULL_FACE);					// Disable Cull
		if (Application::IsKeyPressed('8')) glEnable(GL_CULL_FACE);						// Enable Cull
		if (Application::IsKeyPressed('9')) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Normal
		if (Application::IsKeyPressed('0')) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Wireframe

		if (Application::IsKeyPressed('Z')) splitScreen = 1;
		if (Application::IsKeyPressed('X')) splitScreen = 0;
	}

	camera[0].Update(dt, 0);
	camera[1].Update(dt, 0);
}

void TestScene::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Viewport
	if (splitScreen)
	{
		// Left Half of the screen
		glViewport(0, 0, screenSizeX / 2, screenSizeY);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[currentCam].position.x, camera[currentCam].position.y, camera[currentCam].position.z,
			camera[currentCam].target.x, camera[currentCam].target.y, camera[currentCam].target.z,
			camera[currentCam].up.x, camera[currentCam].up.y, camera[currentCam].up.z);
		modelStack.LoadIdentity();
		
		CalculateLights();

		renderScene();

		// Right Half of the screen
		glViewport(screenSizeX / 2, 0, screenSizeX / 2, screenSizeY);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[currentCam + 1].position.x, camera[currentCam + 1].position.y, camera[currentCam + 1].position.z,
			camera[currentCam + 1].target.x, camera[currentCam + 1].target.y, camera[currentCam + 1].target.z,
			camera[currentCam + 1].up.x, camera[currentCam + 1].up.y, camera[currentCam + 1].up.z);
		modelStack.LoadIdentity();

		CalculateLights();

		renderScene();
	}
	else
	{
		// Full window	
		glViewport(0, 0, screenSizeX, screenSizeY);
		viewStack.LoadIdentity();
		viewStack.LookAt(camera[currentCam].position.x, camera[currentCam].position.y, camera[currentCam].position.z,
			camera[currentCam].target.x, camera[currentCam].target.y, camera[currentCam].target.z,
			camera[currentCam].up.x, camera[currentCam].up.y, camera[currentCam].up.z);
		modelStack.LoadIdentity();

		CalculateLights();

		renderScene();
	}
}

void TestScene::renderScene()
{
	//renderSkysphere(100);
	


	RenderTextOnScreen(meshList[GEO_TEXT], "FPS:" + std::to_string(fps), Color(0, 1, 0), 4, 0, 56);
	//RenderSpriteOnScreen(meshList[GEO_LOGO], 15, 60, 0, 20, 20);
}

void TestScene::renderSkysphere(int size)
{
	modelStack.PushMatrix();
	modelStack.Scale(size, size, size);
	RenderMesh(meshList[GEO_SKY], 0);
	modelStack.PopMatrix();
}

bool TestScene::CheckCollision(Mesh* one, Mesh* two, float x1, float y1, float z1, float scale1, float x2, float y2, float z2, float scale2)
{
	float	pi = 3.14159265358979323846f;

	bool collisionX = x1 + (one->mSize.x * scale1) + one->size.x * scale1 >= x2 + (two->mSize.x * scale2) &&
		x2 + (two->mSize.x * scale2) + two->size.x * scale2 >= x1 + (one->mSize.x * scale1);
	bool collisionY = y1 + (one->mSize.y * scale1) + one->size.y * scale1 >= y2 + (two->mSize.y * scale2) &&
		y2 + (two->mSize.y * scale2) + two->size.y * scale2 >= y1 + (one->mSize.y * scale1);
	bool collisionZ = z1 + (one->mSize.z * scale1) + one->size.z * scale1 >= z2 + (two->mSize.z * scale2) &&
		z2 + (two->mSize.z * scale2) + two->size.z * scale2 >= z1 + (one->mSize.z * scale1);

	return collisionX && collisionY && collisionZ;
}
/*
bool TestScene::CheckT1Coll(float x, float y, float z)
{
	return 
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_TRAIN_CAR], TulipPos[0][0] + x, TulipPos[0][1] + y, TulipPos[0][2] + z, 0.05, -53 * 2, 0, 0, 4) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_TRAIN_CAR], TulipPos[0][0] + x, TulipPos[0][1] + y, TulipPos[0][2] + z, 0.05, -88 * 2, 0, 0, 4) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_TRAIN_CAR], TulipPos[0][0] + x, TulipPos[0][1] + y, TulipPos[0][2] + z, 0.05, -123 * 2, 0, 0, 4) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_TRAIN_CAR], TulipPos[0][0] + x, TulipPos[0][1] + y, TulipPos[0][2] + z, 0.05, -158 * 2, 0, 0, 4) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_TRAIN_CAR], TulipPos[0][0] + x, TulipPos[0][1] + y, TulipPos[0][2] + z, 0.05, -180 * 2 + aniVal[2][0] / 4 + aniVal[1][2], 0, 0, 4) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_TRAIN_CAR], TulipPos[0][0] + x, TulipPos[0][1] + y, TulipPos[0][2] + z, 0.05, -215 * 2 + aniVal[2][0] / 4 + aniVal[1][2], 0, 0, 4) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_TRAIN_CAR], TulipPos[0][0] + x, TulipPos[0][1] + y, TulipPos[0][2] + z, 0.05, -250 * 2 + aniVal[2][0] / 4 + aniVal[1][2], 0, 0, 4) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_TRAIN_CAR], TulipPos[0][0] + x, TulipPos[0][1] + y, TulipPos[0][2] + z, 0.05, -285 * 2 + aniVal[2][0] / 4 + aniVal[1][2], 0, 0, 4) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_TRAIN_ENGINE], TulipPos[0][0] + x, TulipPos[0][1] + y, TulipPos[0][2] + z, 0.05, 0, 1.543 * 4, 0, 4.4) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_TRAIN_CAR], TulipPos[0][0] + x, TulipPos[0][1] + y, TulipPos[0][2] + z, 0.05, aniVal[2][0] + aniVal[1][2], aniVal[2][1], 0, 4) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_FLOOR], TulipPos[0][0] + x, TulipPos[0][1] + y, TulipPos[0][2] + z, 0.05, 0, -6.5, 0, 1200);
}

bool TestScene::CheckT2Coll(float x, float y, float z)
{
	return
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAIN_BENCH], TulipPos[1][0], TulipPos[1][1], TulipPos[1][2], 0.05f, -2, 0, 2, 1) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAIN_BENCH], TulipPos[1][0], TulipPos[1][1], TulipPos[1][2], 0.05f, -1, 0, 2, 1) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAIN_BENCH], TulipPos[1][0], TulipPos[1][1], TulipPos[1][2], 0.05f, 0, 0, 2, 1) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAIN_BENCH], TulipPos[1][0], TulipPos[1][1], TulipPos[1][2], 0.05f, 1, 0, 2, 1) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAIN_CAN], TulipPos[1][0], TulipPos[1][1], TulipPos[1][2], 0.05f, 2, 0, 2, 1) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAIN_HYDRANT], TulipPos[1][0], TulipPos[1][1], TulipPos[1][2], 0.05f, 1, 0, -1, 1) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAIN_HYDRANT], TulipPos[1][0], TulipPos[1][1], TulipPos[1][2], 0.05f, -2, 0, -1, 1) ||
		CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAIN_HYDRANT], TulipPos[1][0], TulipPos[1][1], TulipPos[1][2], 0.05f, 1, 0, 1, 1);
}
*/

void TestScene::moveLight(double dt, int lightNum)
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

void TestScene::renderLightPos(int lightNum)
{
	modelStack.PushMatrix();
	modelStack.Translate(light[lightNum].position.x, light[lightNum].position.y, light[lightNum].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE], false);
	modelStack.PopMatrix();
}

void TestScene::CalculateLights()
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

void TestScene::RenderMesh(Mesh* mesh, bool enableLight, bool showBB)
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
	
	if (showBB)
	{
		meshList[GEO_BOUNDINGBOX] = MeshBuilder::GenerateBoundingBox("Bounding Box", mesh->mSize.x, mesh->mSize.y, mesh->mSize.z,
			mesh->size.x + mesh->mSize.x, mesh->size.y + mesh->mSize.y, mesh->size.z + mesh->mSize.z);
		RenderMesh(meshList[GEO_BOUNDINGBOX], 0, 0);
	}

	if(mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}

void TestScene::RenderSprite(Mesh* mesh, int frameCount)
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

void TestScene::RenderText(Mesh* mesh, std::string text, Color color)
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
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value 
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing; 
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	} 
	glBindTexture(GL_TEXTURE_2D, 0); 
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0); 
	glEnable(GL_DEPTH_TEST);
}

void TestScene::RenderSpriteOnScreen(Mesh* mesh, int frameCount, float x, float y, float sizex, float sizey)
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

void TestScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value 
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

void TestScene::CalculateFPS()
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
