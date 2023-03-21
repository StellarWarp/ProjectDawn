#pragma once
#include "Core/GameObject.h"

class Camera :public GameObject {
	inline static Camera* mainCamera = nullptr;

	uint32_t FBO;
public:
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float fov = 60.0f;
	int targetWidth = 0;
	int targetHeight = 0;
	float aspectRatio = 16.0f / 9.0f;


public:
	Camera()
	{
		if (mainCamera == nullptr)
		{
			mainCamera = this;
		}
	}
	~Camera()
	{
		if (mainCamera == this)
		{
			mainCamera = nullptr;
		}
	}
	static Camera* GetMainCamera()
	{
		return mainCamera;
	}

	glm::mat4 WorldToCameraTransform() const
	{
		return transform.WorldToObjectTransform();
	}

	glm::mat4 CameraToWorldTransform() const
	{
		return transform.ObjectToWorldTransform();
	}

	glm::mat4 CameraToScreenTransform() const
	{
		//return glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, nearPlane, farPlane);
		return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
	}

	glm::mat4 ScreenToCameraTransform() const
	{
		return glm::inverse(CameraToScreenTransform());
	}

	glm::mat4 WorldToScreenTransform() const
	{
		return CameraToScreenTransform() * WorldToCameraTransform();
	}

	glm::mat4 ScreenToWorldTransform() const
	{
		return CameraToWorldTransform() * ScreenToCameraTransform();
	}

	void TargetInfoReset(int width,int height)
	{
		targetWidth = width;
		targetHeight = height;
		aspectRatio = (float)width / (float)height;
	}

	void TagetBindCamera()
	{
		
		glViewport(0, 0, targetWidth, targetHeight);
		//glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}



};
