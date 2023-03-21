#include "GameObject.h"


void GameObject::AddChild(std::unique_ptr<GameObject>&& child) {
	child->parent = this;
	children.push_back(std::move(child));
}

std::unique_ptr<GameObject> GameObject::RemoveChild(GameObject* child) {
	auto it = std::find_if(children.begin(), children.end(), [child](const auto& c) {
		return c.get() == child;
		});
	if (it != children.end()) {
		auto result = std::move(*it);
		children.erase(it);
		return std::move(result);
	}
	return std::unique_ptr<GameObject>(nullptr);
}

void GameObject::SetParent(GameObject* parent) {
	std::unique_ptr<GameObject> ptr;
	if (!this->parent) {
		ptr = std::unique_ptr<GameObject>(this);
	}
	else {
		ptr = this->parent->RemoveChild(this);
	}
	if (ptr) {
		parent->AddChild(std::move(ptr));
	}
}

const GameObject& GameObject::GetChild(uint32_t index)
{
	return *children[index];
}

void GameObject::Start() {
	for (const auto& child : children)
	{
		child->Start();
	}
}

void GameObject::Update(float dt)
{
	for (const auto& child : children)
	{
		child->Update(dt);
	}
}

void SceneObject::Update(float dt)
{
	GameObject::Update(dt);
	renderer.Update();
}


void EngineStart(GameObject* root)
{
	root->Start();
}

void EngineUpdate(GameObject* root,float dt)
{
	root->Update(dt);
}
