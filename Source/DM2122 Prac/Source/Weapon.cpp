#include "Weapon.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Weapon::Weapon(int Type)
{
	health = 0;
	mesh = nullptr;
	type = Type;
	bbScale = 1;

	if (type == 0)
	{
		health = 100;
		mesh = MeshBuilder::GenerateOBJ("Chassis's Mesh", "obj//Weapon0.obj");
		mesh->textureID = LoadTGA("img//Vehicle0a.tga");
	}
}

Weapon::~Weapon()
{

}

int Weapon::getHealth() { return health; }
int Weapon::getType() { return type; }
float Weapon::getBBScale() { return bbScale; }

void Weapon::setHealth(int Health) { health = Health; }
void Weapon::setType(int Type) { type = Type; }
void Weapon::setBBScale(int BBScale) { bbScale = BBScale; }