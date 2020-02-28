#include "Weapon.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Weapon::Weapon(int Type)
{
	health = 0;
	mass = 0;
	magSize = 0;
	damage = 0;
	fireRate = 0;
	name = "None";

	mesh = nullptr;
	type = Type;

	if (type == 0)
	{
		name = "Temp 0";
		mass = 2;
		health = 50;
		bbScale = 1;
		magSize = 10;
		damage = 5;
		fireRate = 0.2f;
		mesh = MeshBuilder::GenerateOBJ("Weapon's Mesh", "obj//Weapon0.obj");
		mesh->textureID = LoadTGA("image//Vehicle0a.tga");
	}
	if (type == 1)
	{
		name = "Temp 1";
		mass = 7;
		health = 80;
		bbScale = 1;
		magSize = 5;
		damage = 20;
		fireRate = 0.5f;
		mesh = MeshBuilder::GenerateOBJ("Weapon's Mesh", "obj//Weapon1.obj");
		mesh->textureID = LoadTGA("image//Vehicle1a.tga");
	}
	if (type == 2)
	{
		name = "Temp 2";
		mass = 10;
		health = 60;
		bbScale = 1;
		magSize = 2;
		damage = 25;
		fireRate = 0.1f;
		mesh = MeshBuilder::GenerateOBJ("Weapon's Mesh", "obj//Weapon2.obj");
		mesh->textureID = LoadTGA("image//Vehicle2a.tga");
	}
	if (type == 3)
	{
		name = "Temp 3";
		mass = 5;
		health = 90;
		bbScale = 1;
		magSize = 10;
		damage = 10;
		fireRate = 0.5f;
		mesh = MeshBuilder::GenerateOBJ("Weapon's Mesh", "obj//Weapon3.obj");
		mesh->textureID = LoadTGA("image//Vehicle3a.tga");
	}
}

Weapon::~Weapon()
{

}

int Weapon::getHealth() { return health; }
int Weapon::getType() { return type; }
float Weapon::getBBScale() { return bbScale; }
std::string Weapon::getName() { return name; }
Mesh* Weapon::getMesh() { return mesh; }
float Weapon::getMass() { return mass; }
float Weapon::getDamage() { return damage; }
float Weapon::getMag() { return magSize; }
float Weapon::getFire() { return fireRate; }

void Weapon::setHealth(int Health) { health = Health; }
void Weapon::setType(int Type) { type = Type; }
void Weapon::setBBScale(int BBScale) { bbScale = BBScale; }
void Weapon::setMag(int mag) { magSize = mag; }