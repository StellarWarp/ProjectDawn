#include "GameApp.h"
#include "Utility/Utility.h"
#include "Rendering/ShaderProperty/CustomPassProperty.h"
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

	m_Light = new LightObject();
	m_Light->SetParent(&m_WorldRoot);
	m_Light->transform.position = { 0, 0, 0 };
	m_Light->light.SetMainLight();
	m_Light->light.SetDirection({ 1,1,1 });

	return true;
}


bool GameApp::LoadResources() {
	//passes
	auto pass = std::make_shared<RenderPass>();
	pass->Create<LitPassProperty>(L"Lit");
	auto shadowPass = std::make_shared<RenderPass>();
	shadowPass->Create<ShadowPassProperty>(L"Caster");
	shadowPass->NormalEnable = false;


	auto mat = std::make_shared <Material>();
	mat->SetPass<LightMode::FORWARD>(pass);
	mat->SetPass<LightMode::SHADOW>(shadowPass);



	//object
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

	auto bunny = LoadAndSetParent(L"bunny");
	bunny->transform.position = { 0,0,0 };
	bunny->transform.scale = { 2,2,2 };
	auto cube = LoadAndSetParent(L"cube");
	auto cube1 = LoadAndSetParent(L"cube");
	cube->transform.position = { 0,-0.5,0 };
	cube->transform.scale = { 2,0.5,2 };
	cube->transform.rotation = glm::qua(glm::vec3(glm::pi<float>(), 0, 0));
	cube1->transform.position = { 0,-0.5,3 };
	cube1->transform.scale = { 0.5,0.5,0.5 };

	Light::mainLight->SetDirection(glm::normalize(glm::vec3{ 1, 1, 1 }));
	return true;
}

void GameApp::UpdateLogic(float dt) {
	EngineUpdate(&m_WorldRoot, dt);

	//if (Input::GetKeyDown(KeyCode::F))
	//{
	//	auto& pos = Camera::GetMainCamera()->transform.position;
	//	auto rot = glm::eulerAngles(Camera::GetMainCamera()->transform.rotation);
	//	std::cout << std::format("x {:.2f} y {:.2f} z {:.2f}\n", pos.x, pos.y, pos.z);
	//	std::cout << std::format("rx {:.2f} ry {:.2f} rz {:.2f}\n", rot.x, rot.y, rot.z);
	//}
	//make light follow camera
	auto& lightPos = Light::mainLight->transform.position;
	lightPos = m_Camera->transform.position + Light::mainLight->GetDirection()*1.0f;

	//rotate light
	if (Input::GetKey(KeyCode::X))
	{
		auto& light = Light::mainLight;
		auto delta = Input::GetMouseDelta();
		Input::SetMouseHide();

		delta *= 10 * dt;
		using namespace glm;
		auto dir = light->GetDirection();
		//rotate dir around y axis with delta.x
		dir = rotate(mat4(1), delta.x, vec3(0, 1, 0)) * vec4(dir, 0);
		//rotate dir up and down with delta.y
		auto right = cross(dir, vec3(0, 1, 0));
		dir = rotate(mat4(1), delta.y, right) * vec4(dir, 0);
		light->SetDirection(dir);
	}
}

void GameApp::RenderScene() {
	//poll IO events (keys pressed/released, mouse moved etc.)
	glfwPollEvents();

	m_RenderPipeline.Render();

	glfwSwapBuffers(m_Window);
}
