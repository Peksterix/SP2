#include "Wheel.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Wheel::Wheel(int Type)
{
	health = 0;
	mass = 0;
	bbScale = 0;
	friction = 0;
	speed = 0;
	
	name = "None";

	type = Type;
	mesh = nullptr;

	if (type == 0) 
	{ 
		name = "Temp 0";
		mass = 12;
		health = 100;
		bbScale = 1.1;
		friction = 1;
		speed = 1;
		mesh = MeshBuilder::GenerateOBJ("Wheel's Mesh", "obj//Wheel0.obj");
		mesh->textureID = LoadTGA("image//Vehicle0a.tga");
	}
	if (type == 1)
	{
		name = "Temp 1";
		mass = 9;
		health = 90;
		bbScale = 1.3;
		friction = 1;
		speed = 1;
		mesh = MeshBuilder::GenerateOBJ("Wheel's Mesh", "obj//Wheel1.obj");
		mesh->textureID = LoadTGA("image//Vehicle1a.tga");
	}
	if (type == 2)
	{
		name = "Temp 2";
		mass = 9;
		health = 90;
		bbScale = 1;
		friction = 1;
		speed = 1;
		mesh = MeshBuilder::GenerateOBJ("Wheel's Mesh", "obj//Wheel2.obj");
		mesh->textureID = LoadTGA("image//Vehicle0a.tga");
	}
	if (type == 3)
	{
		name = "Temp 3";
		mass = 10;
		health = 95;
		bbScale = 1;
		friction = 1;
		speed = 1;
		mesh = MeshBuilder::GenerateOBJ("Wheel's Mesh", "obj//Wheel3.obj");
		mesh->textureID = LoadTGA("image//Vehicle3a.tga");
	}
}

Wheel::~Wheel()
{

}

int Wheel::getHealth() { return health; }
int Wheel::getType() { return type; }
float Wheel::getMass() { return mass; }
float Wheel::getBBScale() { return bbScale; }
std::string Wheel::getName() { return name; }
Mesh* Wheel::getMesh() { return mesh; }

void Wheel::setHealth(int Health) { health = Health; }
void Wheel::setType(int Type) { type = Type; }
void Wheel::setBBScale(int BBScale) { bbScale = BBScale; }