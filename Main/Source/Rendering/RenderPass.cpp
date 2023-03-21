#include "RenderPass.h"
#include "Utility/Utility.h"
#include <sys/stat.h>


bool RenderPass::Shader::UpdateCheck() {
#ifdef RUNTIME_SHADER_RELOAD
	struct _stat64 result;
	_wstat64(shaderPath.c_str(), &result);
	if (result.st_mtime != lastCompiledModification) {
		Compile();
		return true;
	}
#endif
	return false;
}

void RenderPass::Shader::Compile() {
	std::string source;
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	for (size_t i = 0; i < 10; i++)
	{
		try {
			// open files
			file.open(shaderPath);
			std::stringstream shaderStream;
			shaderStream << file.rdbuf();
			file.close();
			source = shaderStream.str();
			break;
		}
		catch (std::ifstream::failure& e) {
			std::cerr << std::format("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {} Tried: {}\n", e.what(), i + 1);
		}
	}

	const char* code = source.c_str();
	// 2. Compile shaders
	glShaderSource(shaderID, 1, &code, nullptr);
	glCompileShader(shaderID);
	checkCompileErrors(shaderID, shaderPath.c_str());
	//recompile check
#ifdef RUNTIME_SHADER_RELOAD
	struct _stat64 result;
	_wstat64(shaderPath.c_str(), &result);
	lastCompiledModification = result.st_mtime;
#endif
}

RenderLayer RenderPass::GetRenderLayer() const {
	if (RenderQueue < 2000) {
		return RenderLayer::Background;
	}
	if (RenderQueue < 3000) {
		return RenderLayer::Opaque;
	}
	if (RenderQueue < 4000) {
		return RenderLayer::Transparent;
	}
	return RenderLayer::Overlay;
}

void RenderPass::Create(std::wstring_view shaderName)
{
	namespace fs = std::filesystem;
	auto srcPath = Utility::ProjectPath();
	srcPath.append("Shader\\");

	fs::path vertPath;
	fs::path fragPath;

	for (const auto& entry : fs::recursive_directory_iterator(srcPath)) {
		if (!vertPath.empty() && !fragPath.empty()) break;
		if (entry.is_regular_file() && entry.path().filename().stem() == shaderName) {
			if (entry.path().extension() == ".vert") {
				vertPath = entry.path();
			}
			else if (entry.path().extension() == ".frag") {
				fragPath = entry.path();
			}
		}
	}
	Load(vertPath.c_str(), fragPath.c_str());
}

void RenderPass::Load(const wchar_t* vertexPath, const wchar_t* fragmentPath) {
	VertexShader.Load(vertexPath);
	FragmentShader.Load(fragmentPath);
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, VertexShader);
	glAttachShader(ID, FragmentShader);
	glLinkProgram(ID);
	checkCompileErrors(ID, L"PROGRAM");

	location_MVP[0] = GetLocation("MATRIX_M");
	location_MVP[1] = GetLocation("MATRIX_PV");
	location_N = GetLocation("MATRIX_N");
}

void RenderPass::Use() {
	glUseProgram(ID);
	//RS.Stepup();
}

void RenderPass::RuntimeUpdateCheck() {
	if (VertexShader.UpdateCheck()) {
		glAttachShader(ID, VertexShader);
		glLinkProgram(ID);
		checkCompileErrors(ID, L"PROGRAM");
	}
	if (FragmentShader.UpdateCheck()) {
		glAttachShader(ID, FragmentShader);
		glLinkProgram(ID);
		checkCompileErrors(ID, L"PROGRAM");
	}
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------

void RenderPass::SetMatrixMVP(const glm::mat4& PV, const glm::mat4& M)
{
	SetMat4(location_MVP[0], M);
	SetMat4(location_MVP[1], PV);
	if (NormalEnable)
		SetMat3(location_N, glm::transpose(glm::inverse(glm::mat3(M))));
}

void RenderPass::checkCompileErrors(unsigned int shader, std::wstring_view type) {
	int success;
	char infoLog[1024];
	if (type != L"PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::wcout << "ERROR::SHADER_COMPILATION_ERROR of shaderType: " << type << "\n" << infoLog
				<< "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::wcout << "ERROR::PROGRAM_LINKING_ERROR of shaderType: " << type << "\n" << infoLog
				<< "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
