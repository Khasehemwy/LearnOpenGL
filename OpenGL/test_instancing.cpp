#include"includes.h"

Camera camera;
float gl_deltaTime;

float g_fov = 45.0f;
unsigned int VAO, VBO, EBO;

const unsigned int asteroidNum = 5000;
glm::mat4 asteroids[asteroidNum];

void renderScene(Shader& shader,Model& rock)
{
	srand(glfwGetTime());
	float radius = 50.0f;
	float offset = 2.5f;
	glm::mat4 model(1.0f);
	for (int i = 0; i < asteroidNum; i++) {
		model = glm::mat4(1.0);
		float angle = (float)i / (float)asteroidNum * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		asteroids[i] = model;
	}
	rock.DrawInstance(shader, asteroids, asteroidNum);
}

int main()
{
	Window window(1400, 800, "???", NULL, NULL);

	Shader shader("system/vShader_instancing.vs", "system/fShader_instancing.fs");
	Shader shader_planet("system/vShader.vs", "system/fShader_instancing.fs");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	Model planet("Resources/planet/planet.obj");
	Model rock("Resources/rock/rock.obj");

	//*******设置**********
	shader.use();
	glEnable(GL_DEPTH_TEST);

	shader_planet.use();
	glEnable(GL_DEPTH_TEST);

	camera.cameraPos = { 0,0,80 };
	//***************************
	
	//变换矩阵
	glm::mat4 model(1.0f), view(1.0f), projection(1.0f);

	//输入交互
	Input inputs(&window, &shader, &camera);
	inputs.EnableCursor();
	inputs.EnableScroll(&g_fov);

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

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glClearColor(0.09, 0.25, 0.32, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glViewport(0, 0, window.width, window.height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, window.width, window.height);
		shader_planet.use();
		projection = glm::perspective(glm::radians(g_fov), window.width / window.height, 0.1f, 100.0f);
		camera.view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
		shader_planet.set("projection", projection);
		shader_planet.set("view", camera.view);
		shader_planet.set("viewPos", camera.cameraPos);


		model = glm::mat4(1.0);
		model = glm::scale(model, { 10,10,10 });
		//model = glm::translate(model, glm::vec3(3.0f, -1.5f, 0.0f)); // translate it down so it's at the center of the scene
		shader_planet.set("model", model);

		planet.Draw(shader_planet);

		shader.use();
		shader.set("projection", projection);
		shader.set("view", camera.view);
		shader.set("viewPos", camera.cameraPos);
		renderScene(shader, rock);

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}
