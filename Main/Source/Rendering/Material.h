#pragma once
#include "Core/Transform.h"
#include "RenderPass.h"

enum class LightMode {
	FORWARD,
	SHADOW
};


class Material {

	RenderPass* activePass;

	void ActivePass(std::shared_ptr<RenderPass>& pass);

	std::shared_ptr<RenderPass> forwardPass;
	std::shared_ptr<RenderPass> shadowCast;
public:
	std::wstring name;
public:
	template<LightMode mode>
	void SetPass(std::shared_ptr<RenderPass> pass)
	{
		if constexpr (mode == LightMode::FORWARD) forwardPass = pass;
		else if constexpr (mode == LightMode::SHADOW) shadowCast = pass;
		else {
			throw std::exception("SetPass: Type not supported");
		}
	}


	template<LightMode mode>
	void ActivePass(Renderer* renderer,auto OnDrawCall)
	{
		if constexpr (mode == LightMode::FORWARD)forwardPass->Active(renderer, OnDrawCall);
		else if constexpr (mode == LightMode::SHADOW)shadowCast->Active(renderer, OnDrawCall);
		else {
			throw std::exception("ActivePass: Type not supported");
		}
	}


	RenderLayer GetRenderLayer();

};

