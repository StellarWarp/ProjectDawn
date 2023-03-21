#pragma once
#include "Camera.h"

class RenderPipeline {
protected:
	std::vector<Renderer*> backgroundQueue;//1000
	std::vector<Renderer*> opaqueQueue;//2000
	std::vector<Renderer*> transparentQueue;//3000
	std::vector<Renderer*> overlayQueue;//4000

	inline static RenderPipeline* instance;
	
public:
	inline static RenderPipeline& Instance() {
		return *instance;
	}
	
public:
	RenderPipeline();

	void Render();


private:
    void RenderShadow();

	void RenderBackground();

	void RenderOpaque();

	void RenderTransparent();

	void RenderOverlay();

	void ClearUp();

	void RendererEnqueue(Renderer* renderer);
};