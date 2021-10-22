#include <rendering/shader.h>

#include <helpers/RootDir.h>
#include <glm/gtc/type_ptr.hpp>

#include <vector>


std::string ShaderBase::readShaderFiles(std::vector<std::string> paths) const {

	std::string shaderCode;
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	for (auto const& path : paths) {

		try {

			file.open(ROOT_DIR "resources/shaders/" + path);
			// won't need these stream objects after file was read
			std::stringstream stream;
			// read
			stream << file.rdbuf();
			// close
			file.close();
			// to string
			shaderCode += stream.str();

		} catch (std::ifstream::failure& error) {
			std::cout << "[Error][Shader] File " << path << " not read" << std::endl;
			return "";
		}
	}

	return shaderCode;
}

bool ShaderBase::checkCompileErrors(int shader, std::string name) {

	int compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		int maxLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLen);
		std::vector<char> log;
		log.reserve(maxLen);
		glGetShaderInfoLog(shader, maxLen, NULL, log.data());

		std::cout << "[Error][Shader] Compilation error at: " << name << "\n" << log.data() << std::endl;

		return false;
	}

	return true;
}

bool ShaderBase::checkLinkErrors(int shader) {
	int linked = 0;
	glGetProgramiv(shader, GL_LINK_STATUS, &linked);

	if (!linked) {
		int maxLen = 0;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLen);
		std::vector<char> log;
		log.reserve(maxLen);
		glGetShaderInfoLog(shader, maxLen, NULL, log.data());

		std::cout << "[Error][Shader] Linking error: \n" << log.data() << std::endl;

		return false;
	}

	return true;
}

void ShaderBase::setUniform(const std::string& name, bool value) {
	glUseProgram(ID_);
	glUniform1i(glGetUniformLocation(ID_, name.c_str()), (int)value);
}

void ShaderBase::setUniform(const std::string& name, int value) {
	glUseProgram(ID_);
	glUniform1i(glGetUniformLocation(ID_, name.c_str()), value);
}

void ShaderBase::setUniform(const std::string& name, float value) {
	glUseProgram(ID_);
	glUniform1f(glGetUniformLocation(ID_, name.c_str()), value);
}

void ShaderBase::setUniform(const std::string& name, glm::vec2 value) {
	glUseProgram(ID_);
	glUniform2fv(glGetUniformLocation(ID_, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderBase::setUniform(const std::string& name, glm::vec3 value) {
	glUseProgram(ID_);
	glUniform3fv(glGetUniformLocation(ID_, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderBase::setUniform(const std::string& name, glm::vec4 value) {
	glUseProgram(ID_);
	glUniform4fv(glGetUniformLocation(ID_, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderBase::setUniform(const std::string& name, glm::mat4 value) {
	glUseProgram(ID_);
	glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderBase::reload() {
	glDeleteProgram(ID_);
	ID_ = 0;
	compile();
}

std::string ShaderBase::createPreprocessorFlags() const {
	std::string flags;
	for (auto const& flag : preprocessorFlags_) {
		if (flag.second) {
			flags += "#define " + flag.first + "\n";
		}
	}

	return flags;
}

void ShaderBase::setBlockBinding(const std::string& name, unsigned int binding) {
	glUniformBlockBinding(ID_, glGetUniformBlockIndex(ID_, name.c_str()), binding);
}

void ShaderBase::setFlag(std::string flag, bool value) {
	preprocessorFlags_.at(flag) = value;
}

void ShaderBase::use() {
	glUseProgram(ID_);
}

Shader::Shader(const std::string& vsPath, const std::string& fsPath, std::vector<std::string> flags)
	: vsPaths_({ vsPath }), fsPaths_({ fsPath }) {
	for (auto const& flag : flags) {
		preprocessorFlags_.insert({ flag, false });
	}
	versionDirective_ = "#version 430\n";
	compile();
}

Shader::Shader(const std::vector<std::string>& vsPaths, const std::vector<std::string>& fsPaths, std::vector<std::string> flags)
	: vsPaths_(vsPaths), fsPaths_(fsPaths) {
		for (auto const& flag : flags) {
			preprocessorFlags_.insert({ flag, false });
		}
		versionDirective_ = "#version 430\n";
		compile();
}

void Shader::compile() {
	// vs = vertex shader, fs = fragment shader
	std::string ppflags = createPreprocessorFlags();
	std::string vsCode = versionDirective_ + ppflags + readShaderFiles(vsPaths_);
	std::string fsCode = versionDirective_ + ppflags + readShaderFiles(fsPaths_);

	unsigned int vsID, fsID, ID;
	// add preprocessor definitions to shader code
	const char *vsCodeChar = vsCode.c_str(), *fsCodeChar = fsCode.c_str();
	// compile vertex shader
	vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1, &vsCodeChar, NULL);
	glCompileShader(vsID);
	bool vsCompiled = checkCompileErrors(vsID, "vertexShader");
	// compile fragment shader
	fsID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsID, 1, &fsCodeChar, NULL);
	glCompileShader(fsID);
	bool fsCompiled = checkCompileErrors(fsID, "fragmentShader");
	// compile combined shader
	ID = glCreateProgram();
	glAttachShader(ID, vsID);
	glAttachShader(ID, fsID);
	glLinkProgram(ID);
	bool linked = checkLinkErrors(ID);

	if(vsCompiled && fsCompiled && linked)
		ID_ = ID;
	glDeleteShader(vsID);
	glDeleteShader(fsID);
}

ComputeShader::ComputeShader(const std::string& path, std::vector<std::string> flags)
	: paths_({ path }) {
	for (auto const& flag : flags) {
		preprocessorFlags_.insert({ flag, false });
	}
	versionDirective_ = "#version 430 \n";
	compile();
}

ComputeShader::ComputeShader(const std::vector<std::string>& paths, std::vector<std::string> flags)
	: paths_(paths) {
	for (auto const& flag : flags) {
		preprocessorFlags_.insert({ flag, false });
	}
	versionDirective_ = "#version 430 \n";
	compile();
}

void ComputeShader::compile() {
	unsigned int csID = glCreateShader(GL_COMPUTE_SHADER);
	std::string code = versionDirective_ + createPreprocessorFlags() + readShaderFiles(paths_);
	const char* csCode = code.c_str();
	glShaderSource(csID, 1, &csCode, NULL);
	glCompileShader(csID);
	bool compiled = checkCompileErrors(csID, "compute");

	ID_ = glCreateProgram();
	glAttachShader(ID_, csID);
	glLinkProgram(ID_);
	bool linked = checkLinkErrors(ID_);

	if (!compiled || !linked) ID_ = 0;
	glDeleteShader(csID);
}