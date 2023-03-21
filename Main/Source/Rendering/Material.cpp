#include "Material.h"
#include "Light.h"




void IShaderData::SetData(std::shared_ptr<RenderPass>& pass) {
	auto mainLight = Light::mainLight;
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mainLight->depthMap);

	pass->SetMat4(location_Light, mainLight->WorldToShadowTransform());
}

void Material::ActivePass(std::shared_ptr<RenderPass>& pass)
{
	pass->RuntimeUpdateCheck();
	pass->Use();
	shaderData.SetData(pass);
	activePass = pass.get();
}

void Material::SetMatrixMVP(const glm::mat4& PV, const glm::mat4& M)
{
	activePass->SetMatrixMVP(PV, M);
}

RenderLayer Material::GetRenderLayer()
{
	return forwardPass->GetRenderLayer();
}
