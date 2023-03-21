#pragma once
#include "../Core/GameObject.h"
#include "RenderPass.h"

class Light {

public:
	inline static Light* mainLight;
public:
	glm::vec3 dir;

	//shadow
	//bool shadowEnable;
	uint32_t depthMapFBO;
	uint32_t depthMap;

	uint32_t shadowWidth;
	uint32_t shadowHeight;

	Light();
	void SetMainLight();
	void SetDirection(glm::vec3 dir);
	void SetShadowMapSize(uint32_t width, uint32_t height);

	glm::mat4 WorldToShadowTransform();

	void TargetBindShadow();

	void ClearShadowMap();

	//void RenderShadow() {
	//	// 1. 首选渲染深度贴图
	//	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//	glClear(GL_DEPTH_BUFFER_BIT);
	//	ConfigureShaderAndMatrices();
	//	RenderScene();
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//	// 2. 像往常一样渲染场景，但这次使用深度贴图
	//	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	ConfigureShaderAndMatrices();
	//	glBindTexture(GL_TEXTURE_2D, depthMap);
	//	RenderScene();
	//}

};