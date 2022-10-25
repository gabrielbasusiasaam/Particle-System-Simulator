#pragma once
#ifndef SHADER_H
#define SHADER_H

typedef struct Material Material;
typedef struct Light Light;

class Shader {
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setMatrix4x4(const std::string& name, glm::mat4x4 matrix) const;
	void setMaterial(Material material) const;
	void setLight(Light light) const;
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};
#endif