#pragma once

#include "Libraries/OpenGL.h"
#include "Libraries/Libs.h"


#if defined(_DEBUG) || defined(DEBUG)
#define RUNTIME_SHADER_RELOAD
#endif

//#ifdef RUNTIME_SHADER_RELOAD
//#define SHADER_RELOAD_CODE(code) code
//#else
//#define SHADER_RELOAD_CODE(code)
//#endif
enum class RenderLayer
{
	Background,
	Opaque,
	Transparent,
	Overlay
};

class RenderPass {

	enum class ShaderType : uint32_t {
		Null = 0,
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

	class Shader {
		uint32_t shaderID;
		std::wstring shaderPath{};
#ifdef RUNTIME_SHADER_RELOAD
		__time64_t lastCompiledModification{};
#endif
	public:
		Shader(const ShaderType& shaderType) {
			shaderPath = L"";
			shaderID = glCreateShader(static_cast<GLenum>(shaderType));
		}

		~Shader() {
			glDeleteShader(shaderID);
		}

		void Load(const wchar_t* path) {
			this->shaderPath = path;
			Compile();
		}

		bool UpdateCheck();


		inline operator uint32_t() const {
			return shaderID;
		}

	private:
		void Compile();

	};
public:
	class IRenderPassProperty {
	public:

		template<class T>
		class ShaderProperty {
		public:
			uint32_t location;
			ShaderProperty(const std::string& name, const RenderPass* pass) {
				location = pass->GetLocation(name);
			}

			void Set(const T& data) {
				RenderPass::SetUniform(location, data);
			}
		};
	public:

		RenderPass* pass{};

		IRenderPassProperty(RenderPass* pass) :pass(pass) {}

	protected:
		virtual void _() {}

	};

private:

	uint32_t progma_id;

	std::unique_ptr<IRenderPassProperty> properties;
public:
	Shader VertexShader{ ShaderType::Vertex };
	Shader FragmentShader{ ShaderType::Fragment };

	uint32_t RenderQueue = 2000;
	bool NormalEnable = true;
	//光栅化状态
	struct RasterizerState {
		bool Cull = true;
		bool Blend = false;
		bool DepthWrite = true;
		GLenum CullMode = GL_BACK;
		GLenum BlendSrc = GL_SRC_ALPHA;
		GLenum BlendDst = GL_ONE_MINUS_SRC_ALPHA;
		GLenum DepthFunc = GL_LEQUAL;


		void Stepup()
		{
			auto SwitchMode = [](bool on, uint32_t mode) {
				on ? glEnable(mode) : glDisable(mode);
			};
			SwitchMode(Cull, GL_CULL_FACE);
			SwitchMode(Blend, GL_CULL_FACE);
			SwitchMode(DepthWrite, GL_DEPTH_CLAMP);

			glCullFace(CullMode);
			glBlendFunc(BlendSrc, BlendDst);
			glDepthFunc(DepthFunc);

		}

	}RS;

	RenderLayer GetRenderLayer() const;

	template<class T> requires std::derived_from<T, IRenderPassProperty>
	void Create(std::wstring_view name)
	{
		CreateShaders(name);
		properties = std::make_unique<T>(this);
	}

	void CreateShaders(std::wstring_view shaderName);

	void Load(const wchar_t* vertexPath, const wchar_t* fragmentPath);

	void Use();

	void RuntimeUpdateCheck();
	template<class T> requires std::derived_from<T, IRenderPassProperty>
	T& Properties()
	{
		//bool is_null = properties->pass == nullptr;
		//std::cout << std::format("pass ptr{}\n", is_null);
		return *dynamic_cast<T*>(properties.get());
	}
private:
	// utility uniform functions
	static inline void SetBool(uint32_t location, bool value) {
		glUniform1i(location, (int)value);
	}
	static inline void SetInt(uint32_t location, int value) {
		glUniform1i(location, value);
	}
	static inline void SetFloat(uint32_t location, float value) {
		glUniform1f(location, value);
	}
	static inline void SetVec2(uint32_t location, const glm::vec2& value) {
		glUniform2fv(location, 1, &value[0]);
	}
	static inline void SetVec2(uint32_t location, float x, float y) {
		glUniform2f(location, x, y);
	}
	static inline void SetVec3(uint32_t location, const glm::vec3& value) {
		glUniform3fv(location, 1, &value[0]);
	}
	static inline void SetVec3(uint32_t location, float x, float y, float z) {
		glUniform3f(location, x, y, z);
	}
	static inline void SetVec4(uint32_t location, const glm::vec4& value) {
		glUniform4fv(location, 1, &value[0]);
	}
	static inline void SetVec4(uint32_t location, float x, float y, float z, float w) {
		glUniform4f(location, x, y, z, w);
	}
	static inline void SetMat2(uint32_t location, const glm::mat2& mat) {
		glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
	}
	static inline void SetMat3(uint32_t location, const glm::mat3& mat) {
		glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
	}
	static inline void SetMat4(uint32_t location, const glm::mat4& mat) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
	}
public:
	template<class T>
	static void SetUniform(uint32_t location, const T& value)
		requires std::is_same_v<T, bool> ||
		std::is_same_v<T, int> ||
		std::is_same_v<T, float> ||
		std::is_same_v<T, glm::vec2> ||
		std::is_same_v<T, glm::vec3> ||
		std::is_same_v<T, glm::vec4> ||
		std::is_same_v<T, glm::mat2> ||
		std::is_same_v<T, glm::mat3> ||
		std::is_same_v<T, glm::mat4>
	{
		if constexpr (std::is_same_v<T, bool>) {
			SetBool(location, value);
		}
		else if constexpr (std::is_same_v<T, int>) {
			SetInt(location, value);
		}
		else if constexpr (std::is_same_v<T, float>) {
			SetFloat(location, value);
		}
		else if constexpr (std::is_same_v<T, glm::vec2>) {
			SetVec2(location, value);
		}
		else if constexpr (std::is_same_v<T, glm::vec3>) {
			SetVec3(location, value);
		}
		else if constexpr (std::is_same_v<T, glm::vec4>) {
			SetVec4(location, value);
		}
		else if constexpr (std::is_same_v<T, glm::mat2>) {
			SetMat2(location, value);
		}
		else if constexpr (std::is_same_v<T, glm::mat3>) {
			SetMat3(location, value);
		}
		else if constexpr (std::is_same_v<T, glm::mat4>) {
			SetMat4(location, value);
		}
		else {
			throw std::exception("SetUniform: Type not supported");
		}
	}

	inline uint32_t GetLocation(const std::string& name) const {
		return glGetUniformLocation(progma_id, name.c_str());
	}

private:



	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	static void checkCompileErrors(unsigned int shader, std::wstring_view type);
};

using IRenderPassProperty = RenderPass::IRenderPassProperty;