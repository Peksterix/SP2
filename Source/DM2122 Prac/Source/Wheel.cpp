#include "Wheel.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Wheel::Wheel(int Type)
{
	health = 0;
	mesh = nullptr;
	type = Type;
	bbScale = 1.2;

	if (type == 0) 
	{ 
		health = 100; 
		mesh = MeshBuilder::GenerateOBJ("Wheel's Mesh", "obj//Wheel0.obj");
		mesh->textureID = LoadTGA("img//Vehicle0a.tga");
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