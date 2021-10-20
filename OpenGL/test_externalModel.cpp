#include"includes.h"

Camera camera;
float gl_deltaTime;

float fov = 45.0f;

unsigned int VAO, VBO, EBO;

float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

int main()
{
	Window window(1400, 800, "???", NULL, NULL);

	Shader ourShader("system/vShader.vs", "system/fShader_model_multiLight.fs");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	Model ourModel("Resources/chair/uploads_files_2769712_Couch+William+302.obj");

	glGenBuffers(1, &VBO);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//光照
	Shader lightShader("", "system/fShader_light.fs");
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	lightShader.use();
	glm::vec3 lightPos(4.2f, -1.0f, 1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//变换矩阵
	ourShader.use();
	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));
	camera.view = view;
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(fov), (float)window.width / (float)window.height, 0.1f, 100.0f);
	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧的时间
	float currentFrame = glfwGetTime();

	//交互
	Input inputs(&window, &ourShader, &camera);
	inputs.EnableCursor();
	inputs.EnableScroll(&fov);

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
		camera.view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
		ourShader.set("view", camera.view);
		ourShader.set("viewPos", camera.cameraPos);
		projection = glm::perspective(glm::radians(fov), window.width / window.height, 0.1f, 100.0f);
		ourShader.set("projection", projection);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -1.5f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		ourShader.set("model", model);

		ourModel.Draw(ourShader);

		//光源
		lightShader.use();
		lightShader.set("view", camera.view);
		lightShader.set("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.set("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}