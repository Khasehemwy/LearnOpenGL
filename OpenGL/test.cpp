#include"includes.h"

Camera camera;
float gl_deltaTime;

unsigned int loadTexture(char const* path);
void renderScene(Shader& shader);
void renderScene2(Shader& shader);

void frameBuffer_size_callback(GLFWwindow* window, int width, int heigth)
{
	glViewport(0, 0, width, heigth);
}

float gl_lastX, gl_lastY;
float gl_pitch, gl_yaw;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xOffset = xpos - gl_lastX;
	float yOffset = gl_lastY - ypos;
	gl_lastX = xpos;
	gl_lastY = ypos;
	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;
	gl_yaw += xOffset;
	gl_pitch += yOffset;
	if (gl_pitch > 89.0f)
		gl_pitch = 89.0f;
	if (gl_pitch < -89.0f)
		gl_pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(gl_pitch)) * cos(glm::radians(gl_yaw));
	front.y = sin(glm::radians(gl_pitch));
	front.z = cos(glm::radians(gl_pitch)) * sin(glm::radians(gl_yaw));
	camera.cameraFront = glm::normalize(front);
}
float fov=45.0f;
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	fov -= yOffset;
	if (fov <= 1.0f)fov = 1.0f;
	if (fov >= 60.0f)fov = 60.0f;
}

void ProcessInput(GLFWwindow* window,GLuint shaderProgram)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		int location = glGetUniformLocation(shaderProgram, "alpha");
		float alpha;
		glGetUniformfv(shaderProgram, location, &alpha);
		glUniform1f(location, alpha < 1.0f ? alpha + 0.001f : alpha);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		int location = glGetUniformLocation(shaderProgram, "alpha");
		float alpha;
		glGetUniformfv(shaderProgram, location, &alpha);
		glUniform1f(location, alpha > 0.0f ? alpha - 0.001f : alpha);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.Move(1);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.Move(2);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.Move(3);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.Move(4);
}

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
float texCoords[] = {
	0.0f,0.0f,
	1.0f,0.0f,
	0.5f,1.0f
};
unsigned int indices[] = {
	0,1,3,
	1,2,3
};
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(0.0f,  1.5f, 0.0f),
	glm::vec3(2.0f, 0.0f, 1.0f),
	glm::vec3(-1.0f, 0.0f, 2.0f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
const float window.width = 1400, window.height = 800;

unsigned int texture1;
unsigned int sepcularMap;
unsigned int texGrass;
unsigned int texWindowRed;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window.width, window.height, "???", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader shader("system/vShader.vs", "system/fShader_oit_store.fs");

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
	//颜色
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1); 


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	int width, height;

	texture1 = loadTexture(("./Resources/container2.png"));
	sepcularMap = loadTexture(("./Resources/container2_specular.png"));
	texGrass = loadTexture(("./Resources/grass.png"));
	texWindowRed = loadTexture(("./Resources/windowRed.png"));

	//stbi_set_flip_vertically_on_load(true);
	//data = stbi_load("./Resources/awesomeface.png", &width, &height, &nrChannels, 0);
	//unsigned int texture2;
	//glGenTextures(1, &texture2);
	//glBindTexture(GL_TEXTURE_2D, texture2);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//stbi_image_free(data);

	//光照
	Shader lightShader("", "system/fShader_light.fs");
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	lightShader.use();
	glm::vec3 lightPos(-1.0f, 4.0f, -1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//阴影
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const GLuint SHADOW_WIDTH = window.width*4, SHADOW_HEIGHT = window.height*4;
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1,1,1,1 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Shader simpleDepthShader("system/vShader_shadow.vs", "system/fShader_shadow.fs");

	//*******Shader设置**********
	shader.use();
	//材质
	shader.set("material.texture_diffuse0", 0);
	shader.set("material.texture_specular0", 1);
	shader.set("material.shininess", 128.0f);
	shader.set("shadowMap", 2);
	glEnable(GL_DEPTH_TEST);

	//混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//逐像素链表OIT
	shader.use();
	GLuint* oit_data;
	size_t total_pixel = window.width * window.height;

	GLuint head_pointer_texture;
	glActiveTexture(GL_TEXTURE3);
	glGenTextures(1, &head_pointer_texture);
	glBindTexture(GL_TEXTURE_2D, head_pointer_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, window.width, window.height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindImageTexture(3, head_pointer_texture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);

	GLuint head_pointer_initializer;
	glGenBuffers(1, &head_pointer_initializer);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_initializer);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, total_pixel * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	oit_data = (GLuint*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	memset(oit_data, 0x00, total_pixel * sizeof(GLuint));
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

	GLuint atomic_counter_buffer;
	glGenBuffers(1, &atomic_counter_buffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter_buffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);

	GLuint linked_list_buffer;
	glGenBuffers(1, &linked_list_buffer);
	glBindBuffer(GL_TEXTURE_BUFFER, linked_list_buffer);
	glBufferData(GL_TEXTURE_BUFFER, 3 * total_pixel * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	GLuint linked_list_texture;
	glActiveTexture(GL_TEXTURE4);
	glGenTextures(1, &linked_list_texture);
	glBindTexture(GL_TEXTURE_BUFFER, linked_list_texture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, linked_list_buffer);
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	glBindImageTexture(4, linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);


	Shader shader_blend("", "system/fShader_oit_blend.fs");

	

	//***************************

	//变换矩阵
	glm::mat4 model(1.0f),view(1.0f),projection(1.0f);


	//鼠标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	float lastX = (float)window.width / 2;
	float lastY = (float)window.height / 2;
	gl_lastX = lastX; gl_lastY = lastY;

	//滚轮
	glfwSetScrollCallback(window, scroll_callback);

	//时间
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧的时间
	float currentFrame = glfwGetTime();


	while (!glfwWindowShouldClose(window))
	{
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.cameraSpeed = std::max(deltaTime * 20000.5f, 0.015f);
		ProcessInput(window, shader.ID);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glClearColor(0.09, 0.25, 0.32, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//OIT重置
		shader.use();
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_counter_buffer);
		GLuint* data = (GLuint*)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_WRITE_ONLY);
		data[0] = 0;
		glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_initializer);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, head_pointer_texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, window.width, window.height, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindImageTexture(3, head_pointer_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

		glBindImageTexture(4, linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);


		//光源属性
		//glm::vec3 lightDir = lightPos - glm::vec3(0, 0, 0);
		shader.set("light.position", lightPos);
		//ourShader.set("light.direction", lightDir);
		shader.set("light.ambient", { 0.4f, 0.4f, 0.4f });
		shader.set("light.diffuse", { 0.6f, 0.6f, 0.6f });
		shader.set("light.specular", { 1.0f, 1.0f, 1.0f });

		//ourShader.set("light.constant", 1.0f);
		//ourShader.set("light.linear", 0.007f);
		//ourShader.set("light.quadratic", 0.0002f);

		//ourShader.set("light.cutOff", glm::cos(glm::radians(17.5f)));
		//ourShader.set("light.outerCutOff", glm::cos(glm::radians(22.5f)));

		////阴影
		glEnable(GL_DEPTH_TEST);
		simpleDepthShader.use();
		projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f);
		//projection = glm::perspective(glm::radians(fov), (float)screenWidth / screenHeight, 0.1f, 100.0f);
		glm::vec3 lightUp = camera.cameraUp;
		view = glm::lookAt(lightPos, glm::vec3(0, 0, 0), lightUp);
		glm::mat4 lightSpaceMatrix = projection * view;
		simpleDepthShader.set("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		renderScene(simpleDepthShader);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, window.width, window.height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





		glViewport(0, 0, window.width, window.height);
		shader.use();
		projection = glm::perspective(glm::radians(fov), window.width / window.height, 0.1f, 100.0f);
		camera.view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
		shader.set("projection", projection);
		shader.set("view", camera.view);
		shader.set("viewPos", camera.cameraPos);
		shader.set("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, sepcularMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glActiveTexture(GL_TEXTURE0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		//glDepthMask(GL_FALSE);
		//glDisable(GL_CULL_FACE);
		renderScene(shader);

		shader_blend.use();
		shader_blend.set("projection", projection);
		shader_blend.set("view", camera.view);
		shader_blend.set("viewPos", camera.cameraPos);
		shader_blend.set("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, head_pointer_texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, linked_list_texture);
		glActiveTexture(GL_TEXTURE0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderScene(shader_blend);


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

		glfwSwapBuffers(window);
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
	for (int i = 0; i < 10; i++) {
		glm::mat4 model(1.0f);
		model = glm::translate(model, cubePositions[i]);
		model = glm::translate(model, { 0,-5,0 });
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		shader.set("model", model);
		glActiveTexture(GL_TEXTURE0);
		if(i<5)glBindTexture(GL_TEXTURE_2D, texWindowRed);
		else glBindTexture(GL_TEXTURE_2D, texture1);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));
	}
	//地面
	model = glm::mat4(1.0f);
	model = glm::translate(model, { 0,-5,0 });
	model = glm::scale(model, { 50,0.5,50 });
	shader.set("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));
}

void renderScene2(Shader& shader)
{
	shader.use();

	glm::mat4 model(1.0f);
	glBindVertexArray(VAO);
	for (int i = 0; i < 10; i++) {
		glm::mat4 model(1.0f);
		model = glm::translate(model, cubePositions[i]);
		model = glm::translate(model, { 0,5,0 });
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		shader.set("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texWindowRed);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));
	}
	//地面
	model = glm::mat4(1.0f);
	model = glm::translate(model, { 0,-5,0 });
	model = glm::scale(model, { 50,0.5,50 });
	shader.set("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}