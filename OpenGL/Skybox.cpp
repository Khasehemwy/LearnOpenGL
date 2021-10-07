#include "Skybox.h"

float Skybox::skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
};

Skybox::Skybox() :
	shader(new Shader("system/vShader_skybox.vs", "system/fShader_skybox.fs"))
{

}

Skybox::Skybox(const std::vector<std::string>& texturePaths) :
	Skybox::Skybox()
{
	shader->use();
	shader->set("cubeMap", 0);

	skyboxTexture = LoadTextureCube(texturePaths);

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int Skybox::Draw(glm::mat4& projection, glm::mat4& view)
{
	// 为了方便起见,不优化. 天空盒应在场景最开始绘制.
	bool isEnableDepthTest = false;
	if (glIsEnabled(GL_DEPTH_TEST)) { isEnableDepthTest = true; }
	//glDepthFunc(GL_LEQUAL);
	//glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	shader->use();
	shader->set("projection", projection);
	shader->set("view", view);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);
	if (isEnableDepthTest) { glEnable(GL_DEPTH_TEST); }
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glClear(GL_DEPTH_BUFFER_BIT);

	return 0;
}

Shader* Skybox::GetShaderPtr()
{
	return this->shader;
}

const unsigned int Skybox::GetSkyboxTexture()
{
	return this->skyboxTexture;
}
