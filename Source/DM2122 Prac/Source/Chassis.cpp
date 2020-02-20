#include "Chassis.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Chassis::Chassis(int Type)
{
	health = 0;
	bbScale = 0;
	name = "None";

	mesh = nullptr;
	type = Type;

	if (type == 0)
	{
		name = "Temp 0";
		health = 100;
		bbScale = 0.6;
		mesh = MeshBuilder::GenerateOBJ("Chassis' Mesh", "obj//Chassis0.obj");
		mesh->textureID = LoadTGA("image//Vehicle0a.tga");
		
		wheelPos.push_back(Position(-3.35, -3.445, 9.725));
		wheelPos.push_back(Position(3.35, -3.445, 9.725));
		wheelPos.push_back(Position(-3.4, -3.445, -6));
		wheelPos.push_back(Position(3.4, -3.445, -6));
		wheelPos.push_back(Position(0, 0, 0));

		weaponPos.push_back(Position(3.35, 0, 0));
		weaponPos.push_back(Position(-3.35, 0, 0));

		for (int i = 0; i < 4; ++i) wheelScale.push_back(Position(1, 1, 1));
		wheelScale.push_back(Position(1, 2.9, 2.9));

		for (int i = 0; i < 2; ++i) weaponScale.push_back(Position(.4, .4, .4));
	}
	if (type == 1)
	{
		name = "Temp 1";
		health = 100;
		bbScale = 1.1;
		mesh = MeshBuilder::GenerateOBJ("Chassis' Mesh", "obj//Chassis1.obj");
		mesh->textureID = LoadTGA("image//Vehicle1a.tga");

		wheelPos.push_back(Position(3.5, 0, 3.4));
		wheelPos.push_back(Position(-3.5, 0, 3.4));
		wheelPos.push_back(Position(3.5, 0, -3.7));
		wheelPos.push_back(Position(-3.5, 0, -3.7));

		weaponPos.push_back(Position(0, 1, -3));

		for (int i = 0; i < 4; ++i) wheelScale.push_back(Position(1, 1, 1));
		
		weaponScale.push_back(Position(.4, .4, .4));
	}
	if (type == 2)
	{
		name = "Temp 2";
		health = 100;
		bbScale = 1;
		mesh = MeshBuilder::GenerateOBJ("Chassis' Mesh", "obj//Chassis2.obj");
		mesh->textureID = LoadTGA("image//Vehicle2a.tga");

		wheelPos.push_back(Position(4.118, -2.77, 8.57));
		wheelPos.push_back(Position(-4.118, -2.77, 8.57));
		wheelPos.push_back(Position(4.118, -2.77, -9.283));
		wheelPos.push_back(Position(-4.118, -2.77, -9.283));

		weaponPos.push_back(Position(4.118, 0, 8.57));
		weaponPos.push_back(Position(-4.118, 0, 8.57));
		weaponPos.push_back(Position(4.118, 0, -9.283));
		weaponPos.push_back(Position(-4.118, 0, -9.283));

		for (int i = 0; i < 4; ++i) wheelScale.push_back(Position(1, 1, 1));

		for (int i = 0; i < 4; ++i) weaponScale.push_back(Position(.4, .4, .4));
	}
	if (type == 3)
	{
		name = "Temp 3";
		health = 100;
		bbScale = 1;
		mesh = MeshBuilder::GenerateOBJ("Chassis' Mesh", "obj//Chassis3.obj");
		mesh->textureID = LoadTGA("image//Vehicle3a.tga");

		wheelPos.push_back(Position(3.52, -1.16, 6.58));
		wheelPos.push_back(Position(3.52, -1.16, -6.3));
		wheelPos.push_back(Position(-3.52, -1.16, 6.58));
		wheelPos.push_back(Position(-3.52, -1.16, -6.3));
		
		weaponPos.push_back(Position(-2, 3, -2));
		weaponPos.push_back(Position(0, 3, -2));
		weaponPos.push_back(Position(2, 3, -2));

		for (int i = 0; i < 4; ++i) wheelScale.push_back(Position(1, 1, 1));

		weaponScale.push_back(Position(.2, .2, .2));
		weaponScale.push_back(Position(.4, .4, .4));
		weaponScale.push_back(Position(.2, .2, .2));
	}
}

Chassis::~Chassis()
{
	wheelPos.clear(); 
	weaponPos.clear();
	wheelScale.clear();
	weaponScale.clear();
}

int Chassis::getHealth() { return health; }
int Chassis::getType() { return type; }
float Chassis::getBBScale() { return bbScale; }
std::string Chassis::getName() { return name; }
Mesh* Chassis::getMesh() { return mesh; }

void Chassis::setHealth(int Health) { health = Health; }
void Chassis::setType(int Type) { type = Type; }
void Chassis::setBBScale(int BBScale) { bbScale = BBScale; }