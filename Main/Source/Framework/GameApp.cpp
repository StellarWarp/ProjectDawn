#include "GameApp.h"
#include "Utility/Utility.h"
#include "Rendering/Light.h"
bool GameApp::Init() {
	if (!OpenGLApp::Init()) return false;
	if (!InitObject()) return false;
	if (!LoadResources()) return false;
	EngineStart(&m_WorldRoot);
	return true;
}

bool GameApp::InitObject()
{
	m_Camera = new FreeCamera();
	m_Camera->TargetInfoReset(m_ScreenWidth, m_ScreenHeight);
	m_Camera->SetParent(&m_WorldRoot);
	m_Camera->transform.position = { -1, 0.5, 1 };
	m_Camera->transform.LookAt({ 0,0,0 });


	Light* light = new Light();

	return true;
}


bool GameApp::LoadResources() {
	//passes
	auto pass = std::make_shared<RenderPass>();
	pass->Create(L"Lit");
	auto shadowPass = std::make_shared<RenderPass>();
	shadowPass->Create(L"Caster");
	shadowPass->NormalEnable = false;


	auto mat = std::make_shared <Material>();
	mat->SetPass<LightMode::FORWARD>(pass);
	mat->SetPass<LightMode::SHADOW>(shadowPass);

	auto modelPathDir = Utility::ProjectPath();
	modelPathDir.append("Model");

	auto LoadAndSetParent = [&](std::wstring_view name) {
		GameObject* obj = ModelLoader::LoadModelFromFile(
			Utility::RecursiveFind(modelPathDir, name).c_str()
		);
		obj->IfCast<SceneObject>([&](SceneObject& obj) {
			obj.renderer.material = mat;
			});
		obj->SetParent(&m_WorldRoot);
		return obj;
	};

	LoadAndSetParent(L"bunny")->transform.position = { 0,0,0 };
	auto cube = LoadAndSetParent(L"cube");
	auto cube1 = LoadAndSetParent(L"cube");
	cube->transform.position = { 0,-0.5,0 };
	cube->transform.scale = { 0.5,0.5,0.5 };
	cube->transform.rotation = glm::qua(glm::vec3(glm::pi<float>(), 0, 0));
	cube1->transform.position = { 0,-0.5,3 };
	cube1->transform.scale = { 0.5,0.5,0.5 };
	return true;
}

void GameApp::UpdateLogic(float dt) {
	EngineUpdate(&m_WorldRoot, dt);

	if (Input::GetKeyDown(KeyCode::F))
	{
		auto& pos = Camera::GetMainCamera()->transform.position;
		auto rot = glm::eulerAngles(Camera::GetMainCamera()->transform.rotation);
		std::cout << std::format("x {:.2f} y {:.2f} z {:.2f}\n", pos.x, pos.y, pos.z);
		std::cout << std::format("rx {:.2f} ry {:.2f} rz {:.2f}\n", rot.x, rot.y, rot.z);
	}
}

void GameApp::RenderScene() {
	//poll IO events (keys pressed/released, mouse moved etc.)
	glfwPollEvents();

	m_RenderPipeline.Render();

	glfwSwapBuffers(m_Window);
}
