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
	std::shared_ptr<RenderPass> ActivePass();

	template<>
	std::shared_ptr<RenderPass> ActivePass<LightMode::FORWARD>()
	{
		ActivePass(forwardPass);
		return forwardPass;
	}

	template<>
	std::shared_ptr<RenderPass> ActivePass<LightMode::SHADOW>()
	{
		ActivePass(shadowCast);
		return shadowCast;
	}

	RenderLayer GetRenderLayer();

};

