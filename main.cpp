#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <random>
#include "Shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void generate_particles(std::vector<float>& positions, std::vector<glm::vec2>& velocities, std::vector<glm::vec2>& acceleration, int count);
void forward_step(std::vector<float>& positions, std::vector<glm::vec2>& velocities, std::vector<glm::vec2>& acceleration, int count, float dt);
float generate_rand(float a, float b);
int generate_randint(int a, int b);


int main()
{
	const int particle_count = 100000;
	int window_size[] = { 500, 500 };
	float dt = (1.0f / 60.f);
	std::vector<float> positions(particle_count * 6);
	std::vector<glm::vec2> velocities(particle_count, glm::vec2());
	std::vector<glm::vec2> acceleration(particle_count);

	generate_particles(positions, velocities, acceleration, particle_count);

	//window initialisation
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_size[0], window_size[1], "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create window!\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD\n";
		return -1;
	}

	glViewport(0, 0, window_size[0], window_size[1]);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex Buffer Object for the positions of particles and their colors
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, particle_count * 6 * sizeof(float), &positions[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Shader shader = Shader("vertexShader.vert", "fragmentShader.frag", "geometryShader.geom");
	shader.use();

	//main loop
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);

		forward_step(positions, velocities, acceleration, particle_count, dt);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, particle_count * 6 * sizeof(float), &positions[0], GL_DYNAMIC_DRAW);
		glDrawArrays(GL_POINTS, 0, particle_count);

		glfwSwapBuffers(window);
		glfwPollEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	glfwTerminate();
	return 0;
}

 
void forward_step(std::vector<float>& positions, std::vector<glm::vec2>& velocities, std::vector<glm::vec2>& acceleration, int count, float dt) {
	float k = 0.4;
	float d = 0.8f;
	for (int i = 0; i < count; i++) {
		int index = 6 * i;
		velocities[i] += dt * acceleration[i];
		positions[index] += velocities[i][0] * dt;
		positions[index + 1] += velocities[i][1] * dt;
		if (positions[index + 1] < -1) {
			positions[index + 1] = -1;
			velocities[i][1] = d * glm::abs(velocities[i][1]);
		}
		if (positions[index] < -1) {
			positions[index] = -1;
			velocities[i][0] = d * glm::abs(velocities[i][0]);
		}
		if (positions[index + 1] > 1) {
			positions[index + 1] = 1;
			velocities[i][1] = d * -glm::abs(velocities[i][1]);
		}
		if (positions[index] > 1) {
			positions[index] = 1;
			velocities[i][0] = d * -glm::abs(velocities[i][0]);
		}
	}
}

float generate_rand(float a, float b) {

	std::random_device ran;
	std::default_random_engine eng{ ran() };
	std::uniform_real_distribution<float> urd(a, b);

	return urd(eng);
}

int generate_randint(int a, int b) {
	std::random_device ran;
	std::default_random_engine eng{ ran() };
	std::uniform_int_distribution<int> uid(a, b);

	return uid(eng);
}

void generate_particles(std::vector<float>& positions, std::vector<glm::vec2>& velocities, std::vector<glm::vec2>& acceleration, int count) {
	for (int i = 0; i < count; i++) {
		int index = 6 * i;
		int red = generate_randint(0, 1);

		positions[index] = generate_rand(-1, 1);
		positions[index + 1] = generate_rand(-1, 1);
		positions[index + 2] = 0.0f;
		positions[index + 3] = red;
		positions[index + 4] = 1 - red;
		positions[index + 5] = 0.0f;

		velocities[i] = glm::vec2(generate_rand(-1, 1), generate_rand(-1, 0));
		acceleration[i] = glm::vec2(0.0f, -.1f);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

