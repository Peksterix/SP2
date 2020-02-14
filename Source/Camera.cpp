#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
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
}

void Camera::Update(double dt, int type)
{
	static const float CAMERA_SPEED = 20.f;
	static const float pi = 3.14159265358979323846f;
	static float cameraAngleX = 0, cameraAngleY = 0;
	
	if (type == 0)
	{
		if (Application::IsKeyPressed('W'))
		{
			//position.y += (float)(CAMERA_SPEED * 0.2 * dt);
			position.x += dt * CAMERA_SPEED * cos(cameraAngleX * (pi / 180));
			position.z += dt * CAMERA_SPEED * sin(cameraAngleX * (pi / 180));
			position.y += dt * CAMERA_SPEED * sin(cameraAngleY * (pi / 180));
		}
		if (Application::IsKeyPressed('S'))
		{
			position.x -= dt * CAMERA_SPEED * cos(cameraAngleX * (pi / 180));
			position.z -= dt * CAMERA_SPEED * sin(cameraAngleX * (pi / 180));
			position.y -= dt * CAMERA_SPEED * sin(cameraAngleY * (pi / 180));
		}
		if (Application::IsKeyPressed('A'))
		{
			position.x -= dt * CAMERA_SPEED * cos((cameraAngleX + 90.f) * (pi / 180));
			position.z -= dt * CAMERA_SPEED * sin((cameraAngleX + 90.f) * (pi / 180));
		}
		if (Application::IsKeyPressed('D'))
		{
			position.x += dt * CAMERA_SPEED * cos((cameraAngleX + 90.f) * (pi / 180));
			position.z += dt * CAMERA_SPEED * sin((cameraAngleX + 90.f) * (pi / 180));
		}
		if (Application::IsKeyPressed(VK_SPACE))	position.y += dt * CAMERA_SPEED;
		if (Application::IsKeyPressed(VK_CONTROL))	position.y -= dt * CAMERA_SPEED;

		if (Application::IsKeyPressed(VK_UP) && cameraAngleY <= 90) cameraAngleY += dt * CAMERA_SPEED * 4;
		if (Application::IsKeyPressed(VK_DOWN) && cameraAngleY >= -90)	cameraAngleY -= dt * CAMERA_SPEED * 4;
		if (Application::IsKeyPressed(VK_LEFT)) cameraAngleX -= dt * CAMERA_SPEED * 4;
		if (Application::IsKeyPressed(VK_RIGHT)) cameraAngleX += dt * CAMERA_SPEED * 4;

		target.x = position.x + CAMERA_SPEED * cos(cameraAngleX * (pi / 180));
		target.z = position.z + CAMERA_SPEED * sin(cameraAngleX * (pi / 180));
		target.y = position.y + CAMERA_SPEED * sin(cameraAngleY * (pi / 180));
	}
}