#pragma once

#include "GameObject.h"
class Component {
public:
	//GameObject& root;
	//Component(GameObject& root) : root(root) {}
	
	virtual void Start();
	virtual void Update();

};