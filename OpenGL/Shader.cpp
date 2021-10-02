#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	if (!vertexPath || vertexPath == "") { vertexPath = "system/vShader.vs"; }
	if (!fragmentPath|| fragmentPath == "") { fragmentPath = "system/fShader.fs"; }

	std::string vertexCode;
	std::string fragmentCode;

	const char* vShaderCode;
	const char* fShaderCode;

	std::ifstream vShaderFile(vertexPath);
	std::ifstream fShaderFile(fragmentPath);

	//从文件读取"着色器代码"并储存为const char*.
	std::ostringstream vShaderStream, fShaderStream;
	char ch;
	while (vShaderStream && vShaderFile.get(ch)) { vShaderStream.put(ch); }
	while (fShaderStream && fShaderFile.get(ch)) { fShaderStream.put(ch); }
	vShaderFile.close();
	fShaderFile.close();
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();
	vShaderCode = vertexCode.c_str();
	fShaderCode = fragmentCode.c_str();

	//创建顶点和片段着色器
	unsigned int vertexShader, fragmentShader;
	int success; char infoLog[512];

	//编译和链接着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n" << infoLog << std::endl;
	};

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n" << infoLog << std::endl;
	};

	//着色器程序 对象
	this->ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
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
}

void Shader::Set_Shader(const char* vertexPath, const char* fragmentPath)
{
	if (!vertexPath || vertexPath == "") { vertexPath = "system/vShader.vs"; }
	if (!fragmentPath || fragmentPath == "") { fragmentPath = "system/fShader.fs"; }

	std::string vertexCode;
	std::string fragmentCode;

	const char* vShaderCode;
	const char* fShaderCode;

	std::ifstream vShaderFile(vertexPath);
	std::ifstream fShaderFile(fragmentPath);

	//从文件读取"着色器代码"并储存为const char*.
	std::ostringstream vShaderStream, fShaderStream;
	char ch;
	while (vShaderStream && vShaderFile.get(ch)) { vShaderStream.put(ch); }
	while (fShaderStream && fShaderFile.get(ch)) { fShaderStream.put(ch); }
	vShaderFile.close();
	fShaderFile.close();
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();
	vShaderCode = vertexCode.c_str();
	fShaderCode = fragmentCode.c_str();

	//创建顶点和片段着色器
	unsigned int vertexShader, fragmentShader;
	int success; char infoLog[512];

	//编译和链接着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n" << infoLog << std::endl;
	};

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n" << infoLog << std::endl;
	};

	//着色器程序 对象
	this->ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
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
