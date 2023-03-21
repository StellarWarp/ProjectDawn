#pragma once
#include "Core/Transform.h"
#include "RenderPass.h"

enum class LightMode {
	FORWARD,
	SHADOW
};

struct IShaderData{

	uint32_t location_Light;
	uint32_t location_ShadowMap;
	uint32_t shadowMapIndex;

	virtual void SetData(std::shared_ptr<RenderPass>& pass);
};

class Material {

	RenderPass* activePass;

	void ActivePass(std::shared_ptr<RenderPass>& pass);

	std::shared_ptr<RenderPass> forwardPass;
	std::shared_ptr<RenderPass> shadowCast;
public:
	std::wstring name;
public:
	IShaderData shaderData;
	template<LightMode mode>
	void SetPass(std::shared_ptr<RenderPass>);

	template<>
	void SetPass<LightMode::FORWARD>(std::shared_ptr<RenderPass> pass)
	{
		forwardPass = pass;
		shaderData.location_Light = forwardPass->GetLocation("MATRIX_LIGHT");
		shaderData.location_ShadowMap = forwardPass->GetLocation("shadowMap");
		shaderData.shadowMapIndex = 0;
		forwardPass->SetInt(shaderData.location_ShadowMap, shaderData.shadowMapIndex);

	}

	template<>
	void SetPass<LightMode::SHADOW>(std::shared_ptr<RenderPass> pass)
	{
		shadowCast = pass;
	}


	template<LightMode mode>
	void ActivePass();

	template<>
	void ActivePass<LightMode::FORWARD>()
	{
		ActivePass(forwardPass);
	}

	template<>
	void ActivePass<LightMode::SHADOW>()
	{
		ActivePass(shadowCast);
	}

	void SetMatrixMVP(const glm::mat4& PV, const glm::mat4& M);

	RenderLayer GetRenderLayer();

};

class Texture {
	std::wstring name;
	uint32_t textureID;
	uint32_t width;
	uint32_t height;
	uint32_t channels;
	uint32_t internalFormat;
	uint32_t dataFormat;
};

