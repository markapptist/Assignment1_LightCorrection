
//Mark Meritt
//100692252
//PLEASE COMPILE IN x86
//PLEASE COMPILE IN x86
//PLEASE COMPILE IN x86
//PLEASE COMPILE IN x86
//PLEASE COMPILE IN x86

#include <glad/glad.h>
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <GLM/glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height),	
	 mouse_callback(GLFWwindow* window, double xpos, double ypos),			
	 scroll_callback(GLFWwindow* window, double xoffset, double yoffset),	
	 processInput(GLFWwindow *window); 

bool firstMouse = true;
float deltaTime = 0.0f, lastFrame = 0.0f;
const unsigned int SCR_WIDTH = 1600, SCR_HEIGHT = 900;
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;

glm::vec3 ObjectPosition1 = glm::vec3(0.5f, 2.4f, 3.8f);
glm::vec3 ObjectPosition2 = glm::vec3(1.0f, 2.5f, 2.8f);
glm::vec3 ObjectPosition3 = glm::vec3(-0.5f, 2.4f, 3.8f);
glm::vec3 ObjectPosition4 = glm::vec3(-1.0f, 2.5f, 2.8f);

glm::vec3 ObjectScale1 = glm::vec3(0.02f, 0.02f, 0.02f);
glm::vec3 ObjectScale2 = glm::vec3(0.02f, 0.02f, 0.02f);
glm::vec3 ObjectScale3 = glm::vec3(0.02f, 0.02f, 0.02f);
glm::vec3 ObjectScale4 = glm::vec3(0.02f, 0.02f, 0.02f);

glm::vec3 ufoPos = glm::vec3(0.0f, 2.35f, 3.5f);
glm::vec3 ufoScale = glm::vec3(0.025f, 0.025f, 0.025f);

glm::vec3 levelPos = glm::vec3(0.0f, 0.45f, 0.0f);
glm::vec3 levelScale = glm::vec3(0.1f, 0.1f, 0.1f);

Shader basicLight;

Camera camera(glm::vec3(0.0f, 2.6f, 7.0f)); 

glm::vec3 lightPos = ObjectPosition1;

glm::mat4 modelTransform = glm::mat4(1.0f);

glm::mat4 _ufo, o1, o2, o3, o4, _room;

bool noLight = false;
bool ambient = false;
bool specular = false;
bool spec_rim = false;
bool ambient_spec_rim = false;
bool rampShading = false;

//glm::vec3 pointLightPositions[] = {
//  ObjectPosition1,
//   ObjectPosition2,
//   ObjectPosition3,
//   ObjectPosition4
//};

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 1 - Lighting & Color Correction", NULL, NULL);
	if (window == NULL) {
		std::cout << "> Failed to create GLFW window" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "> Failed to initialize GLAD" << std::endl;
		system("pause");
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	Model object1("./Assets/Models/nanosuit.obj"),
		object2("./Assets/Models/nanosuit.obj"),
		object3("./Assets/Models/nanosuit.obj"),
		object4("./Assets/Models/nanosuit.obj"),
		Level("./Assets/Models/floor/Low_poly_UFO.obj"),
		room("./Assets/Models/room/ut.obj");

	_ufo = glm::translate(_ufo, ufoPos);
	o1 = glm::translate(o1, ObjectPosition1);
	o2 = glm::translate(o2, ObjectPosition2);
	o3 = glm::translate(o3, ObjectPosition3);
	o4 = glm::translate(o4, ObjectPosition4);
	_room = glm::translate(_room, levelPos);

	_ufo = glm::scale(_ufo, ufoScale);
	o1 = glm::scale(o1, ObjectScale1);
	o2 = glm::scale(o2, ObjectScale1);
	o3 = glm::scale(o3, ObjectScale1);
	o4 = glm::scale(o4, ObjectScale1);
	_room = glm::scale(_room, levelScale);

	unsigned int tex_toon;

	//static const GLubyte toon_tex_data[] =
	//{
	//	0x44, 0x00, 0x00, 0x00,
	//	0x88, 0x00, 0x00, 0x00,
	//	0xCC, 0x00, 0x00, 0x00,
	//	0xFF, 0x00, 0x00, 0x00
	//};
	//
	//glGenTextures(1, &tex_toon);
	//glBindTexture(GL_TEXTURE_1D, tex_toon);
	//glTexSubImage1D(GL_TEXTURE_1D, 0, 0, sizeof(toon_tex_data) / 4, GL_RGBA, GL_UNSIGNED_BYTE, toon_tex_data);
	//glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	int width = 0;
	int height = 0;
	int nrChannels = 0;
	unsigned char *data = stbi_load("./Assets/Textures/ramp.png", &width, &height, &nrChannels, 0);

	GLuint texture_r;
	glGenTextures(1, &texture_r);
	glBindTexture(GL_TEXTURE_1D, texture_r);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	if (data)
	{
		glTexSubImage1D(GL_TEXTURE_1D, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_1D, 0);
	stbi_image_free(data);

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(1.0f, 0.45f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		Shader basicLight("./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/Light.frag");

		basicLight.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, texture_r);

		basicLight.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
		basicLight.setVec3("viewPos", camera.Position);

		if (noLight == false || ambient == false)
		{
			basicLight.setVec3("light.ambient", 0.05f, 0.05f, 0.95f);
			basicLight.setVec3("light.diffuse", 0.2f, 0.2f, 0.2f);
			basicLight.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
			glUniform1i(glGetUniformLocation(basicLight.ID, "lightDefault"), true);
			glUniform1i(glGetUniformLocation(basicLight.ID, "rimLighting"), true);
		}

		if (noLight == true)
		{
			basicLight.setVec3("light.ambient", 0.0f, 0.0f, 0.0f);
			basicLight.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);
			basicLight.setVec3("light.specular", 0.0f, 0.0f, 0.0f);
			glUniform1i(glGetUniformLocation(basicLight.ID, "lightDefault"), true);
			glUniform1i(glGetUniformLocation(basicLight.ID, "rimLighting"), false);
		}

		if (ambient == true)
		{
			basicLight.setVec3("light.ambient", 0.6f, 0.6f, 0.6f);
			basicLight.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);
			basicLight.setVec3("light.specular", 0.0f, 0.0f, 0.0f);
			glUniform1i(glGetUniformLocation(basicLight.ID, "lightDefault"), true);
			glUniform1i(glGetUniformLocation(basicLight.ID, "rimLighting"), false);
		}

		if (specular == true)
		{
			basicLight.setVec3("light.ambient", 0.0f, 0.0f, 0.0f);
			basicLight.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
			basicLight.setVec3("light.specular", 0.8f, 0.8f, 0.8f);
			glUniform1i(glGetUniformLocation(basicLight.ID, "lightDefault"), true);
			glUniform1i(glGetUniformLocation(basicLight.ID, "rimLighting"), false);
		}

		if (spec_rim == true)
		{
			basicLight.setVec3("light.ambient", 0.0f, 0.0f, 0.0f);
			basicLight.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);
			basicLight.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
			glUniform1i(glGetUniformLocation(basicLight.ID, "lightDefault"), true);
			glUniform1i(glGetUniformLocation(basicLight.ID, "rimLighting"), true);
		}

		if (ambient_spec_rim == true)
		{
			basicLight.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
			basicLight.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);
			basicLight.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
			glUniform1i(glGetUniformLocation(basicLight.ID, "lightDefault"), true);
			glUniform1i(glGetUniformLocation(basicLight.ID, "rimLighting"), true);
		}

		if (rampShading == true)
		{
			basicLight.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
			basicLight.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);
			basicLight.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
			glUniform1i(glGetUniformLocation(basicLight.ID, "lightDefault"), false);
			glUniform1i(glGetUniformLocation(basicLight.ID, "rimLighting"), false);
			glUniform1i(glGetUniformLocation(basicLight.ID, "rampSwitch"), true);
		}

		
		
		basicLight.setFloat("material.shininess", 32.0f);
		
		basicLight.setMat4("projection_matrix", projection);
		basicLight.setMat4("view_matrix", camera.GetViewMatrix());
		
		basicLight.setMat4("model_matrix", _ufo);
		Level.Draw(basicLight);
		
		basicLight.setMat4("model_matrix", o1);
		object1.Draw(basicLight);
		
		basicLight.setMat4("model_matrix", o2);
		object2.Draw(basicLight);
		
		basicLight.setMat4("model_matrix", o3);
		object3.Draw(basicLight);
		
		basicLight.setMat4("model_matrix", o4);
		object4.Draw(basicLight);

		basicLight.setMat4("model_matrix", _room);
		room.Draw(basicLight);

		_ufo = glm::rotate(_ufo, glm::radians(5.0f + deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));

		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}
	glfwTerminate(); 
	return 0;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		o1 = glm::rotate_slow(o1, glm::radians(5.0f + deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		o2 = glm::rotate_slow(o2, glm::radians(5.0f + deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		o3 = glm::rotate_slow(o3, glm::radians(5.0f + deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		o4 = glm::rotate_slow(o4, glm::radians(5.0f + deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.Position.y += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.Position.y -= 0.1f; 

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); 
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
	//	//No Lighting
		noLight = true;
		ambient = false;
		specular = false;
		spec_rim = false;
		ambient_spec_rim = false;
		rampShading = false;
	}
	
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
	//	//Ambient Lighting Only
		noLight = false;
		ambient = true;
		specular = false;
		spec_rim = false;
		ambient_spec_rim = false;
		rampShading = false;
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		//Specular Lighting Only
		noLight = false;
		ambient = false;
		specular = true;
		spec_rim = false;
		ambient_spec_rim = false;
		rampShading = false;
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		//Specular + Rim Lighting
		noLight = false;
		ambient = false;
		specular = false;
		spec_rim = true;
		ambient_spec_rim = false;
		rampShading = false;
	}

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		//Ambient + Specular + Rim
		noLight = false;
		ambient = false;
		specular = false;
		spec_rim = false;
		ambient_spec_rim = true;
		rampShading = false;
	}

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		//TOGGLE diffuse warp/ramp
		noLight = false;
		ambient = false;
		specular = false;
		spec_rim = false;
		ambient_spec_rim = false;
		rampShading = true;
	}

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		//TOGGLE specular warp/ramp
	}

	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		//TOGGLE Color Grading Warm
	}

	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		//TOGGLE Color Grading Cool
	}

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		//TOGGLE Color Grading Custom Effect
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse == true) {
		lastX = xpos, lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos, lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}