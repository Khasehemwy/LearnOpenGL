//#undef GLM_FORCE_DEPTH_ZERO_TO_ONE
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include"includes.h"

Camera camera;
float gl_deltaTime;
float fov;

int nrRows = 7;
int nrColumns = 7;
float spacing = 2.5;

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
	Window window(1400, 800, "clip control", NULL, NULL);


	if (GL_ARB_clip_control) {
		glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	}

	//光照
	Shader lightShader("", "system/fShader_light.fs");
	glm::vec3 lightPos(-3.0f, 0.0f, 5.0f);
	glm::vec3 lightColor(50.0f, 0.8f, 0.8f);
	glm::vec3 lightPos2(3.0f, 0.0f, 5.0f);
	glm::vec3 lightColor2(50.0f, 0.8f, 0.8f);

	Shader shader_model("system/test_clipControl_vs.glsl", "system/test_clipControl_fs.glsl");

	shader_model.use();
	shader_model.set("light[0].position", lightPos);
	shader_model.set("light[0].color", lightColor);
	shader_model.set("light[0].constant", 1.0f);
	shader_model.set("light[0].linear", 0.09f);
	shader_model.set("light[0].quadratic", 0.032f);
	shader_model.set("light[0].cutOff", glm::cos(glm::radians(12.5f)));
	shader_model.set("light[0].outerCutOff", glm::cos(glm::radians(17.5f)));
	shader_model.set("light[1].position", lightPos2);
	shader_model.set("light[1].color", lightColor2);
	shader_model.set("light[1].constant", 1.0f);
	shader_model.set("light[1].linear", 0.09f);
	shader_model.set("light[1].quadratic", 0.032f);
	shader_model.set("light[1].cutOff", glm::cos(glm::radians(12.5f)));
	shader_model.set("light[1].outerCutOff", glm::cos(glm::radians(17.5f)));
	shader_model.set("albedo", { 0.5f,0.0f,0.0f });
	shader_model.set("ao", 1.0f);
	shader_model.set("metallic", 0.6f);
	shader_model.set("roughness", 0.5f);

	Model ourModel("Resources/planet/planet.obj");

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

		//glViewport(0, 0, window.width, window.height);

		//天空盒
		projection = glm::perspective(glm::radians(fov), window.width / window.height, 0.1f, 100.0f);
		view = glm::mat4(glm::mat3(camera.view));
		skybox.Draw(projection, view);

		//光源
		//lightShader.use();
		//lightShader.set("view", camera.view);
		//lightShader.set("projection", projection);
		//model = glm::mat4(1.0);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//lightShader.set("model", model);
		//lightShader.set("lightColor", lightColor);
		//ourModel.Draw(lightShader);
		//model = glm::mat4(1.0);
		//model = glm::translate(model, lightPos2);
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//lightShader.set("lightColor", lightColor2);
		//lightShader.set("model", model);
		//ourModel.Draw(lightShader);


		//glEnable(GL_DEPTH_TEST);
		//shader_model.use();
		//shader_model.set("projection", projection);
		//shader_model.set("view", camera.view);
		//shader_model.set("viewPos", camera.cameraPos);
		//model = glm::mat4(1.0);

		//model = glm::mat4(1.0f);
		//model = glm::scale(model, { 0.4,0.4,0.4 });
		//shader_model.set("model", model);
		//ourModel.Draw(shader_model);

		glEnable(GL_DEPTH_TEST);
		shader_model.use();
		shader_model.set("projection", projection);
		shader_model.set("view", camera.view);
		shader_model.set("viewPos", camera.cameraPos);
		model = glm::mat4(1.0);
		for (int row = 0; row < nrRows; ++row)
		{
			shader_model.set("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; ++col)
			{
				// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
				// on direct lighting.
				shader_model.set("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(col - (nrColumns / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f
				));
				model = glm::scale(model, { 0.4,0.4,0.4 });
				shader_model.set("model", model);
				ourModel.Draw(shader_model);
			}
		}

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}