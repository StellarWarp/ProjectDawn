#include "Camera.h"
#include "Light.h"
#include "Renderer.h"


void Renderer::ForwardRender() {
	material->ActivePass<LightMode::FORWARD>(this, [this]() {
		GraphicDrawCall();
		});
}

void Renderer::ShadowRender()
{
	material->ActivePass<LightMode::SHADOW>(this, [this]() {
		GraphicDrawCall();
		});
}


