#pragma once
#include "Rendering/RenderPass.h"

struct TransformProperty : IRenderPassProperty {
	ShaderProperty <glm::mat4> matrixM{ "MATRIX_M",    pass };
	ShaderProperty <glm::mat4> matrixPV{ "MATRIX_PV",   pass };
	ShaderProperty <glm::mat3> matrixN{ "MATRIX_N",    pass };
	TransformProperty(RenderPass* pass) :IRenderPassProperty(pass) {}
	inline void SetMatrixMVP(const glm::mat4& PV, const glm::mat4& M)
	{
		matrixM.Set(M);
		matrixPV.Set(PV);
		matrixN.Set(glm::transpose(glm::inverse(M)));
	}

	void SetData(Renderer* renderer) override;
};

struct LitPassProperty :TransformProperty {
	using Super = TransformProperty;

	ShaderProperty <glm::mat4> matrixL{ "MATRIX_LIGHT",pass };
	ShaderProperty <glm::vec3> lightDir{ "light_dir",   pass };
	ShaderProperty <int> shadowMap{ "shadowMap",pass };
	//uint32_t shadowMapIndex;

	LitPassProperty(RenderPass* pass) :TransformProperty(pass) {
		shadowMap.Set(0);
	}


	inline void SetLightMatrix(const glm::mat4& lightMatrix)
	{
		matrixL.Set(lightMatrix);
	}

	inline void SetLightDirection(const glm::vec3& dir)
	{
		lightDir.Set(dir);
	}

	void SetData(Renderer* renderer) override;


};

struct ShadowPassProperty :TransformProperty {
	using Super = TransformProperty;

	ShaderProperty <glm::mat4> matrixL{ "MATRIX_LIGHT",pass };

	ShadowPassProperty(RenderPass* pass) :TransformProperty(pass) {}

	void SetData(Renderer* renderer) override;

	void ClearUp(Renderer* renderer) override;
};