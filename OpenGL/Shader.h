#pragma once

//#ifdef SHADER_H
//#define SHADER_H

#include"includes.h"

class Shader
{
private:
    unsigned int CreateSingleShader(const char* shaderFilePath, unsigned int shaderFlag);
public:
	unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    //设置shader
    void Set_Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    // 使用/激活程序
    void use();
    // uniform工具函数
    void set(const std::string& name, bool value) const;
    void set(const std::string& name, int value) const;
    void set(const std::string& name, float value) const;
    void set(const std::string& name, glm::mat4 value) const;
    void set(const std::string& name, float r, float g, float b) const;
    void set(const std::string& name, glm::vec3 value) const;
    
};
//#endif //SHADER_H

