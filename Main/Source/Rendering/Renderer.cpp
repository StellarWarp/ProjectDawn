#include "Camera.h"
#include "Light.h"
#include "Renderer.h"



void Renderer::ForwardRender() {
	material->ActivePass<LightMode::FORWARD>();
	auto mainCam = Camera::GetMainCamera();
	//mainCam->TagetBindCamera();

	material->SetMatrixMVP(
		mainCam->WorldToScreenTransform(),
		transform.ObjectToWorldTransform()
	);

	GraphicDrawCall();
}

void Renderer::ShadowRender()
{
	material->ActivePass<LightMode::SHADOW>();
	auto mainLight = Light::mainLight;
	mainLight->TargetBindShadow();

	material->SetMatrixMVP(
		mainLight->WorldToShadowTransform(),
		transform.ObjectToWorldTransform()
	);

	GraphicDrawCall();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Camera::GetMainCamera()->TagetBindCamera();
}


