#include "Material.h"
#include "Light.h"




//void IShaderData::SetData(std::shared_ptr<RenderPass>& pass) {
//	auto mainLight = Light::mainLight;
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, mainLight->depthMap);
//
//	pass->SetMat4(location_Light, mainLight->WorldToShadowTransform());
//}

void Material::ActivePass(std::shared_ptr<RenderPass>& pass)
{
	pass->RuntimeUpdateCheck();
	pass->Use();
	activePass = pass.get();
}

RenderLayer Material::GetRenderLayer()
{
	return forwardPass->GetRenderLayer();
}
