#pragma once

#include "../Core/GameObject.h"
#include "../Framework/Input.h"


class RigidBody {
	glm::vec3 velocity{};
	glm::vec3 angularVelocity{};
public:
	Transform& transform;
	float mass = 1;
	float friction = 10;
	float angularFriction = 10;

	enum ForceMode
	{
		FORCE,
		IMPULSE,
		ACCELERATION
	};


	RigidBody(Transform& transform) : transform(transform) {}

	void Update(float dt)
	{
		//move
		transform.position += velocity * dt;
		AddForce(velocity * -friction, dt);
		//angle
		transform.rotation;
		AddTorque(angularVelocity * -angularFriction, dt);
	}
	template<ForceMode mode = FORCE>
	void AddForce(glm::vec3 force, float dt);

	template<> void AddForce<FORCE>(glm::vec3 force, float dt)
	{
		velocity += force / mass * dt;
	}
	template<> void AddForce<IMPULSE>(glm::vec3 force, float dt)
	{
		velocity += force / mass;
	}
	template<> void AddForce<ACCELERATION>(glm::vec3 force, float dt)
	{
		velocity += force * dt;
	}

	void AddTorque(glm::vec3 torque, float dt)
	{
		angularVelocity += torque / mass * dt;
	}

	glm::vec3& Velocity() { return velocity; }
	glm::vec3& Position() { return transform.position; }
};


class MovementController {
	RigidBody& rigidbody;
	float rotateSpeed = 1;
	float moveForce = 10;
public:
	MovementController(RigidBody& rb) :rigidbody(rb) {}
	void Update(float dt)
	{
		glm::vec3 moveDir = glm::vec3(0.0f);
		if (Input::GetKey(KeyCode::W))
		{
			moveDir += glm::vec3(0.0f, 0.0f, -1.0f);
		}
		if (Input::GetKey(KeyCode::S))
		{
			moveDir += glm::vec3(0.0f, 0.0f, 1.0f);
		}
		if (Input::GetKey(KeyCode::A))
		{
			moveDir += glm::vec3(-1.0f, 0.0f, 0.0f);
		}
		if (Input::GetKey(KeyCode::D))
		{
			moveDir += glm::vec3(1.0f, 0.0f, 0.0f);
		}
		if (Input::GetKey(KeyCode::E))
		{
			moveDir += glm::vec3(0.0f, 1.0f, 0.0f);
		}
		if (Input::GetKey(KeyCode::Q))
		{
			moveDir += glm::vec3(0.0f, -1.0f, 0.0f);
		}
		if (glm::length(moveDir) > 0.0f)
		{
			moveDir = glm::normalize(moveDir);
			moveDir = rigidbody.transform.rotation * glm::vec4(moveDir, 1);
			rigidbody.AddForce(moveDir * 10.0f, dt);
		}
		
		auto delta = Input::GetMouseDelta();
		if (Input::GetKey(KeyCode::SPACE))
		{
			Input::SetMouseHide();
			delta *= rotateSpeed * dt;
			auto& rotation = rigidbody.transform.rotation;
			using namespace glm;

			rotation = qua{ vec3{ 0.0f, -delta.x,0.0f} } * rotation * qua{ vec3{ -delta.y, 0.0f,0.0f} };

		}
		else
		{
			Input::SetMouseDefault();
		}

	}


};