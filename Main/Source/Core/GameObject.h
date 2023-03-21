#pragma once
#include "Rendering/Renderer.h"

class GameObject {
protected:
	GameObject* parent{};
	std::vector<std::unique_ptr<GameObject>> children;
public:

	std::wstring name;
	Transform transform;


	void AddChild(std::unique_ptr<GameObject>&& child);

	std::unique_ptr<GameObject> RemoveChild(GameObject* child);

	void SetParent(GameObject* parent);

	const GameObject& GetChild(uint32_t index);
	template<class T>
	void IfCast(std::function<void(T&)> func)
	{
		T* ptr = dynamic_cast<T*>(this);
		if (ptr) {
			func(*ptr);
		}
	}

	//GameObject() {}
	//virtual ~GameObject() {}
protected:
	void friend EngineStart(GameObject* root);
	void friend EngineUpdate(GameObject* root,float dt);
	virtual void Start();
	virtual void Update(float dt);
};

class SceneObject :public GameObject {
public:
	Renderer renderer{ transform };
protected:
	void Update(float dt) override;

};