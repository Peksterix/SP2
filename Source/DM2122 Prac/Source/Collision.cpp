#include "Collision.h"

Collision::Collision()
{
	
}

Collision::~Collision()
{

}

bool Collision::CheckCollision(Entity* one, Entity* two, Position offset1)
{
	bool collisionX = offset1.x + one->position.x + (one->mesh->mSize.x * one->bbScale * one->scale.x) + one->mesh->size.x * one->bbScale * one->scale.x >= two->position.x + (two->mesh->mSize.x * two->scale.x) &&
		two->position.x + (two->mesh->mSize.x * two->scale.x) + two->mesh->size.x * two->scale.x >= offset1.x + one->position.x + (one->mesh->mSize.x * one->bbScale * one->scale.x);
	bool collisionY = offset1.y + one->position.y + (one->mesh->mSize.y * one->bbScale * one->scale.y) + one->mesh->size.y * one->bbScale * one->scale.y >= two->position.y + (two->mesh->mSize.y * two->scale.y) &&
		two->position.y + (two->mesh->mSize.y * two->scale.y) + two->mesh->size.y * two->scale.y >= offset1.y + one->position.y + (one->mesh->mSize.y * one->bbScale * one->scale.y);
	bool collisionZ = offset1.z + one->position.z + (one->mesh->mSize.z * one->bbScale * one->scale.z) + one->mesh->size.z * one->bbScale * one->scale.z >= two->position.z + (two->mesh->mSize.z * two->scale.z) &&
		two->position.z + (two->mesh->mSize.z * two->scale.z) + two->mesh->size.z * two->scale.z >= offset1.z + one->position.z + (one->mesh->mSize.z * one->bbScale * one->scale.z);

	return collisionX && collisionY && collisionZ;
}