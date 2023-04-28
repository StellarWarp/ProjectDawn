#pragma once
#include "Material.h"

class MeshData {
public:
	struct Vertices {
		template<class T, uint32_t index>
		struct Attribute {
		private:

			template<class U>
			consteval static auto get_info_count();

			template<>
			consteval static auto get_info_count<glm::vec2>() { return 2; }

			template<>
			consteval static auto get_info_count<glm::vec3>() { return 3; }

			template<>
			consteval static auto get_info_count<glm::vec4>() { return 4; }

			template<class U>
			consteval static auto get_info_type() { return GL_FLOAT; }

			std::vector<T> vector;
		public:

			operator T* () {
				return vector.data();
			}

			uint32_t byte_size() {
				return sizeof(T) * static_cast<uint32_t>(vector.size());
			}

			void resize(uint32_t size) {
				vector.resize(size);
			}

			consteval uint32_t count() {
				return get_info_count<T>();
			}

			consteval uint32_t type() {
				return get_info_type<T>();
			}
		};

		Attribute<glm::vec3, 0> position;
		Attribute<glm::vec3, 1> normal;
		Attribute<glm::vec2, 2> texCoords;
		Attribute<glm::vec3, 3> tangent;
		Attribute<glm::vec3, 4> bitangent;

		size_t byte_size() {
			return position.byte_size() + normal.byte_size() + texCoords.byte_size() + tangent.byte_size() + bitangent.byte_size();
		}
	};

public:
	uint32_t VAO;
	uint32_t VBO;
	uint32_t EBO;

	Vertices vertices;
	std::vector<uint32_t> indices;

	template<class T, uint32_t index>
	void LoadAttribute(Vertices::Attribute<T, index> attribute, size_t& offset) {
		glBufferSubData(GL_ARRAY_BUFFER, offset, attribute.byte_size(), attribute);
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, attribute.count(), attribute.type(), GL_FALSE, 0/*tightly packed*/,
			reinterpret_cast<const void*>(offset));
		offset += attribute.byte_size();
	}

	void LoadToGPU() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.byte_size(), nullptr, GL_STATIC_DRAW);

		size_t offset = 0;
		LoadAttribute(vertices.position, offset);
		LoadAttribute(vertices.normal, offset);
		//LoadAttribute(vertices.texCoords, offset);

		//index
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t VertexCount() {
		return vertices.position.byte_size();
	}
};

class Renderer {

	inline static std::function<void(Renderer*)> RenderEnqueue;
	inline friend void EngineSetRenderQueue(std::function<void(Renderer*)> enqueue)
	{
		Renderer::RenderEnqueue = enqueue;
	}


public:

	Transform& transform;
	std::shared_ptr<Material> material;
	std::shared_ptr<MeshData> meshData;

	Renderer(Transform& transform)
		: transform(transform) {
	}

	void Update()
	{
		RenderEnqueue(this);
	}

	void ForwardRender();

	void ShadowRender();

	void GraphicDrawCall()
	{
		glBindVertexArray(meshData->VAO);
		glDrawElements(
			GL_TRIANGLES, static_cast<uint32_t>(meshData->indices.size()),
			GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

};
