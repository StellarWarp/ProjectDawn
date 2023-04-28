#include "CustomPassProperty.h"
#include "Rendering/Camera.h"
#include "Rendering/Light.h"
#include "Rendering/Renderer.h"

void TransformProperty::SetData(Renderer* renderer)
{
	SetMatrixMVP(
		Camera::GetMainCamera()->WorldToScreenTransform(),
		renderer->transform.ObjectToWorldTransform()
	);
}

void LitPassProperty::SetData(Renderer* renderer)
{
	Super::SetData(renderer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Light::mainLight->depthMap);
	SetLightMatrix(Light::mainLight->WorldToShadowTransform());
	SetLightDirection(Light::mainLight->GetDirection());
}

void ShadowPassProperty::SetData(Renderer* renderer)
{
	auto mainLight = Light::mainLight;
	mainLight->TargetBindShadow();


	SetMatrixMVP(
		mainLight->WorldToShadowTransform(),
		renderer->transform.ObjectToWorldTransform()
	);
}

void ShadowPassProperty::ClearUp(Renderer* renderer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Camera::GetMainCamera()->TagetBindCamera();
}
