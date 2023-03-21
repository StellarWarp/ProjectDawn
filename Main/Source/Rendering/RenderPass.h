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

private:

	uint32_t ID;

	uint32_t location_MVP[2];
	uint32_t location_N;
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

	void Create(std::wstring_view name);

	void Load(const wchar_t* vertexPath, const wchar_t* fragmentPath);

	void Use();

	void RuntimeUpdateCheck();

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

	inline uint32_t GetLocation(const std::string& name) const {
		return glGetUniformLocation(ID, name.c_str());
	}
	void SetMatrixMVP(const glm::mat4& PV, const glm::mat4& M);

private:



	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	static void checkCompileErrors(unsigned int shader, std::wstring_view type);
};
