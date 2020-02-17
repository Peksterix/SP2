#include "Chassis.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Chassis::Chassis(int Type)
{
	health = 0;
	mesh = nullptr;
	type = Type;
	bbScale = 0.9;

	if (type == 0)
	{
		health = 100;
		mesh = MeshBuilder::GenerateOBJ("Chassis' Mesh", "obj//Chassis0.obj");
		mesh->textureID = LoadTGA("img//Vehicle0a.tga");
		
		wheelPos[0].Set(-3.35, -3.445*2, 9.725);
		wheelPos[1].Set(3.35, -3.445*2, 9.725);
		wheelPos[2].Set(-3.35, -2.313, 6.53);
		wheelPos[3].Set(3.35, -2.313, 6.53);
	}
	if (type == 1)
	{
		health = 100;
		mesh = MeshBuilder::GenerateOBJ("Chassis' Mesh", "obj//Chassis1.obj");
		mesh->textureID = LoadTGA("img//Vehicle1a.tga");

		wheelPos[0].Set(2.491, 0, 1.366);
		wheelPos[1].Set(-2.491, 0, 1.366);
		wheelPos[0].Set(2.491, 0, 2.478);
		wheelPos[1].Set(-2.491, 0, 2.478);
	}
}

Chassis::~Chassis()
{

}

int Chassis::getHealth() { return health; }
int Chassis::getType() { return type; }
float Chassis::getBBScale() { return bbScale; }
Mesh* Chassis::getMesh() { return mesh; }

void Chassis::setHealth(int Health) { health = Health; }
void Chassis::setType(int Type) { type = Type; }
void Chassis::setBBScale(int BBScale) { bbScale = BBScale; }