#include "MyScene.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

MyScene::MyScene()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

MyScene::~MyScene()
{
}

void MyScene::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.Init(Vector3(50, 25, 50), Vector3(0, 0, 0), Vector3(0, 1, 0));

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
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	/*
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
	*/
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	
	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(25, 25, 0);
	light[0].color.Set(0.5f, 0.5f, 0.5f);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	/*
	light[1].type = Light::LIGHT_SPOT;
	light[1].position.Set(25, 22, 0);
	light[1].color.Set(61.f / 255.f, 128.f / 255.f, 37.f / 255.f);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(1.f, 0.f, 0.f);
	*/

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
	/*
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
	*/
	glUniform1i(m_parameters[U_NUMLIGHTS], 1); 

	meshList[GEO_SKY] = MeshBuilder::GenerateOBJ("SkyDome", "OBJ//SkyDome.obj");
	meshList[GEO_SKY]->textureID = LoadTGA("Image//SkyDome Texture.tga");
	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(0, 0, 0), 1, 1);//MeshBuilder::GenerateOBJ("Floor", "OBJ//Floor.obj");
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//Floor Texture.tga");
	
	meshList[GEO_TRACK] = MeshBuilder::GenerateOBJ("Train Tracks", "OBJ//Tracks.obj");
	meshList[GEO_TRACK]->textureID = LoadTGA("Image//Track Texture.tga");

	meshList[GEO_CLOUD1] = MeshBuilder::GenerateOBJ("Cloud Layer 1", "OBJ//Floor.obj");
	meshList[GEO_CLOUD1]->textureID = LoadTGA("Image//Cloud1 Texture.tga");
	meshList[GEO_CLOUD2] = MeshBuilder::GenerateOBJ("Cloud Layer 2", "OBJ//Floor.obj");
	meshList[GEO_CLOUD2]->textureID = LoadTGA("Image//Cloud2 Texture.tga");
	meshList[GEO_CLOUD3] = MeshBuilder::GenerateOBJ("Cloud Layer 3", "OBJ//Floor.obj");
	meshList[GEO_CLOUD3]->textureID = LoadTGA("Image//Cloud3 Texture.tga");
	meshList[GEO_CLOUD4] = MeshBuilder::GenerateOBJ("Cloud Layer 4", "OBJ//Floor.obj");
	meshList[GEO_CLOUD4]->textureID = LoadTGA("Image//Cloud4 Texture.tga");
	meshList[GEO_CLOUD5] = MeshBuilder::GenerateOBJ("Cloud Layer 5", "OBJ//Floor.obj");
	meshList[GEO_CLOUD5]->textureID = LoadTGA("Image//Cloud5 Texture.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_DOOR] = MeshBuilder::GenerateOBJ("Door", "OBJ//Door.obj");
	meshList[GEO_DOOR]->textureID = LoadTGA("Image//Door Texture.tga");
	meshList[GEO_DOOR_HANDLE] = MeshBuilder::GenerateOBJ("Door Handle", "OBJ//Door Handle.obj");
	meshList[GEO_DOOR_HANDLE]->textureID = LoadTGA("Image//Door Texture.tga");

	meshList[GEO_TRAIN_ENGINE] = MeshBuilder::GenerateOBJ("Train Engine", "OBJ//Train Engine.obj");
	meshList[GEO_TRAIN_ENGINE]->textureID = LoadTGA("Image//Train Texture.tga");
	meshList[GEO_TRAIN_ENGINE_TRACK] = MeshBuilder::GenerateOBJ("Train Engine", "OBJ//Train Engine Track.obj");
	meshList[GEO_TRAIN_ENGINE_TRACK]->textureID = LoadTGA("Image//Train Texture.tga");
	meshList[GEO_UTRAIN] = MeshBuilder::GenerateOBJ("Unifished Train Car", "OBJ//Train Unfinished.obj");
	meshList[GEO_UTRAIN]->textureID = LoadTGA("Image//Train Texture.tga");
	meshList[GEO_TRAIN_CAR] = MeshBuilder::GenerateOBJ("Train Car", "OBJ//Train Car.obj");
	meshList[GEO_TRAIN_CAR]->textureID = LoadTGA("Image//Train Texture.tga");
	meshList[GEO_TRAIN_WHEEL] = MeshBuilder::GenerateOBJ("Train Car", "OBJ//Train Wheel.obj");
	meshList[GEO_TRAIN_WHEEL]->textureID = LoadTGA("Image//Train Texture.tga");

	meshList[GEO_TULIP_JACKET] = MeshBuilder::GenerateOBJ("Tulip Jacket", "OBJ//Tulip Jacket.obj");
	meshList[GEO_TULIP_JACKET]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_SKIRT] = MeshBuilder::GenerateOBJ("Tulip Skirt", "OBJ//Tulip Skirt.obj");
	meshList[GEO_TULIP_SKIRT]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_UPPERARM] = MeshBuilder::GenerateOBJ("Tulip Upper Arm", "OBJ//Tulip UArm.obj");
	meshList[GEO_TULIP_UPPERARM]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_FOREARM] = MeshBuilder::GenerateOBJ("Tulip Lower Arm", "OBJ//Tulip LArm.obj");
	meshList[GEO_TULIP_FOREARM]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_HAND] = MeshBuilder::GenerateOBJ("Tulip Hand", "OBJ//Tulip Hand.obj");
	meshList[GEO_TULIP_HAND]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_THUMBCON] = MeshBuilder::GenerateOBJ("Tulip Thumb Connector", "OBJ//Tulip ThumbCon.obj");
	meshList[GEO_TULIP_THUMBCON]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_THUMB] = MeshBuilder::GenerateOBJ("Tulip LThumb", "OBJ//Tulip Thumb.obj");
	meshList[GEO_TULIP_THUMB]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_THUMB1] = MeshBuilder::GenerateOBJ("Tulip UThumb", "OBJ//Tulip Thumb1.obj");
	meshList[GEO_TULIP_THUMB1]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_INDEX] = MeshBuilder::GenerateOBJ("Tulip LIndex", "OBJ//Tulip Index.obj");
	meshList[GEO_TULIP_INDEX]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_INDEX1] = MeshBuilder::GenerateOBJ("Tulip UIndex", "OBJ//Tulip Index1.obj");
	meshList[GEO_TULIP_INDEX1]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_MIDDLE] = MeshBuilder::GenerateOBJ("Tulip LMiddle", "OBJ//Tulip Middle.obj");
	meshList[GEO_TULIP_MIDDLE]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_MIDDLE1] = MeshBuilder::GenerateOBJ("Tulip UMiddle", "OBJ//Tulip Middle1.obj");
	meshList[GEO_TULIP_MIDDLE1]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_RING] = MeshBuilder::GenerateOBJ("Tulip LRing", "OBJ//Tulip Ring.obj");
	meshList[GEO_TULIP_RING]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_RING1] = MeshBuilder::GenerateOBJ("Tulip URing", "OBJ//Tulip Ring1.obj");
	meshList[GEO_TULIP_RING1]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_PINKY] = MeshBuilder::GenerateOBJ("Tulip LPinky", "OBJ//Tulip Pinky.obj");
	meshList[GEO_TULIP_PINKY]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_PINKY1] = MeshBuilder::GenerateOBJ("Tulip UPinky", "OBJ//Tulip Pinky1.obj");
	meshList[GEO_TULIP_PINKY1]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_NECK] = MeshBuilder::GenerateOBJ("Tulip Neck", "OBJ//Tulip Neck.obj");
	meshList[GEO_TULIP_NECK]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_HEAD] = MeshBuilder::GenerateOBJ("Tulip Head", "OBJ//Tulip Head.obj");
	meshList[GEO_TULIP_HEAD]->textureID = LoadTGA("Image//Tulip Texture.tga");
	meshList[GEO_TULIP_HAIR] = MeshBuilder::GenerateOBJ("Tulip Hair", "OBJ//Tulip Hair.obj");
	meshList[GEO_TULIP_HAIR]->textureID = LoadTGA("Image//Tulip Texture.tga");

	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 1.f), 9, 36, 1.f);
	
	train_deploy = 0;
	train_deploy1 = 0;
	cameraAngleX = -135;
	cameraAngleY = -15;
	
	animation1 = 0;
	animation2 = 0;
	animation3 = 0;
	animation4 = 0;
	skySphere_rotate = 0;
	a1_translate = 0;
	a1_rotate = 0;
	a2_translate1 = 0;
	a2_translate2 = 0;
	a2_translate3 = 0;
	a2_rotate1 = -45;
	a2_rotate2 = 0;
	a2_rotate3 = 0;
	a3_translate1 = -53;
	a3_translate2 = 21.5;
	a3_rotate1 = 0;
	animation_bounceTime = 0;
	a4_rotate1 = 0;
	a4_rotate2 = 0;
	a4_rotate3 = 0;
	a4_rotate4 = 0;
	a4_buffer1 = 0;
}

void MyScene::Update(double dt)
{
	// View Mode
	if (Application::IsKeyPressed('1')) glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2')) glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3')) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4')) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Camera Position
	{
		float	pi = 3.14159265358979323846f,
			camSpeed = 0.25f;

		if (Application::IsKeyPressed(VK_SHIFT)) camSpeed = 0.5;
		if (Application::IsKeyPressed(0x57)) /* W */ {
			camera.position.x += (cos(cameraAngleX * (pi / 180)) * camSpeed);
			camera.position.z += (sin(cameraAngleX * (pi / 180)) * camSpeed);
			camera.position.y += (sin(cameraAngleY * (pi / 180)) * camSpeed);
		}
		if (Application::IsKeyPressed(0x53)) /* S */ {
			camera.position.x += (-cos(cameraAngleX * (pi / 180)) * camSpeed);
			camera.position.z += (-sin(cameraAngleX * (pi / 180)) * camSpeed);
			camera.position.y += (-sin(cameraAngleY * (pi / 180)) * camSpeed);
		}
		if (Application::IsKeyPressed(0x41)) /* A */ {
			camera.position.x += (-cos((cameraAngleX + 90.f) * (pi / 180)) * camSpeed);
			camera.position.z += (-sin((cameraAngleX + 90.f) * (pi / 180)) * camSpeed);
		}
		if (Application::IsKeyPressed(0x44)) /* D */ {
			camera.position.x += (cos((cameraAngleX + 90.f) * (pi / 180)) * camSpeed);
			camera.position.z += (sin((cameraAngleX + 90.f) * (pi / 180)) * camSpeed);
		}
		if (Application::IsKeyPressed(VK_SPACE))	camera.position.y += camSpeed;
		if (Application::IsKeyPressed(VK_CONTROL))	camera.position.y -= camSpeed;
	}

	// Camera Direction	
	{
		if (Application::IsKeyPressed(VK_UP))
			if (cameraAngleY <= 90) cameraAngleY++;

		if (Application::IsKeyPressed(VK_DOWN))
			if (cameraAngleY >= -90) cameraAngleY--;

		if (Application::IsKeyPressed(VK_LEFT))
			cameraAngleX--;

		if (Application::IsKeyPressed(VK_RIGHT))
			cameraAngleX++;

		float	pi = 3.14159265358979323846f,
			camSpeedX = 8,
			camSpeedY = 8;

		camera.target.x = camSpeedX * cos(cameraAngleX * (pi / 180)) + camera.position.x;
		camera.target.z = camSpeedX * sin(cameraAngleX * (pi / 180)) + camera.position.z;
		camera.target.y = camSpeedY * sin(cameraAngleY * (pi / 180)) + camera.position.y;
	}

	if (Application::IsKeyPressed('I'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('5')) light[0].type = Light::LIGHT_POINT;
	if (Application::IsKeyPressed('6')) light[0].type = Light::LIGHT_DIRECTIONAL;
	if (Application::IsKeyPressed('7')) light[0].type = Light::LIGHT_SPOT;
	
	camera.Update(dt);

	skySphere_rotate += dt;
	
	if (Application::IsKeyPressed('M') && animation_bounceTime <= 0)
	{
		a1_rotate = 0;
		animation_bounceTime = 0.25;
		if (!animation1) animation1 = 1;
		else animation1 = 0;
	}
	if (Application::IsKeyPressed('N') && !animation2 && !animation3 && train_deploy1)
	{ 
		if (!train_deploy)
		{
			a2_rotate1 = -45;
			a2_rotate2 = 0;
			a2_translate1 = 0;
			a2_translate2 = 0;
			a2_translate3 = 0;
			train_deploy = 1;
		}
		else
		{
			a2_rotate1 = -12;
			a2_rotate2 = 12;
			a2_translate1 = -18.75;
			a2_translate2 = -165;
			a2_translate3 = -30;
			train_deploy = 0;
		}
		a2_rotate3 = 0;
		animation2 = 1;
	}
	if (Application::IsKeyPressed('B') && !animation2 && !animation3 && !train_deploy)
	{
		if (!train_deploy1)
		{
			a3_translate1 = -53;
			a3_translate2 = 21.5;
			train_deploy1 = 1;
		}
		else
		{
			a3_translate1 = -193;
			a3_translate2 = 0;
			train_deploy1 = 0;
		}
		a3_rotate1 = 0;
		animation3 = 1;
	}
	if (Application::IsKeyPressed('V') && !animation4)
	{
		a4_rotate1 = 0;
		a4_rotate2 = 0;
		a4_rotate3 = 0;
		a4_rotate4 = 0;
		a4_buffer1 = 0;
		animation4 = 1;
	}

	if (animation1)
	{
		a1_translate += dt * 40;
		a1_rotate += dt * 360;
	}

	if (animation2)
	{
		if (train_deploy)
		{
			if (a2_rotate1 < -12)	a2_rotate1 += dt * 15;
			else if (a2_rotate2 < 12)
			{
				a2_translate1 += dt * 2;
				a2_translate2 -= dt * 12;
				a2_rotate2 += dt * 12;
				a2_rotate3 += dt * 90;
			}
			else if (a2_translate3 > -30)
			{
				a2_translate3 -= dt * 10;
				a2_translate2 -= dt * 50;
				a2_rotate3 += dt * 270;
			}
			else if (a2_translate2 > -163)
			{
				a2_translate2 -= dt * 50;
				a2_rotate3 += dt * 270;
			}
			else if (a2_translate1 > -18.75)
			{
				a2_translate1 -= dt * 15;
				a2_rotate3 += dt * 90;
			}
			else if (a2_translate2 > -165)
			{
				a2_translate2 -= dt * 10;
				a2_rotate3 += dt * 270;
			}
			else animation2 = 0;
		}
		else
		{
			if (a2_translate2 < -163)
			{
				a2_translate2 += dt * 10;
				a2_rotate3 -= dt * 270;
			}
			else if (a2_translate1 < 2 && a2_translate2 < -100)
			{
				a2_translate1 += dt * 15;
				a2_rotate3 -= dt * 90;
			}
			else if (a2_translate3 < 0)
			{
				a2_translate3 += dt * 10;
				a2_translate2 += dt * 50;
				a2_rotate3 -= dt * 270;
			}
			else if (a2_translate2 < -12)
			{
				a2_translate2 += dt * 50;
				a2_rotate3 -= dt * 270;
			}
			else if (a2_rotate2 > 0)
			{
				a2_translate1 -= dt * 2;
				a2_translate2 += dt * 12;
				a2_rotate2 -= dt * 12;
				a2_rotate3 -= dt * 90;
			}
			else if (a2_rotate1 > -45)
			{
				a2_translate1 = 0; 
				a2_translate2 = 0; 
				a2_rotate1 -= dt * 15;
			}
			else animation2 = 0;
		}
	}

	if (animation3)
	{
		if (train_deploy1)
		{
			if (a3_translate1 > -193)
			{
				a3_translate1 -= dt * 50;
				a3_rotate1 += dt * 270;
			}
			else if (a3_translate2 > 0)
			{
				a3_translate2 -= dt * 15;
				a3_rotate1 += dt * 90;
			}
			else animation3 = 0;
		}
		else
		{
			if (a3_translate2 < 21.5)
			{
				a3_translate2 += dt * 15;
				a3_rotate1 -= dt * 90;
			}
			else if (a3_translate1 < -53)
			{
				a3_translate1 += dt * 50;
				a3_rotate1 -= dt * 270;
			}
			else animation3 = 0;
		}
	}

	if (animation4)
	{
		if (a4_rotate1 > -180)	a4_rotate1 -= dt * 180;
		else if (a4_rotate2 > -90) a4_rotate2 -= dt * 135;
		else if (a4_buffer1 < 1) a4_buffer1 += dt;
		else if (a4_rotate3 < 90) a4_rotate3 += dt * 135;
		else if (a4_rotate4 < 180) a4_rotate4 += dt * 180;
		else animation4 = 0;
	}

	if (animation_bounceTime > 0) animation_bounceTime -= dt;
}

void MyScene::Exit()
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

void MyScene::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	// passing the light direction if it is a direction light	
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction 
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	RenderSkybox(300);
	
	/*
	// Tulip
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 20);
	{
		// Head
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 4.141, 0);
			RenderMesh(meshList[GEO_TULIP_NECK], false);
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, 0.585, 0.006);
				RenderMesh(meshList[GEO_TULIP_HEAD], false);
				{
					modelStack.PushMatrix();
					modelStack.Translate(0, 1.865, -1.538);
					RenderMesh(meshList[GEO_TULIP_HAIR], false);
					modelStack.PopMatrix();
				}
				modelStack.PopMatrix();
			}
			modelStack.PopMatrix();
		}

		// Left Arm
		{
			modelStack.PushMatrix();
			modelStack.Translate(1.113, 3.052, -0.176);
			modelStack.Rotate(90, 0, 0, 1);
			RenderMesh(meshList[GEO_TULIP_UPPERARM], false);
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, -2.058, 0);
				RenderMesh(meshList[GEO_TULIP_FOREARM], false);
				{
					modelStack.PushMatrix();
					modelStack.Translate(0, -2.149, 0);
					RenderMesh(meshList[GEO_TULIP_HAND], false);

					//Thumb
					{
						modelStack.PushMatrix();
						modelStack.Translate(0.119, -0.067, 0);
						RenderMesh(meshList[GEO_TULIP_THUMBCON], false);
						{
							modelStack.PushMatrix();
							modelStack.Translate(0.125, -0.144, 0);
							RenderMesh(meshList[GEO_TULIP_THUMB], false);
							{
								modelStack.PushMatrix();
								modelStack.Translate(0.181, 0, 0);
								RenderMesh(meshList[GEO_TULIP_THUMB1], false);
								modelStack.PopMatrix();
							}
							modelStack.PopMatrix();
						}
						modelStack.PopMatrix();
					}
					//Index
					{
						modelStack.PushMatrix();
						modelStack.Translate(0.156, -0.477, 0);
						RenderMesh(meshList[GEO_TULIP_INDEX], false);
						{
							modelStack.PushMatrix();
							modelStack.Translate(0, -0.161, 0);
							RenderMesh(meshList[GEO_TULIP_INDEX1], false);
							modelStack.PopMatrix();
						}
						modelStack.PopMatrix();
					}
					//Middle
					{
						modelStack.PushMatrix();
						modelStack.Translate(0.05, -0.477, 0);
						RenderMesh(meshList[GEO_TULIP_MIDDLE], false);
						{
							modelStack.PushMatrix();
							modelStack.Translate(0, -0.217, 0);
							RenderMesh(meshList[GEO_TULIP_MIDDLE1], false);
							modelStack.PopMatrix();
						}
						modelStack.PopMatrix();
					}
					//Ring
					{
						modelStack.PushMatrix();
						modelStack.Translate(-0.056, -0.477, 0);
						RenderMesh(meshList[GEO_TULIP_RING], false);
						{
							modelStack.PushMatrix();
							modelStack.Translate(0, -0.194, 0);
							RenderMesh(meshList[GEO_TULIP_RING1], false);
							modelStack.PopMatrix();
						}
						modelStack.PopMatrix();
					}
					//Pinky
					{
						modelStack.PushMatrix();
						modelStack.Translate(-0.157, -0.477, 0);
						RenderMesh(meshList[GEO_TULIP_PINKY], false);
						{
							modelStack.PushMatrix();
							modelStack.Translate(0, -0.123, 0);
							RenderMesh(meshList[GEO_TULIP_PINKY1], false);
							modelStack.PopMatrix();
						}
						modelStack.PopMatrix();
					}

					modelStack.PopMatrix();
				}
				modelStack.PopMatrix();
			}
			modelStack.PopMatrix();
		}
		
		// Right Arm
		{
			modelStack.PushMatrix();
			modelStack.Translate(-1.113, 3.052, -0.176);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Rotate(-90, 0, 0, 1);
			RenderMesh(meshList[GEO_TULIP_UPPERARM], false);
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, -2.058, 0);
				RenderMesh(meshList[GEO_TULIP_FOREARM], false);
				{
					modelStack.PushMatrix();
					modelStack.Translate(0, -2.149, 0);
					RenderMesh(meshList[GEO_TULIP_HAND], false);

					//Thumb
					{
						modelStack.PushMatrix();
						modelStack.Translate(0.119, -0.067, 0);
						RenderMesh(meshList[GEO_TULIP_THUMBCON], false);
						{
							modelStack.PushMatrix();
							modelStack.Translate(0.125, -0.144, 0);
							RenderMesh(meshList[GEO_TULIP_THUMB], false);
							{
								modelStack.PushMatrix();
								modelStack.Translate(0.181, 0, 0);
								RenderMesh(meshList[GEO_TULIP_THUMB1], false);
								modelStack.PopMatrix();
							}
							modelStack.PopMatrix();
						}
						modelStack.PopMatrix();
					}
					//Index
					{
						modelStack.PushMatrix();
						modelStack.Translate(0.156, -0.477, 0);
						RenderMesh(meshList[GEO_TULIP_INDEX], false);
						{
							modelStack.PushMatrix();
							modelStack.Translate(0, -0.161, 0);
							RenderMesh(meshList[GEO_TULIP_INDEX1], false);
							modelStack.PopMatrix();
						}
						modelStack.PopMatrix();
					}
					//Middle
					{
						modelStack.PushMatrix();
						modelStack.Translate(0.05, -0.477, 0);
						RenderMesh(meshList[GEO_TULIP_MIDDLE], false);
						{
							modelStack.PushMatrix();
							modelStack.Translate(0, -0.217, 0);
							RenderMesh(meshList[GEO_TULIP_MIDDLE1], false);
							modelStack.PopMatrix();
						}
						modelStack.PopMatrix();
					}
					//Ring
					{
						modelStack.PushMatrix();
						modelStack.Translate(-0.056, -0.477, 0);
						RenderMesh(meshList[GEO_TULIP_RING], false);
						{
							modelStack.PushMatrix();
							modelStack.Translate(0, -0.194, 0);
							RenderMesh(meshList[GEO_TULIP_RING1], false);
							modelStack.PopMatrix();
						}
						modelStack.PopMatrix();
					}
					//Pinky
					{
						modelStack.PushMatrix();
						modelStack.Translate(-0.157, -0.477, 0);
						RenderMesh(meshList[GEO_TULIP_PINKY], false);
						{
							modelStack.PushMatrix();
							modelStack.Translate(0, -0.123, 0);
							RenderMesh(meshList[GEO_TULIP_PINKY1], false);
							modelStack.PopMatrix();
						}
						modelStack.PopMatrix();
					}

					modelStack.PopMatrix();
				}
				modelStack.PopMatrix();
			}
			modelStack.PopMatrix();
		}
		
		//Body
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0, -0.156);
			RenderMesh(meshList[GEO_TULIP_JACKET], false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			RenderMesh(meshList[GEO_TULIP_SKIRT], false);
			modelStack.PopMatrix();
		}

		// Left Leg
		{
			modelStack.PushMatrix();
			modelStack.Translate(0.638, -0.976, -0.176);
			RenderMesh(meshList[GEO_TULIP_UPPERLEG], false);
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, -1.82, 0);
				RenderMesh(meshList[GEO_TULIP_FORELEG], false);
				{
					modelStack.PushMatrix();
					modelStack.Translate(0, -2.19, 0);
					RenderMesh(meshList[GEO_TULIP_BOOT], false);
					{
						modelStack.PushMatrix();
						modelStack.Translate(0, -0.389, 0.254);
						RenderMesh(meshList[GEO_TULIP_BOTBOOT], false);
						modelStack.PopMatrix();
					}
					modelStack.PopMatrix();
				}
				modelStack.PopMatrix();
			}
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
	*/
	
	//Render Train Engine
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 1.543 * 2, 0);
		modelStack.Scale(2, 2, 2.2);
		RenderMesh(meshList[GEO_TRAIN_ENGINE], true);

		renderWheels(0, 11, 1, 1, 1, .9);

		//Render Engine Track
		modelStack.PushMatrix();
		modelStack.Translate(-12.5, 7.93, 0);
		modelStack.Rotate(a2_rotate1, 0, 0, 1);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_TRAIN_ENGINE_TRACK], true);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}
	
	//Render Uninished Train
	{
		modelStack.PushMatrix();
		modelStack.Translate(-24.8 + a2_translate2, 18.5 + a2_translate1, 0);
		modelStack.Rotate(a2_rotate1 + a2_rotate2, 0, 0, 1);
		modelStack.Scale(2, 2, 2.2);
		RenderMesh(meshList[GEO_UTRAIN], true);

		bool temp = 0;
		if (train_deploy && !animation2) temp = 1;
		else temp = 0;
		renderWheels(1.5, 5, temp, 0.88, 0.88, 0.88);

		modelStack.PopMatrix();
	}

	//Render Train Cars
	for (int i = 0, d = -53; i <= 3; ++i, d -= 35) {
		//Render Train Car
		modelStack.PushMatrix();
		modelStack.Translate(d, 0, 0);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_TRAIN_CAR], true);
		
		renderDoor(8.1, 8.45);
		renderDoor(-8.1, 8.45, 0, -90);

		renderWheels(1.543, 5);

		modelStack.PopMatrix();
	}
	
	//Render Last Train Car
	{
		modelStack.PushMatrix();
		modelStack.Translate(a3_translate1 + a2_translate3, a3_translate2, 0);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_TRAIN_CAR], true);

		renderDoor(8.1, 8.45);
		renderDoor(-8.1, 8.45, 0, -90);

		bool temp = 0;
		if (train_deploy1 && !animation3) temp = 1;
		else temp = 0;
		renderWheels(1.543, 5, temp);

		modelStack.PopMatrix();
	}

	// Render Train Cars
	for (int i = 0, d = -180; i <= 3; ++i, d -= 35) {
		//Render Train Car
		modelStack.PushMatrix();
		modelStack.Translate(d + a3_translate1/4 + a2_translate3, 0, 0);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_TRAIN_CAR], true);

		renderDoor(8.1, 8.45);
		renderDoor(-8.1, 8.45, 0, -90);

		renderWheels(1.543, 5);

		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 10, 20);
	RenderText(meshList[GEO_TEXT], "HELLO WORLD", Color(0, 1, 0));
	modelStack.PopMatrix();

	//RenderTextOnScreen(meshList[GEO_TEXT], "Hello World", Color(0, 1, 0), 2, 0, 0);
}

void MyScene::RenderMesh(Mesh* mesh, bool enableLight)
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

	if(mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}

void MyScene::RenderSkybox(int size)
{
	modelStack.PushMatrix();
		modelStack.Scale(size, size, size);
		RenderMesh(meshList[GEO_SKY], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		modelStack.Translate(-a1_translate, 0, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(size*2, size*2, size*2);
		RenderMesh(meshList[GEO_FLOOR], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		modelStack.Translate(size * 2 - a1_translate, 0, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(size * 2, size * 2, size * 2);
		RenderMesh(meshList[GEO_FLOOR], false);
	modelStack.PopMatrix();

	if (a1_translate >= size * 2) a1_translate = 0;

	modelStack.PushMatrix();
	modelStack.Translate(0, (size / 100) * 65, 0);

	modelStack.PushMatrix();
		modelStack.Translate(0, 2, 0);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Rotate(skySphere_rotate-30, 0, 1, 0);
		modelStack.Scale((size / 10) * 6, (size / 10) * 6, (size / 10) * 6);
		RenderMesh(meshList[GEO_CLOUD4], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		modelStack.Translate(0, 3, 0);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Rotate(skySphere_rotate+30, 0, 1, 0);
		modelStack.Scale((size / 10) * 7, (size / 10) * 7, (size / 10) * 7);
		RenderMesh(meshList[GEO_CLOUD3], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		modelStack.Translate(0, 1, 0);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Rotate(skySphere_rotate, 0, 1, 0);
		modelStack.Scale((size / 10) * 5, (size / 10) * 5, (size / 10) * 5);
		RenderMesh(meshList[GEO_CLOUD2], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Rotate(skySphere_rotate*0.8 +20, 0, 1, 0);
		modelStack.Scale((size / 10) * 5, (size / 10) * 5, (size / 10) * 5);
		RenderMesh(meshList[GEO_CLOUD1], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		modelStack.Translate(0, -20, 0);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Rotate(skySphere_rotate - 180, 0, 1, 0);
		modelStack.Scale((size / 10) * 8, (size / 10) * 8, (size / 10) * 8);
		RenderMesh(meshList[GEO_CLOUD5], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	for (int i = -size; i < size * 3.1; i += 40) {
		modelStack.PushMatrix();
		modelStack.Translate(-a1_translate + i, 0, 0);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_TRACK], true);
		modelStack.PopMatrix();
	}
}

void MyScene::renderDoor(double tx, double ty, double tz, double ry)
{
	modelStack.PushMatrix();
	modelStack.Translate(tx, ty, tz);
	modelStack.Rotate(ry, 0, 1, 0);
	modelStack.Scale(0.025, 0.025, 0.025);
	{
		modelStack.PushMatrix();
		modelStack.Translate(-5.9, 0, 0);
		modelStack.Rotate(a4_rotate2 + a4_rotate3, 0, 1, 0);
		RenderMesh(meshList[GEO_DOOR], true);
		// Render Handle
		{
			modelStack.PushMatrix();
			modelStack.Translate(5.9, 10, 0);
			modelStack.Rotate(a4_rotate1 + a4_rotate4, 0, 0, 1);
			RenderMesh(meshList[GEO_DOOR_HANDLE], true);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(5.9, 0, 0);
		modelStack.Rotate(-a4_rotate2 - a4_rotate3 + 180, 0, 1, 0);
		RenderMesh(meshList[GEO_DOOR], true);
		// Render Handle
		{
			modelStack.PushMatrix();
			modelStack.Translate(5.9, 10, 0);
			modelStack.Rotate(-a4_rotate1 - a4_rotate4, 0, 0, 1);
			modelStack.PushMatrix();
			modelStack.Translate(0, -5, 0);
			RenderMesh(meshList[GEO_DOOR_HANDLE], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();
	
}

void MyScene::renderWheels(double ty, int num, bool rot, double sx, double sy, double sz)
{
	for (int i = 1, k = 0; i >= -1; i -= 2, k += 180) {
		modelStack.PushMatrix();
		modelStack.Translate(0, ty, 0);
		modelStack.Scale(sx, sy, sz);
		modelStack.Rotate(k, 0, 1, 0);

		for (int o = 1; o >= -1; o -= 2) {
			for (int j = 0; j <= ((num - 1) / 2); ++j) {
				modelStack.PushMatrix();
				modelStack.Translate(o * j * 3.1, 0, 0);
				if (rot) modelStack.Rotate(-a1_rotate * i, 0, 0, 1);
				else modelStack.Rotate((a2_rotate3 + a3_rotate1) * i, 0, 0, 1);
				RenderMesh(meshList[GEO_TRAIN_WHEEL], true);
				modelStack.PopMatrix();
			}
		}
		modelStack.PopMatrix();
	}
}

void MyScene::RenderText(Mesh* mesh, std::string text, Color color)
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

void MyScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
	modelStack.Scale(size, size, size); 
	modelStack.Translate(x, y, 0); 

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