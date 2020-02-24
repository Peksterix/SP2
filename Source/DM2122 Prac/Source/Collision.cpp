#include "Collision.h"

Collision::Collision()
{
	
}

Collision::~Collision()
{

}

bool Collision::CheckCollision(Entity* one, Entity* two)
{
	bool collisionX = one->position.x + (one->mesh->mSize.x * one->scale.x) + one->mesh->size.x * one->scale.x >= two->position.x + (two->mesh->mSize.x * two->scale.x) &&
		two->position.x + (two->mesh->mSize.x * two->scale.x) + two->mesh->size.x * two->scale.x >= one->position.x + (one->mesh->mSize.x * one->scale.x);
	bool collisionY = one->position.y + (one->mesh->mSize.y * one->scale.y) + one->mesh->size.y * one->scale.y >= two->position.y + (two->mesh->mSize.y * two->scale.y) &&
		two->position.y + (two->mesh->mSize.y * two->scale.y) + two->mesh->size.y * two->scale.y >= one->position.y + (one->mesh->mSize.y * one->scale.y);
	bool collisionZ = one->position.z + (one->mesh->mSize.z * one->scale.z) + one->mesh->size.z * one->scale.z >= two->position.z + (two->mesh->mSize.z * two->scale.z) &&
		two->position.z + (two->mesh->mSize.z * two->scale.z) + two->mesh->size.z * two->scale.z >= one->position.z + (one->mesh->mSize.z * one->scale.z);

	return collisionX && collisionY && collisionZ;
}

