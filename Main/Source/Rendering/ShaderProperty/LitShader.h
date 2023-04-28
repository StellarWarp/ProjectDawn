#pragma once
#include "Rendering/RenderPass.h"

struct LitPassData :IRenderPassProperty {
	ShaderProperty <glm::mat4> matrixM{ "MATRIX_M",    pass };
	ShaderProperty <glm::mat4> matrixPV{ "MATRIX_PV",   pass };
	ShaderProperty <glm::mat3> matrixN{ "MATRIX_N",    pass };
	ShaderProperty <glm::mat4> matrixL{ "MATRIX_LIGHT",pass };
	ShaderProperty <glm::vec3> lightDir{ "light_dir",   pass };
	ShaderProperty <int> shadowMap{ "shadowMap",pass };
	//uint32_t shadowMapIndex;

	LitPassData(RenderPass* pass) :IRenderPassProperty(pass) {
		shadowMap.Set(0);
	}
	inline void SetMatrixMVP(const glm::mat4& PV, const glm::mat4& M)
	{
		matrixM.Set(M);
		matrixPV.Set(PV);
		matrixN.Set(glm::transpose(glm::inverse(M)));
	}

	inline void SetLightMatrix(const glm::mat4& lightMatrix)
	{
		matrixL.Set(lightMatrix);
	}

	inline void SetLightDirection(const glm::vec3& dir)
	{
		lightDir.Set(dir);
	}

};

struct ShadowPassData :IRenderPassProperty {
	ShaderProperty <glm::mat4> matrixM{ "MATRIX_M",    pass };
	ShaderProperty <glm::mat4> matrixPV{ "MATRIX_PV",   pass };
	ShaderProperty <glm::mat3> matrixN{ "MATRIX_N",    pass };
	ShaderProperty <glm::mat4> matrixL{ "MATRIX_LIGHT",pass };

	ShadowPassData(RenderPass* pass) :IRenderPassProperty(pass) {}
	inline void SetMatrixMVP(const glm::mat4& PV, const glm::mat4& M)
	{
		matrixM.Set(M);
		matrixPV.Set(PV);
		matrixN.Set(glm::transpose(glm::inverse(M)));
	}
};