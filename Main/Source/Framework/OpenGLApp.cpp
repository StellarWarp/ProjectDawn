#include "OpenGLApp.h"
#include "Input.h"
#include "Rendering/Camera.h"

 bool OpenGLApp::Init() {
	//        if (!InitOpenGL()) return false;
	if (!InitImGui()) return false;

	return true;
}

 void OpenGLApp::Run() {
	m_Timer.Reset();
	while (!glfwWindowShouldClose(m_Window)) {
		// input
		// -----
		//processInput(m_Window);
		m_Timer.Tick();
		UpdateLogic(m_Timer.DeltaTime());
		RenderScene();

		if(Input::GetKeyDown(KeyCode::ESCAPE))
			glfwSetWindowShouldClose(m_Window, true);
	}
}

 void OpenGLApp::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	if (Camera::GetMainCamera())
	{
		Camera::GetMainCamera()->TargetInfoReset(width, height);
	}
}

 bool OpenGLApp::InitOpenGL() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_Window = glfwCreateWindow(m_ScreenWidth, m_ScreenHeight, (const char*)m_Title.c_str(), NULL, NULL);
	if (m_Window == nullptr) {
		//std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_Window);
	using namespace std::placeholders;

	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to Load GLAD" << std::endl;
		return false;
	}
	Input::Init(m_Window);

	return true;
}

 bool OpenGLApp::InitImGui() {
	return true;
}
