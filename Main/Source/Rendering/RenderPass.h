#pragma once

#include "Libraries/OpenGL.h"
#include "Libraries/Libs.h"
class Renderer;


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
			requires std::is_same_v<T, bool> ||
			std::is_same_v<T, int> ||
			std::is_same_v<T, float> ||
			std::is_same_v<T, glm::vec2> ||
			std::is_same_v<T, glm::vec3> ||
			std::is_same_v<T, glm::vec4> ||
			std::is_same_v<T, glm::mat2> ||
			std::is_same_v<T, glm::mat3> ||
			std::is_same_v<T, glm::mat4>
		class ShaderProperty {

		public:
			uint32_t location;
			ShaderProperty(const std::string& name, const RenderPass* pass) {
				location = glGetUniformLocation(pass->progma_id, name.c_str());;
			}

			void Set(const T& value) {
				//SetUniform(location, data);
				if constexpr (std::is_same_v<T, bool>) {
					glUniform1i(location, (int)value);
				}
				else if constexpr (std::is_same_v<T, int>) {
					glUniform1i(location, value);
				}
				else if constexpr (std::is_same_v<T, float>) {
					glUniform1f(location, value);
				}
				else if constexpr (std::is_same_v<T, glm::vec2>) {
					glUniform2fv(location, 1, &value[0]);
				}
				else if constexpr (std::is_same_v<T, glm::vec3>) {
					glUniform3fv(location, 1, &value[0]);
				}
				else if constexpr (std::is_same_v<T, glm::vec4>) {
					glUniform4fv(location, 1, &value[0]);
				}
				else if constexpr (std::is_same_v<T, glm::mat2>) {
					glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
				}
				else if constexpr (std::is_same_v<T, glm::mat3>) {
					glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
				}
				else if constexpr (std::is_same_v<T, glm::mat4>) {
					glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
				}
				else {
					throw std::exception("SetUniform: Type not supported");
				}
			}
		};

		template<class T>
		class ShaderConstantBuffer {
		public:
			//to do: finish api for constant buffer
		};
		
	public:

		IRenderPassProperty(RenderPass* pass) :pass(pass) {}
		virtual void SetData(Renderer* renderer) {};
		virtual void ClearUp(Renderer* renderer) {};
	protected:
		RenderPass* pass{};

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

	void RuntimeUpdateCheck();
	void Active(Renderer* renderer,auto OnDrawCall)
	{
		RuntimeUpdateCheck();
		glUseProgram(progma_id);
		properties->SetData(renderer);
		OnDrawCall();
		properties->ClearUp(renderer);
	}
	void Active(auto OnDrawCall)
	{
		RuntimeUpdateCheck();
		glUseProgram(progma_id);
		OnDrawCall();
	}
private:


private:



	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	static void checkCompileErrors(unsigned int shader, std::wstring_view type);
};

using IRenderPassProperty = RenderPass::IRenderPassProperty;