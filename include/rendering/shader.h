#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderBase {
public:
	ShaderBase() : ID_(0) {};
	// set uniforms
	void setUniform(const std::string& name, bool value);
	void setUniform(const std::string& name, int value);
	void setUniform(const std::string& name, float value);
	void setUniform(const std::string& name, glm::vec2 value);
	void setUniform(const std::string& name, glm::vec3 value);
	void setUniform(const std::string& name, glm::vec4 value);
	void setUniform(const std::string& name, glm::mat4 value);

	void reload();
	void setBlockBinding(const std::string& name, unsigned int binding);

	void setFlag(std::string flag, bool value);
	std::map<std::string, bool> getFlags() const {
		return preprocessorFlags_;
	}

	unsigned int getID() const { return ID_; }

	void use();

protected:
	unsigned int ID_;

	virtual void compile() = 0;

	std::string readShaderFiles(std::vector<std::string> paths) const;
	bool checkCompileErrors(int shader, std::string name);
	bool checkLinkErrors(int shader);

	std::string versionDirective_;

	std::map<std::string, bool> preprocessorFlags_;
	std::string createPreprocessorFlags() const;
};
class Shader : public ShaderBase {
public:
	// read shader file contents and compile shader code
	Shader() {};
	Shader(const std::string& vsPath, const std::string& fsPath, std::vector<std::string> flags = {});
	Shader(const std::vector<std::string>& vsPaths, const std::vector<std::string>& fsPaths, std::vector<std::string> flags = {});

private:
	std::vector<std::string> vsPaths_;
	std::vector<std::string> fsPaths_;
	

	void compile() override;
};

class ComputeShader : public ShaderBase {
public:

	ComputeShader() {};
	ComputeShader(const std::string& path, std::vector<std::string> flags = {});
	ComputeShader(const std::vector<std::string>& paths, std::vector<std::string> flags = {});

private:
	std::vector<std::string> paths_;
	void compile() override;
};
