#include"includes.h"

Camera camera;
float gl_deltaTime;
float fov;

unsigned int VAO, VBO, EBO;

std::vector<std::string> skyBoxPaths{
	"./Resources/skybox/right.jpg",
	"./Resources/skybox/left.jpg",
	"./Resources/skybox/top.jpg",
	"./Resources/skybox/bottom.jpg",
	"./Resources/skybox/front.jpg",
	"./Resources/skybox/back.jpg"
};

int main()
{
	Window window(1400, 800, "???", NULL, NULL);

	Shader shader_model("system/vShader.vs", "system/fShader_environment.fs");

	shader_model.use();
	shader_model.set("light.ambient", { 0.8f, 0.8f, 0.8f });
	shader_model.set("light.diffuse", { 0.8f, 0.8f, 0.8f });
	shader_model.set("light.specular", { 1.0f, 1.0f, 1.0f });
	shader_model.set("light.constant", 1.0f);
	shader_model.set("light.linear", 0.09f);
	shader_model.set("light.quadratic", 0.032f);
	shader_model.set("light.cutOff", glm::cos(glm::radians(12.5f)));
	shader_model.set("light.outerCutOff", glm::cos(glm::radians(17.5f)));
	shader_model.set("material.shininess", 32.0f);

	shader_model.set("cubeMap", 4);


	Model ourModel("Resources/TV/uploads_files_2941243_retrotv0319.obj");

	//天空盒
	Skybox skybox(skyBoxPaths);

	//变换矩阵
	glm::mat4 model(1.0f), view(1.0f), projection(1.0f);


	//输入交互
	Input inputs(&window, skybox.GetShaderPtr(), &camera);
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
		camera.cameraSpeed = std::max(deltaTime * 20000.5f, 0.01f);
		inputs.ProcessInput();

		glClearColor(0.09, 0.25, 0.32, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projection = glm::perspective(glm::radians(fov), window.width / window.height, 0.1f, 100.0f);
		camera.view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);

		shader_model.set("light.position", camera.cameraPos);
		shader_model.set("light.direction", camera.cameraFront);

		//glViewport(0, 0, window.width, window.height);

		//天空盒
		projection = glm::perspective(glm::radians(fov), window.width / window.height, 0.1f, 100.0f);
		view = glm::mat4(glm::mat3(camera.view));
		skybox.Draw(projection, view);


		glEnable(GL_DEPTH_TEST);
		shader_model.use();
		shader_model.set("projection", projection);
		shader_model.set("view", camera.view);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -1.5f, 0.0f)); 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	
		shader_model.set("model", model);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetSkyboxTexture());

		ourModel.Draw(shader_model);

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}