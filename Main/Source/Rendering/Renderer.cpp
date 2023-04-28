#include "Camera.h"
#include "Light.h"
#include "Renderer.h"
#include "Rendering/ShaderProperty/LitShader.h"



void Renderer::ForwardRender() {
	auto pass = material->ActivePass<LightMode::FORWARD>();
	auto& properties = pass->Properties<LitPassData>();
	auto mainCam = Camera::GetMainCamera();
	//mainCam->TagetBindCamera();

	properties.SetMatrixMVP(
		mainCam->WorldToScreenTransform(),
		transform.ObjectToWorldTransform()
	);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Light::mainLight->depthMap);

	properties.SetLightMatrix(Light::mainLight->WorldToShadowTransform());
	properties.SetLightDirection(Light::mainLight->GetDirection());

	GraphicDrawCall();
}

void Renderer::ShadowRender()
{
	auto pass = material->ActivePass<LightMode::SHADOW>();
	auto properties = pass->Properties<ShadowPassData>();

	auto mainLight = Light::mainLight;
	mainLight->TargetBindShadow();


	properties.SetMatrixMVP(
		mainLight->WorldToShadowTransform(),
		transform.ObjectToWorldTransform()
	);
	GraphicDrawCall();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Camera::GetMainCamera()->TagetBindCamera();
}


