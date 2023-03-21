#pragma once
#include "Libraries/OpenGL.h"
#include "Libraries/Libs.h"
#include "Utility/CpuTimer.h"


class OpenGLApp {
protected:
	std::string m_Title;
	int m_ScreenWidth;
	int m_ScreenHeight;
	GLFWwindow* m_Window{};

	CpuTimer m_Timer;
public:
	OpenGLApp(std::string windowName, int initWidth, int initHeight)
		: m_Title(std::move(windowName)),
		m_ScreenWidth(initWidth),
		m_ScreenHeight(initHeight) {
		InitOpenGL();
	}

	virtual ~OpenGLApp() {
		glfwTerminate();
	}

	virtual bool Init();

	void Run();

	virtual void UpdateLogic(float dt) = 0;

	virtual void RenderScene() = 0;

private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	bool InitOpenGL();

	bool InitImGui();
};