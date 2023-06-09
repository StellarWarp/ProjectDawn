#include "RenderPipeline.h"
#include "Light.h"

RenderPass* debugPass;

unsigned int quadVAO = 0;
unsigned int quadVBO;
void QuadInit()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		//set quadVertices to left up corner
		for (int i = 0; i < 4; i++)
		{
			auto &x = quadVertices[i * 5 + 0];
			auto &y = quadVertices[i * 5 + 1];
			x = x/2 + 0.5f;
			y = y/2 + 0.5f;
		}

		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
}

void renderQuad()
{
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


RenderPipeline::RenderPipeline() {
	if (instance) throw std::exception("RenderPipeline is a singleton");
	instance = this;
	using namespace std::placeholders;
	EngineSetRenderQueue(
		std::bind(&RenderPipeline::RendererEnqueue, this, _1));
	backgroundQueue.reserve(1000);
	opaqueQueue.reserve(1000);
	transparentQueue.reserve(1000);
	overlayQueue.reserve(1000);

	debugPass = new RenderPass();
	debugPass->Create<IRenderPassProperty>(L"DebugQuad");
	QuadInit();
}


void RenderPipeline::Render()
{
	Light::mainLight->ClearShadowMap();

	Camera::GetMainCamera()->TagetBindCamera();

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	RenderBackground();
	RenderShadow();
	RenderOpaque();
	RenderTransparent();
	RenderOverlay();
	ClearUp();

}


void RenderPipeline::RenderShadow()
{
	glCullFace(GL_FRONT);
	for (auto renderer : opaqueQueue)
	{
		renderer->ShadowRender();
	}
	for (auto renderer : transparentQueue)
	{
		renderer->ShadowRender();
	}
	glCullFace(GL_BACK);


	debugPass->Active(renderQuad);
	
}

void RenderPipeline::RenderBackground()
{
	for (auto renderer : backgroundQueue)
	{
		renderer->ForwardRender();
	}
}

void RenderPipeline::RenderOpaque()
{
	for (auto renderer : opaqueQueue)
	{
		renderer->ForwardRender();
	}
}

void RenderPipeline::RenderTransparent()
{
	for (auto renderer : transparentQueue)
	{
		renderer->ForwardRender();
	}
}

void RenderPipeline::RenderOverlay()
{
	for (auto renderer : overlayQueue)
	{
		renderer->ForwardRender();
	}
}

void RenderPipeline::ClearUp()
{
	backgroundQueue.clear();
	opaqueQueue.clear();
	transparentQueue.clear();
	overlayQueue.clear();

}

void RenderPipeline::RendererEnqueue(Renderer* renderer)
{
	switch (renderer->material->GetRenderLayer())
	{
	case RenderLayer::Background:
		backgroundQueue.push_back(renderer);
		break;
	case RenderLayer::Opaque:
		opaqueQueue.push_back(renderer);
		break;
	case RenderLayer::Transparent:
		transparentQueue.push_back(renderer);
		break;
	case RenderLayer::Overlay:
		overlayQueue.push_back(renderer);
		break;
	default:
		break;
	}
}
