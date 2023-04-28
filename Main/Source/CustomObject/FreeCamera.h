#pragma once

#include "Component/MovementController.h"

class FreeCamera : public Camera {

public:
	RigidBody rigidBody{ transform };
	MovementController movementController{ rigidBody };

	void Update(float dt) override {
		Camera::Update(dt);
		movementController.Update(dt);	
		rigidBody.Update(dt);

	}

};