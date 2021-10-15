#include"includes.h"

Camera camera;
float gl_deltaTime;
float fov;
float g_time = 0.0f;
float g_exposure = 0.5f;

void renderScene(Shader& shader);
void renderScene2(Shader& shader);

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

	Shader shader("system/vShader.vs", "system/fShader_multiLight.fs");
	Shader shader_hdr("system/vShader_frameBuffer.vs", "system/fShader_HDR.fs");

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

	texture1 = LoadTexture(("./Resources/container2.png"));
	sepcularMap = LoadTexture(("./Resources/container2_specular.png"));

	//光照
	Shader lightShader("", "system/fShader_light.fs");
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	lightShader.use();
	glm::vec3 lightPos(0.0f, 0.0f, -10.0f);
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
	GLuint hdrRBO;
	glGenRenderbuffers(1, &hdrRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, hdrRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window.width, window.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrRBO);
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


	//*******Shader设置**********
	shader.use();
	//材质
	shader.set("material.texture_diffuse0", 0);
	shader.set("material.texture_specular0", 1);
	shader.set("material.shininess", 128.0f);
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

		glClearColor(0.09, 0.25, 0.32, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//光源属性
		shader.use();
		//glm::vec3 lightDir = lightPos - glm::vec3(0, 0, 0);
		shader.set("light.position", lightPos);
		//ourShader.set("light.direction", lightDir);
		shader.set("light.ambient", { 200.0f, 200.0f, 200.0f });
		shader.set("light.diffuse", { 0.9f, 0.9f, 0.9f });
		shader.set("light.specular", { 1.0f, 1.0f, 1.0f });

		shader.set("light.constant", 1.0f);
		shader.set("light.linear", 1.4f);
		shader.set("light.quadratic", 5.6f);

		//ourShader.set("light.cutOff", glm::cos(glm::radians(17.5f)));
		//ourShader.set("light.outerCutOff", glm::cos(glm::radians(22.5f)));

		projection = glm::perspective(glm::radians(fov), window.width / window.height, 0.1f, 100.0f);
		camera.view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);

		//绘制到浮点缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

		//天空盒
		view = glm::mat4(glm::mat3(camera.view));
		skybox.Draw(projection, view);

		//场景
		glViewport(0, 0, window.width, window.height);
		shader.use();
		shader.set("projection", projection);
		shader.set("view", camera.view);
		shader.set("viewPos", camera.cameraPos);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, sepcularMap);
		glActiveTexture(GL_TEXTURE0);
		renderScene(shader);

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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//绘制用于采样渲染的纹理
		glDisable(GL_DEPTH_TEST);
		shader_hdr.use();
		glBindVertexArray(texVAO);
		shader_hdr.set("screenTexture", 0);
		//自适应曝光
		if (camera.cameraPos.z > -10.0f) {
			g_exposure = 0.3f;
			shader_hdr.set("exposure", g_exposure);
		}
		else {
			auto timeTween=[]() {
				float timeValue = glfwGetTime();
				if (g_time <= 0.00001f) {
					g_time = timeValue;
				}
				else {
					if (timeValue - g_time >= 0.01f) {
						g_exposure += 0.01f;
						g_time = timeValue;
					}
					g_exposure = std::min(g_exposure, 2.0f);
				}
			};
			timeTween();
			shader_hdr.set("exposure", g_exposure);
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_DEPTH_TEST); 


		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

void renderScene(Shader& shader)
{
	shader.use();
	glm::mat4 model(1.0f);
	glBindVertexArray(VAO);
	model = glm::scale(model, { 1,1,20 });
	shader.set("model", model);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));
	glBindTexture(GL_TEXTURE_2D, 0);
}