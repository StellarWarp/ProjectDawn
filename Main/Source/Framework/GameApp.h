#pragma once
#include "Rendering/RenderPipeline.h"
#include "Rendering/ModelLoader.h"
#include "OpenGLApp.h"
#include "CustomObject/FreeCamera.h"

class GameApp : public OpenGLApp {

	GameObject m_WorldRoot;
	RenderPipeline m_RenderPipeline;
	FreeCamera* m_Camera{};

public:
	GameApp(const std::string& windowName, int initWidth, int initHeight)
		: OpenGLApp(windowName, initWidth, initHeight) {
	}

	~GameApp() {
	}

	bool Init() override;

	bool InitObject();

	bool LoadResources();


	void UpdateLogic(float dt) override;

	void RenderScene() override;
};