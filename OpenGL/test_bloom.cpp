#include"includes.h"

Camera camera;
float gl_deltaTime;
float fov;
float g_time = 0.0f;
float g_exposure = 0.5f;

unsigned int VAO, VBO, EBO;

float vertices[] = {
	// positions          // normals           // texture coords//
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,

	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
};

float vertices_screen[] = {
	// positions          
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, //左下
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f, //左上
	 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, //右下
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, //右上
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, //右下
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, //左上
};

std::vector<std::string> skyBoxPaths{
	"./Resources/skybox/right.jpg",
	"./Resources/skybox/left.jpg",
	"./Resources/skybox/top.jpg",
	"./Resources/skybox/bottom.jpg",
	"./Resources/skybox/front.jpg",
	"./Resources/skybox/back.jpg"
};

unsigned int texture1;
unsigned int sepcularMap;
unsigned int texGrass;
unsigned int texWindowRed;

int main()
{
	Window window(1400, 800, "???", NULL, NULL);


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Shader shader("system/vShader.vs", "system/fShader_model_bloom.fs");
	Shader shader_hdr("system/vShader_frameBuffer.vs", "system/fShader_HDR_bloom.fs");
	Shader shader_blur("system/vShader_frameBuffer.vs", "system/fShader_bloom_blur.fs");

	glBindVertexArray(VAO);
	//顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//法向量
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//纹理
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	int width, height;

	Model ourModel("Resources/chair/uploads_files_2769712_Couch+William+302.obj");

	//光照
	Shader lightShader("", "system/fShader_light_bloom.fs");
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	lightShader.use();
	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//HDR
	GLuint hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	GLuint hdrTex;
	glGenTextures(1, &hdrTex);
	glBindTexture(GL_TEXTURE_2D, hdrTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window.width, window.height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //必须设置
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrTex, 0);
	GLuint bloomOriginTex;
	glGenTextures(1, &bloomOriginTex);
	glBindTexture(GL_TEXTURE_2D, bloomOriginTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window.width, window.height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //必须设置
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bloomOriginTex, 0);
	GLuint hdrRBO;
	glGenRenderbuffers(1, &hdrRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, hdrRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window.width, window.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrRBO);
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// texture绘制的信息
	unsigned int texVAO;
	unsigned int texVBO;
	glGenVertexArrays(1, &texVAO);
	glBindVertexArray(texVAO);
	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_screen), vertices_screen, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Bloom
	//生成两个用于模糊处理的缓冲和贴图
	GLuint pingpongFBO[2];
	glGenFramebuffers(2, pingpongFBO);
	GLuint pingpongTex[2];
	glGenTextures(2, pingpongTex);
	for (int i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongTex[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window.width, window.height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTex[i], 0);
	}


	//*******Shader设置**********
	shader.use();
	//材质
	shader.set("material.shininess", 16.0f);
	glEnable(GL_DEPTH_TEST);

	//天空盒
	Skybox skybox(skyBoxPaths);

	//***************************

	//变换矩阵
	glm::mat4 model(1.0f), view(1.0f), projection(1.0f);


	//输入交互
	Input inputs(&window, &shader, &camera);
	inputs.EnableCursor();
	inputs.EnableScroll(&fov);

	//时间
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧的时间
	float currentFrame = glfwGetTime();


	while (!glfwWindowShouldClose(window.window))
	{
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.cameraSpeed = std::max(deltaTime * 20000.5f, 0.015f);
		inputs.ProcessInput();

		glClearColor(0.0, 0.0, 0.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//光源属性
		shader.use();
		shader.set("light.type", 2);
		//glm::vec3 lightDir = lightPos - glm::vec3(0, 0, 0);
		shader.set("light.position", lightPos);
		//ourShader.set("light.direction", lightDir);
		shader.set("light.ambient", { 50.0f, 50.0f, 50.0f });
		shader.set("light.diffuse", { 150.0f, 150.0f, 150.0f });
		shader.set("light.specular", { 150.0f, 150.0f, 150.0f });

		shader.set("light.constant", 1.0f);
		shader.set("light.linear", 1.4f);
		shader.set("light.quadratic", 5.6f);

		//ourShader.set("light.cutOff", glm::cos(glm::radians(17.5f)));
		//ourShader.set("light.outerCutOff", glm::cos(glm::radians(22.5f)));

		projection = glm::perspective(glm::radians(fov), window.width / window.height, 0.1f, 100.0f);
		camera.view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);

		//绘制到浮点缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//天空盒
		view = glm::mat4(glm::mat3(camera.view));
		skybox.Draw(projection, view);

		//场景
		glViewport(0, 0, window.width, window.height);
		shader.use();
		shader.set("projection", projection);
		shader.set("view", camera.view);
		model = glm::mat4(1.0);
		model = glm::translate(model, { 0,-1,-2 });
		shader.set("model", model);
		shader.set("viewPos", camera.cameraPos);
		ourModel.Draw(shader);

		//画光源
		lightShader.use();
		camera.view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
		projection = glm::perspective(glm::radians(fov), window.width / window.height, 0.1f, 100.0f);
		lightShader.set("view", camera.view);
		lightShader.set("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.set("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//处理模糊
		GLboolean horizontal = true;
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[0]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[1]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader_blur.use();
		for (int i = 0; i < 30; i++) {
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			shader_blur.set("image", 0);
			shader_blur.set("horizontal", horizontal);
			glActiveTexture(GL_TEXTURE0);
			if (i == 0) {
				glBindTexture(GL_TEXTURE_2D, bloomOriginTex);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, pingpongTex[!horizontal]);
			}
			glDisable(GL_DEPTH_TEST);
			glBindVertexArray(texVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindTexture(GL_TEXTURE_2D, 0);
			glEnable(GL_DEPTH_TEST);
			horizontal = !horizontal;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//绘制用于采样渲染的纹理
		glDisable(GL_DEPTH_TEST);
		shader_hdr.use();
		glBindVertexArray(texVAO);
		shader_hdr.set("screenTexture", 0);
		shader_hdr.set("bloomTexture", 1);
		//自适应曝光
		g_exposure = 1.2f;
		shader_hdr.set("exposure", g_exposure);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongTex[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}