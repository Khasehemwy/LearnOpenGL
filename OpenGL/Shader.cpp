#include "Shader.h"

unsigned int Shader::CreateSingleShader(const char* shaderFilePath, unsigned int shaderFlag)
{
	std::string shaderCodeStr;
	const char* shaderCode;

	//从文件读取"着色器代码"并储存为const char*.
	std::ifstream shaderFile(shaderFilePath);
	std::ostringstream shaderStream;
	char ch;
	while (shaderStream && shaderFile.get(ch)) { shaderStream.put(ch); }
	shaderFile.close();
	shaderCodeStr = shaderStream.str();
	shaderCode = shaderCodeStr.c_str();

	//编译和链接着色器
	unsigned int singleShader;
	int success; char infoLog[512];
	singleShader = glCreateShader(shaderFlag);
	glShaderSource(singleShader, 1, &shaderCode, NULL);
	glCompileShader(singleShader);
	glGetShaderiv(singleShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(singleShader, 512, NULL, infoLog);
		if(shaderFlag == GL_VERTEX_SHADER)
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n" << infoLog << std::endl;
		if(shaderFlag == GL_FRAGMENT_SHADER)
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n" << infoLog << std::endl;
	};

	return singleShader;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	int success; char infoLog[512];

	if (!vertexPath || vertexPath == "") { vertexPath = "system/vShader.vs"; }
	if (!fragmentPath || fragmentPath == "") { fragmentPath = "system/fShader.fs"; }

	bool hasGeometryShader = false;
	if (geometryPath && geometryPath != "") { hasGeometryShader = true; }

	//创建顶点和片段着色器
	unsigned int vertexShader, fragmentShader;
	vertexShader = CreateSingleShader(vertexPath, GL_VERTEX_SHADER);
	fragmentShader = CreateSingleShader(fragmentPath, GL_FRAGMENT_SHADER);

	//额外的着色器
	unsigned int geometryShader;
	if (hasGeometryShader) {
		geometryShader = CreateSingleShader(geometryPath, GL_GEOMETRY_SHADER);
	}

	//着色器程序 对象
	this->ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	if (hasGeometryShader) { glAttachShader(ID, geometryShader); }
	glLinkProgram(ID);
	// 打印连接错误（如果有的话）
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED:\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (hasGeometryShader) { glDeleteShader(geometryShader); }
}

void Shader::Set_Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	int success; char infoLog[512];

	if (!vertexPath || vertexPath == "") { vertexPath = "system/vShader.vs"; }
	if (!fragmentPath || fragmentPath == "") { fragmentPath = "system/fShader.fs"; }

	bool hasGeometryShader = false;
	if (geometryPath && geometryPath != "") { hasGeometryShader = true; }

	//创建顶点和片段着色器
	unsigned int vertexShader, fragmentShader;
	vertexShader = CreateSingleShader(vertexPath, GL_VERTEX_SHADER);
	fragmentShader = CreateSingleShader(fragmentPath, GL_FRAGMENT_SHADER);

	//额外的着色器
	unsigned int geometryShader;
	if (hasGeometryShader) {
		geometryShader = CreateSingleShader(geometryPath, GL_GEOMETRY_SHADER);
	}

	//着色器程序 对象
	this->ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	if (hasGeometryShader) { glAttachShader(ID, geometryShader); }
	glLinkProgram(ID);
	// 打印连接错误（如果有的话）
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED:\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (hasGeometryShader) { glDeleteShader(geometryShader); }
}

void Shader::use()
{
	glUseProgram(this->ID);
}
void Shader::set(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::set(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::set(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set(const std::string& name, float r, float g, float b) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
}
void Shader::set(const std::string& name, glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
