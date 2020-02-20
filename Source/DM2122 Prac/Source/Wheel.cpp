#include "Wheel.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Wheel::Wheel(int Type)
{
	health = bbScale = 0;
	mesh = nullptr;
	type = Type;

	if (type == 0) 
	{ 
		health = 100; 
		bbScale = 1.1;
		mesh = MeshBuilder::GenerateOBJ("Wheel's Mesh", "obj//Wheel0.obj");
		mesh->textureID = LoadTGA("image//Vehicle0a.tga");
	}
	if (type == 1)
	{
		health = 100;
		bbScale = 1.3;
		mesh = MeshBuilder::GenerateOBJ("Wheel's Mesh", "obj//Wheel1.obj");
		mesh->textureID = LoadTGA("image//Vehicle1a.tga");
	}
	if (type == 2)
	{
		health = 100;
		bbScale = 1;
		mesh = MeshBuilder::GenerateOBJ("Wheel's Mesh", "obj//Wheel2.obj");
		mesh->textureID = LoadTGA("image//Vehicle0a.tga");
	}
	if (type == 3)
	{
		health = 100;
		bbScale = 1;
		mesh = MeshBuilder::GenerateOBJ("Wheel's Mesh", "obj//Wheel3.obj");
		mesh->textureID = LoadTGA("image//Vehicle3a.tga");
	}
}

Wheel::~Wheel()
{

}

int Wheel::getHealth() { return health; }
int Wheel::getType() { return type; }
float Wheel::getBBScale() { return bbScale; }
Mesh* Wheel::getMesh() { return mesh; }

void Wheel::setHealth(int Health) { health = Health; }
void Wheel::setType(int Type) { type = Type; }
void Wheel::setBBScale(int BBScale) { bbScale = BBScale; }