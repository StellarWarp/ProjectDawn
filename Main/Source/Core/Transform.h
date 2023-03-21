#pragma once
#include "Libraries/libs.h"

class Transform {
public:

	glm::vec3 position{ 0,0,0 };
	glm::qua<float> rotation{ glm::vec3(0.0f) };
	glm::vec3 scale{ 1,1,1 };

	glm::vec3 forward()
	{
		return rotation * glm::vec3(0, 0, 1);
	}

	glm::vec3 right()
	{
		return rotation * glm::vec3(1, 0, 0);
	}

	glm::vec3 up()
	{
		return rotation * glm::vec3(0, 1, 0);
	}

	glm::mat4 ObjectToWorldTransform() const
	{
		auto S = glm::scale(glm::mat4(1.0f), scale);
		auto R = glm::mat4_cast(rotation);
		auto T = glm::translate(glm::mat4(1.0f), position);
		return T * R * S;
	}

	glm::mat4 WorldToObjectTransform() const
	{
		auto T = glm::translate(glm::mat4(1.0f), -position);
		auto R = glm::mat4_cast(glm::conjugate(rotation));
		auto S = glm::scale(glm::mat4(1.0f), 1.0f / scale);
		return S * R * T;
	}

	void Translate(const glm::vec3& translation)
	{
		position += translation;
	}

	void Rotate(const glm::vec3& axis, float angle)
	{
		rotation = glm::angleAxis(angle, axis) * rotation;

	}

	void LookAt(const glm::vec3& target)
	{
		auto dir = glm::normalize(target - position);
		rotation = glm::quatLookAt(dir, glm::vec3(0, 1, 0));
	}

};