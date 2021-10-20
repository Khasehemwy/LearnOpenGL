#include"includes.h"

Camera camera;
float gl_deltaTime;

float fov = 45.0f;

float vertices[] = {
	// positions          
	0.6f, 0.6f, 0.0f, 0.0f, 0.0f,
	0.6f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  0.6f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, 0.6f, 0.0f, 1.0f, 0.0f,
	0.6f, 1.0f, 0.0f, 0.0f, 1.0f,
};

int main()
{
	Window window(1400, 800, "???", NULL, NULL);

	Shader ourShader("system/vShader.vs", "system/fShader_model.fs");
	Shader shader_tex("system/vShader_frameBuffer.vs", "system/fShader_frameBuffer.fs");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	Model ourModel("Resources/chair/uploads_files_2769712_Couch+William+302.obj");

	//FrameBuffer Texture
	ourShader.use();
	unsigned int frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.width, window.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.width, window.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// texture绘制的信息
	unsigned int texVAO;
	unsigned int texVBO;
	glGenVertexArrays(1, &texVAO);
	glBindVertexArray(texVAO);
	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//变换矩阵
	ourShader.use();
	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));
	camera.view = view;
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(fov), (float)window.width / (float)window.height, 0.1f, 100.0f);

	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧的时间
	float currentFrame = glfwGetTime();

	//交互
	Input inputs(&window, &ourShader, &camera);
	inputs.EnableCursor();
	inputs.EnableScroll(&fov);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window.window))
	{
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.cameraSpeed = std::max(deltaTime * 20000.5f, 0.01f);
		inputs.ProcessInput();

		glClearColor(0.09, 0.25, 0.32, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ourShader.use();

		//光源属性
		ourShader.set("light.position", camera.cameraPos);
		ourShader.set("light.direction", camera.cameraFront);
		ourShader.set("light.ambient", { 0.8f, 0.8f, 0.8f });
		ourShader.set("light.diffuse", { 0.8f, 0.8f, 0.8f }); // 将光照调暗了一些以搭配场景
		ourShader.set("light.specular", { 1.0f, 1.0f, 1.0f });

		ourShader.set("light.constant", 1.0f);
		ourShader.set("light.linear", 0.09f);
		ourShader.set("light.quadratic", 0.032f);

		ourShader.set("light.cutOff", glm::cos(glm::radians(12.5f)));
		ourShader.set("light.outerCutOff", glm::cos(glm::radians(17.5f)));

		//材质
		//ourShader.set("material.ambient", { 1.0f, 0.5f, 0.31f });
		ourShader.set("material.shininess", 32.0f);

		//坐标变换
		glViewport(0, 0, window.width, window.height);
		camera.view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
		ourShader.set("view", camera.view);
		ourShader.set("viewPos", camera.cameraPos);
		projection = glm::perspective(glm::radians(fov), window.width / window.height, 0.1f, 100.0f);
		ourShader.set("projection", projection);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -1.5f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		ourShader.set("model", model);

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClearColor(0.19, 0.35, 0.42, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ourModel.Draw(ourShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ourModel.Draw(ourShader);
		//绘制用于采样渲染的纹理
		glDisable(GL_DEPTH_TEST);
		shader_tex.use();
		glBindVertexArray(texVAO);
		shader_tex.set("screenTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);

		glEnable(GL_DEPTH_TEST);
		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}