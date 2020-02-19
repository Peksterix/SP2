#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"
#include <MyMath.h>

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const Position& pos, const Position& target, const Position& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
	cameraAngleX = cameraAngleY = 0;
}

void Camera::Update(double dt, Entity* obj, Position offset, Position targetOffset)
{
	// Follow Cam
	position.x = obj->position.x + offset.x * cos(Math::DegreeToRadian(obj->rotate.y)) + offset.z * sin(Math::DegreeToRadian(obj->rotate.y));
	position.z = obj->position.z + offset.z * cos(Math::DegreeToRadian(obj->rotate.y)) - offset.x * sin(Math::DegreeToRadian(obj->rotate.y));
	position.y = obj->position.y + offset.y;

	target.x = obj->position.x + targetOffset.x * cos(Math::DegreeToRadian(obj->rotate.y)) + targetOffset.z * sin(Math::DegreeToRadian(obj->rotate.y));
	target.z = obj->position.z + targetOffset.z * cos(Math::DegreeToRadian(obj->rotate.y)) - targetOffset.x * sin(Math::DegreeToRadian(obj->rotate.y));
	target.y = obj->position.y + targetOffset.y;
}
void Camera::Update(double dt, double angleX, double angleY)
{	
	// Free Cam
	static float CAMERA_SPEED = 25.f;
	if (angleX || angleY) { cameraAngleX = angleX; cameraAngleY = angleY; }
	if (Application::IsKeyPressed('W'))
	{
		//position.y += (float)(CAMERA_SPEED * 0.2 * dt);
		position.x += dt * CAMERA_SPEED * cos(Math::DegreeToRadian(cameraAngleX));
		position.z += dt * CAMERA_SPEED * sin(Math::DegreeToRadian(cameraAngleX));
		position.y += dt * CAMERA_SPEED * sin(Math::DegreeToRadian(cameraAngleY));
	}
	if (Application::IsKeyPressed('S'))
	{
		position.x -= dt * CAMERA_SPEED * cos(Math::DegreeToRadian(cameraAngleX));
		position.z -= dt * CAMERA_SPEED * sin(Math::DegreeToRadian(cameraAngleX));
		position.y -= dt * CAMERA_SPEED * sin(Math::DegreeToRadian(cameraAngleY));
	}
	if (Application::IsKeyPressed('A'))
	{
		position.x -= dt * CAMERA_SPEED * cos(Math::DegreeToRadian(cameraAngleX + 90.f));
		position.z -= dt * CAMERA_SPEED * sin(Math::DegreeToRadian(cameraAngleX + 90.f));
	}
	if (Application::IsKeyPressed('D'))
	{
		position.x += dt * CAMERA_SPEED * cos(Math::DegreeToRadian(cameraAngleX + 90.f));
		position.z += dt * CAMERA_SPEED * sin(Math::DegreeToRadian(cameraAngleX + 90.f));
	}
	if (Application::IsKeyPressed(VK_SPACE))	position.y += dt * CAMERA_SPEED;
	if (Application::IsKeyPressed(VK_CONTROL))	position.y -= dt * CAMERA_SPEED;

	if (Application::IsKeyPressed(VK_UP) && cameraAngleY <= 90) cameraAngleY += dt * CAMERA_SPEED;
	if (Application::IsKeyPressed(VK_DOWN) && cameraAngleY >= -90)	cameraAngleY -= dt * CAMERA_SPEED;
	if (Application::IsKeyPressed(VK_LEFT)) cameraAngleX -= dt * CAMERA_SPEED * 1;
	if (Application::IsKeyPressed(VK_RIGHT)) cameraAngleX += dt * CAMERA_SPEED * 1;

	if (Application::IsKeyPressed(VK_SHIFT)) CAMERA_SPEED = 50;
	else CAMERA_SPEED = 25;

	target.x = position.x + cos(Math::DegreeToRadian(cameraAngleX));
	target.z = position.z + sin(Math::DegreeToRadian(cameraAngleX));
	target.y = position.y + sin(Math::DegreeToRadian(cameraAngleY));

}

// Old Camera Logic
/*
else if (currentCam == 1)
{
	// Tulip Controls
	if (Application::IsKeyPressed('W') && !CheckT1Coll(cos(TulipDir[0][0] * (pi / 180.0)), 0, -sin(TulipDir[0][0] * (pi / 180.0))))
	{
		TulipPos[0][0] += (dt + cos(TulipDir[0][0] * (pi / 180.0))) * 0.25;
		TulipPos[0][2] -= (dt + sin(TulipDir[0][0] * (pi / 180.0))) * 0.25;
		TulipDir[1][0] = 90;
	}
	if (Application::IsKeyPressed('S') && !CheckT1Coll(-cos(TulipDir[0][0] * (pi / 180.0)), 0, sin(TulipDir[0][0] * (pi / 180.0))))
	{
		TulipPos[0][0] -= (dt + cos(TulipDir[0][0] * (pi / 180.0))) * 0.25;
		TulipPos[0][2] += (dt + sin(TulipDir[0][0] * (pi / 180.0))) * 0.25;
		TulipDir[1][0] = 270;
	}
	if (Application::IsKeyPressed('A') && !CheckT1Coll(cos((TulipDir[0][0] + 90) * (pi / 180.0)), 0, -sin((TulipDir[0][0] + 90) * (pi / 180.0))))
	{
		TulipPos[0][0] += (dt + cos((TulipDir[0][0] + 90) * (pi / 180.0))) * 0.25;
		TulipPos[0][2] -= (dt + sin((TulipDir[0][0] + 90) * (pi / 180.0))) * 0.25;
		TulipDir[1][0] = 180;
	}
	if (Application::IsKeyPressed('D') && !CheckT1Coll(-cos((TulipDir[0][0] + 90) * (pi / 180.0)), 0, sin((TulipDir[0][0] + 90) * (pi / 180.0))))
	{
		TulipPos[0][0] -= (dt + cos((TulipDir[0][0] + 90) * (pi / 180.0))) * 0.25;
		TulipPos[0][2] += (dt + sin((TulipDir[0][0] + 90) * (pi / 180.0))) * 0.25;
		TulipDir[1][0] = 0;
	}
	if (Application::IsKeyPressed(VK_SPACE) && !animation[4] && CheckT1Coll(0, -1, 0)) animation[4] = 1;

	// Tulip Direction
	if (Application::IsKeyPressed(VK_UP)) --TulipCam[0][3];
	if (Application::IsKeyPressed(VK_DOWN)) ++TulipCam[0][3];
	if (Application::IsKeyPressed(VK_LEFT)) TulipDir[0][0] += 2;
	if (Application::IsKeyPressed(VK_RIGHT)) TulipDir[0][0] -= 2;

	// Tulip Logic
	{
		// Jump
		TulipPos[0][1] += sin(aniVal[4][0] * (3.14159265358979323846f / 180.0)) * 0.5;

		// If below Tulip Collision,
		if (!CheckT1Coll(0, -1, 0) && !animation[4])
		{
			TulipPos[0][1] -= dt * 20;
		}

		// If Tulip Collided with Floor,
		if (CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_FLOOR], TulipPos[0][0], TulipPos[0][1] -1 , TulipPos[0][2], 0.05, 0, -6, 0, 1200) && animation[0])
		{
			TulipPos[0][0] -= dt * 40 * 2;
		}

		// If Tulip is out of Range
		if (TulipPos[0][0] <= -400 || TulipPos[0][0] >= 400 || TulipPos[0][1] <= -10 || TulipPos[0][2] <= -400 || TulipPos[0][2] >= 400)
		{
			TulipPos[0][0] = -120;
			TulipPos[0][1] = 100;
			TulipPos[0][2] = 0;
		}

		// If Tulip is not on the Unfinished Train,
		if (CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAINUP], TulipPos[0][0], TulipPos[0][1], TulipPos[0][2], 0.05, -24.8 * 2 + aniVal[1][1], 18.5 * 2 + aniVal[1][0], 0, 4.4))
		{
			TulipPos[0][0] = -120;
			TulipPos[0][1] = 100;
			TulipPos[0][2] = 0;
			currentCam = 2;
		}
	}

	TulipCam[0][0] = TulipPos[0][0] - 2 * cos(TulipDir[0][0] * (pi / 180.0));
	TulipCam[0][1] = TulipPos[0][1] + TulipCam[0][3] * 0.01;
	TulipCam[0][2] = TulipPos[0][2] + 2 * sin(TulipDir[0][0] * (pi / 180.0));

	camera[1].position.Set(TulipCam[0][0], TulipCam[0][1], TulipCam[0][2]);
	camera[1].target.Set(TulipPos[0][0], TulipPos[0][1], TulipPos[0][2]);
}
else if (currentCam == 2)
{
	// Tulip Position
	if (Application::IsKeyPressed('W') && !CheckT2Coll(cos(TulipDir[0][1] * (pi / 180.0)), 0, -sin(TulipDir[0][1] * (pi / 180.0))))
	{
		TulipPos[1][0] += (dt + cos(TulipDir[0][1] * (pi / 180.0))) * 0.025;
		TulipPos[1][2] -= (dt + sin(TulipDir[0][1] * (pi / 180.0))) * 0.025;
		TulipDir[1][1] = 90;
	}
	if (Application::IsKeyPressed('S') && !CheckT2Coll(-cos(TulipDir[0][1] * (pi / 180.0)), 0, sin(TulipDir[0][1] * (pi / 180.0))))
	{
		TulipPos[1][0] -= (dt + cos(TulipDir[0][1] * (pi / 180.0))) * 0.025;
		TulipPos[1][2] += (dt + sin(TulipDir[0][1] * (pi / 180.0))) * 0.025;
		TulipDir[1][1] = 270;
	}
	if (Application::IsKeyPressed('A') && !CheckT2Coll(cos((TulipDir[0][1] + 90) * (pi / 180.0)), 0, -sin((TulipDir[0][1] + 90) * (pi / 180.0))))
	{
		TulipPos[1][0] += (dt + cos((TulipDir[0][1] + 90) * (pi / 180.0))) * 0.025;
		TulipPos[1][2] -= (dt + sin((TulipDir[0][1] + 90) * (pi / 180.0))) * 0.025;
		TulipDir[1][1] = 180;
	}
	if (Application::IsKeyPressed('D') && !CheckT2Coll(-cos((TulipDir[0][1] + 90) * (pi / 180.0)), 0, sin((TulipDir[0][1] + 90) * (pi / 180.0))))
	{
		TulipPos[1][0] -= (dt + cos((TulipDir[0][1] + 90) * (pi / 180.0))) * 0.025;
		TulipPos[1][2] += (dt + sin((TulipDir[0][1] + 90) * (pi / 180.0))) * 0.025;
		TulipDir[1][1] = 0;
	}
	if (Application::IsKeyPressed(VK_SPACE) && !animation[4] && (CheckT2Coll(0, -1, 0) || CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAINBOT], TulipPos[1][0], TulipPos[1][1] - 0.35, TulipPos[1][2], 0.05f, 0, 0, 0, 1))) animation[4] = 1;

	// Tulip Direction
	if (Application::IsKeyPressed(VK_UP)) ++TulipCam[1][3];
	if (Application::IsKeyPressed(VK_DOWN)) --TulipCam[1][3];
	if (Application::IsKeyPressed(VK_LEFT)) TulipDir[0][1] += 2;
	if (Application::IsKeyPressed(VK_RIGHT)) TulipDir[0][1] -= 2;

	// Tulip Logic
	{
		// Jump
		TulipPos[1][1] += sin(aniVal[4][0] * (3.14159265358979323846f / 180.0)) * 0.05;

		// If below Tulip Collision,
		if (!CheckT2Coll(0, -1, 0) && !CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAINBOT], TulipPos[1][0], TulipPos[1][1] - 0.35, TulipPos[1][2], 0.05f, 0, 0, 0, 1) && !animation[4])
		{
			TulipPos[1][1] -= dt * 3;
		}

		// If Tulip is on the Unfinished Train,
		if (!CheckCollision(meshList[GEO_TULIP_JACKET], meshList[GEO_UTRAINUP], TulipPos[1][0], TulipPos[1][1], TulipPos[1][2], 0.05, 0, 0, 0, 1))
		{
			TulipPos[1][0] = -5;
			TulipPos[1][1] = 5;
			TulipPos[1][2] = 0;
			currentCam = 1;
		}
	}

	// Tulip Camera
	camera[2].position.Set(-32 - 24.8 * 2 + aniVal[1][1] - (aniVal[1][3] + aniVal[1][4]) / 4, 37 + 18.5 * 2 + aniVal[1][0] - (aniVal[1][3] + aniVal[1][4]) / 2, 0);
	camera[2].target.Set(-24.8 * 2 + aniVal[1][1], 5 + 18.5 * 2 + aniVal[1][0], 0);
}
else if (currentCam == 3)
{
	// OneOne Split
	if (Application::IsKeyPressed('Z') && animation_bounceTime <= 0)
	{
		animation_bounceTime = 0.25;

		if (!OneOneSplit)
		{
			OneOnePos[0][0] = OneOnePos[1][0] + 2;
			OneOnePos[0][1] = OneOnePos[1][1];
			OneOnePos[0][2] = OneOnePos[1][2] + 2;
			OneOneSplit = 1;
		}
		else
		{
			OneOnePos[0][0] = OneOnePos[1][0];
			OneOnePos[0][1] = OneOnePos[1][1];
			OneOnePos[0][2] = OneOnePos[1][2];
			OneOneSplit = 0;
		}
	}

	if (!OneOneSplit)
	{
		// OneOne Position
		if (Application::IsKeyPressed('W'))
		{
			OneOnePos[0][0] += (dt + cos(OneOneDir[0][0] * (pi / 180.0))) * 0.25;
			OneOnePos[0][2] -= (dt + sin(OneOneDir[0][0] * (pi / 180.0))) * 0.25;
			OneOneDir[0][1] = 90;
		}
		if (Application::IsKeyPressed('S'))
		{
			OneOnePos[0][0] -= (dt + cos(OneOneDir[0][0] * (pi / 180.0))) * 0.25;
			OneOnePos[0][2] += (dt + sin(OneOneDir[0][0] * (pi / 180.0))) * 0.25;
			OneOneDir[0][1] = 270;
		}
		if (Application::IsKeyPressed('A'))
		{
			OneOnePos[0][0] += (dt + cos((OneOneDir[0][0] + 90) * (pi / 180.0))) * 0.25;
			OneOnePos[0][2] -= (dt + sin((OneOneDir[0][0] + 90) * (pi / 180.0))) * 0.25;
			OneOneDir[0][1] = 180;
		}
		if (Application::IsKeyPressed('D'))
		{
			OneOnePos[0][0] -= (dt + cos((OneOneDir[0][0] + 90) * (pi / 180.0))) * 0.25;
			OneOnePos[0][2] += (dt + sin((OneOneDir[0][0] + 90) * (pi / 180.0))) * 0.25;
			OneOneDir[0][1] = 0;
		}

		// OneOne Direction
		if (Application::IsKeyPressed(VK_SPACE)) OneOnePos[0][1] += dt * 25;
		if (Application::IsKeyPressed(VK_CONTROL)) OneOnePos[0][1] -= dt * 25;
		if (Application::IsKeyPressed(VK_UP)) --OneOneCam[0][3];
		if (Application::IsKeyPressed(VK_DOWN)) ++OneOneCam[0][3];
		if (Application::IsKeyPressed(VK_LEFT)) OneOneDir[0][0] += 2;
		if (Application::IsKeyPressed(VK_RIGHT)) OneOneDir[0][0] -= 2;

		// OneOne Cam
		OneOneCam[0][0] = OneOnePos[0][0] - 4 * cos(OneOneDir[0][0] * (pi / 180.0));
		OneOneCam[0][1] = OneOnePos[0][1] + OneOneCam[0][3] * 0.01;
		OneOneCam[0][2] = OneOnePos[0][2] + 4 * sin(OneOneDir[0][0] * (pi / 180.0));

		camera[3].position.Set(OneOneCam[0][0], OneOneCam[0][1], OneOneCam[0][2]);
		camera[3].target.Set(OneOnePos[0][0], OneOnePos[0][1], OneOnePos[0][2]);
	}
	else
	{
		// Glad One Position
		if (Application::IsKeyPressed('W'))
		{
			OneOnePos[0][0] += (dt + cos(OneOneDir[0][0] * (pi / 180.0))) * 0.25;
			OneOnePos[0][2] -= (dt + sin(OneOneDir[0][0] * (pi / 180.0))) * 0.25;
		}
		if (Application::IsKeyPressed('S'))
		{
			OneOnePos[0][0] -= (dt + cos(OneOneDir[0][0] * (pi / 180.0))) * 0.25;
			OneOnePos[0][2] += (dt + sin(OneOneDir[0][0] * (pi / 180.0))) * 0.25;
		}
		// Glad One Direction
		if (Application::IsKeyPressed('A')) OneOneDir[0][0] += 2;
		if (Application::IsKeyPressed('D')) OneOneDir[0][0] -= 2;

		// Glad One Camera
		OneOneCam[0][0] = OneOnePos[0][0] - 4 * cos(OneOneDir[0][0] * (pi / 180.0));
		OneOneCam[0][1] = OneOnePos[0][1] + 1;
		OneOneCam[0][2] = OneOnePos[0][2] + 4 * sin(OneOneDir[0][0] * (pi / 180.0));

		camera[3].position.Set(OneOneCam[0][0], OneOneCam[0][1], OneOneCam[0][2]);
		camera[3].target.Set(OneOnePos[0][0], OneOnePos[0][1], OneOnePos[0][2]);

		// Sad One Position
		if (Application::IsKeyPressed(VK_UP))
		{
			OneOnePos[1][0] += (dt + cos(OneOneDir[1][0] * (pi / 180.0))) * 0.25;
			OneOnePos[1][2] -= (dt + sin(OneOneDir[1][0] * (pi / 180.0))) * 0.25;
		}
		if (Application::IsKeyPressed(VK_DOWN))
		{
			OneOnePos[1][0] -= (dt + cos(OneOneDir[1][0] * (pi / 180.0))) * 0.25;
			OneOnePos[1][2] += (dt + sin(OneOneDir[1][0] * (pi / 180.0))) * 0.25;
		}
		// Sad One Direction
		if (Application::IsKeyPressed(VK_LEFT)) OneOneDir[1][0] += 2;
		if (Application::IsKeyPressed(VK_RIGHT)) OneOneDir[1][0] -= 2;

		// Sad One Camera
		OneOneCam[1][0] = OneOnePos[1][0] - 4 * cos(OneOneDir[1][0] * (pi / 180.0));
		OneOneCam[1][1] = OneOnePos[1][1] + 1;
		OneOneCam[1][2] = OneOnePos[1][2] + 4 * sin(OneOneDir[1][0] * (pi / 180.0));

		camera[4].position.Set(OneOneCam[1][0], OneOneCam[1][1], OneOneCam[1][2]);
		camera[4].target.Set(OneOnePos[1][0], OneOnePos[1][1], OneOnePos[1][2]);
	}
}
*/