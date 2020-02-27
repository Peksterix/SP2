#include "Bullet.h"
#include "MeshBuilder.h"
#include "GL\glew.h"
#include "LoadTGA.h"

Bullet::Bullet()
{
	playerID = 0;
	xDir = 0;
	zDir = 0;
	speed = 0;
	damage = 0;

	mesh = MeshBuilder::GenerateOBJ("Bullet", "obj//bullet.obj");
	mesh->textureID = LoadTGA("image//scifi texture.tga");
}

Bullet::Bullet(Position start, Position dir, float vel, float dam, int ID)
{
	position = start;

	xDir = dir.x;
	zDir = dir.z;
	speed = vel;
	damage = dam;

	playerID = ID;

	mesh = MeshBuilder::GenerateOBJ("Bullet", "obj//bullet.obj");
	mesh->textureID = LoadTGA("image//scifi texture.tga");
}

Bullet::~Bullet()
{

}

void Bullet::Update()
{
	position.x += xDir * speed;
	position.z += zDir * speed;


}

float Bullet::getDamage()
{
	return damage;
}

int Bullet::getID()
{
	return playerID;
}