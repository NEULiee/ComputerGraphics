// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
using namespace glm;


GLfloat* makeCircleVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);
GLfloat* makeConeVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat height, GLfloat radius, GLint numberOfSides);


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Computer Graphics Project", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.4f, 0.6f, 1.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");


	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

	static const GLfloat ground_vertex_buffer_data[] = {
		-10000.0f,0.0f,10000.0f,
		10000.0f,0.0f,10000.0f,
		-10000.0f,0.0f,-10000.0f,
		10000.0f,0.0f,10000.0f,
		-10000.0f,0.0f,-10000.0f,
		10000.0f,0.0f,-10000.0f,
		/*
		10000.0f,0.0f,0.0f,
	  -10000.0f,0.0f,0.0f,
	  -10000.0f,-10000.0f,0.0f,
	  10000.0f,0.0f,0.0f,
	  -10000.0f,-10000.0f,0.0f,
	  10000.0f,-10000.0f,0.0f
	  */
	};

	static const GLfloat ground_color_buffer_data[] = {
		0.18f, 0.62f, 0.15f,
		0.18f, 0.62f, 0.15f,
		0.18f, 0.62f, 0.15f,
		0.18f, 0.62f, 0.15f,
		0.18f, 0.62f, 0.15f,
		0.18f, 0.62f, 0.15f,
		/*
		0.18f, 0.62f, 0.15f,
		0.18f, 0.62f, 0.15f,
		0.18f, 0.62f, 0.15f,
		0.18f, 0.62f, 0.15f,
		0.18f, 0.62f, 0.15f,
		0.18f, 0.62f, 0.15f
		*/
	};

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,0.0f,1.0f,
	  -1.0f,0.0f, 3.0f,
	  -1.0f, 3.0f, 3.0f,

	   1.0f, 3.0f,1.0f,
	  -1.0f,0.0f,1.0f,
	  -1.0f, 3.0f,1.0f,

	   1.0f,0.0f, 3.0f,
	  -1.0f,0.0f,1.0f,
	   1.0f,0.0f,1.0f,

	   1.0f, 3.0f,1.0f,
	   1.0f,0.0f,1.0f,
	  -1.0f,0.0f,1.0f,

	  -1.0f,0.0f,1.0f,
	  -1.0f, 3.0f, 3.0f,
	  -1.0f, 3.0f,1.0f,

	   1.0f,0.0f, 3.0f,
	  -1.0f,0.0f, 3.0f,
	  -1.0f,0.0f,1.0f,

	  -1.0f, 3.0f, 3.0f,
	  -1.0f,0.0f, 3.0f,
	   1.0f,0.0f, 3.0f,

	   1.0f, 3.0f, 3.0f,
	   1.0f,0.0f,1.0f,
	   1.0f, 3.0f,1.0f,

	   1.0f,0.0f,1.0f,
	   1.0f, 3.0f, 3.0f,
	   1.0f,0.0f, 3.0f,

	   1.0f, 3.0f, 3.0f,
	   1.0f, 3.0f,1.0f,
	  -1.0f, 3.0f,1.0f,

	   1.0f, 3.0f, 3.0f,
	  -1.0f, 3.0f,1.0f,
	  -1.0f, 3.0f, 3.0f,

	   1.0f, 3.0f, 3.0f,
	  -1.0f, 3.0f, 3.0f,
	   1.0f,0.0f, 3.0f,

	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	// window
	GLfloat* window_vertex_buffer_data = makeCircleVertexData(0.0, 2, 3.01, 0.4, 36);
	static const GLfloat window_color_buffer_data[]{
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,

		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,

		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,

		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,

		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,

		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f
	}; 

	// window2
	GLfloat* window2_vertex_buffer_data = makeCircleVertexData(0, 2, 0.99, 0.4, 36);
	static const GLfloat window2_color_buffer_data[]{
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,

		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,

		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,

		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,

		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,

		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f,
		0.70f, 0.92f, 0.96f
	};

	// wing
	static const GLfloat wing_vertex_buffer_data[]{
		1.0f, 1.0f, 2.0f,
		1.7f, 0.0f, 2.0f,
		1.0f, 0.0f, 2.0f,

		-1.0f, 1.0f, 2.0f,
		-1.7f, 0.0f, 2.0f,
		-1.0f, 0.0f, 2.0f

	};

	static const GLfloat wing_color_buffer_data[]{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f

	};

	// head
	static const GLfloat* head_vertex_buffer_data = makeConeVertexData(0, 3, 2, 2, 0.9, 36);
	static const GLfloat head_color_buffer_data[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

	};

	static const GLfloat chute_vertex_buffer_data[] = {
		0.0f, 8.0f, -1.0f,
		-3.0f, 6.0f, -4.0f,
		-3.0f, 6.0f, -2.0f,

		0.0f, 8.0f, -1.0f,
		-3.0f, 6.0f, -2.0f,
		-3.0f, 6.0f, 0.0f,

		0.0f, 8.0f, -1.0f,
		-3.0f, 6.0f, 0.0f,
		-3.0f, 6.0f, 2.0f,

		0.0f, 8.0f, -1.0f,
		-3.0f, 6.0f, -4.0f,
		-1.0f, 6.0f, -4.0f,

		0.0f, 8.0f, -1.0f,
		-3.0f, 6.0f, -4.0f,
		1.0f, 6.0f, -4.0f,

		0.0f, 8.0f, -1.0f,
		-3.0f, 6.0f, -4.0f,
		3.0f, 6.0f, -4.0f,

		0.0f, 8.0f, -1.0f,
		3.0f, 6.0f, -4.0f,
		3.0f, 6.0f, -2.0f,

		0.0f, 8.0f, -1.0f,
		3.0f, 6.0f, -4.0f,
		3.0f, 6.0f, 0.0f,

		0.0f, 8.0f, -1.0f,
		3.0f, 6.0f, -4.0f,
		3.0f, 6.0f, 2.0f,

		0.0f, 8.0f, -1.0f,
		3.0f, 6.0f, 2.0f,
		1.0f, 6.0f, 2.0f,

		0.0f, 8.0f, -1.0f,
		3.0f, 6.0f, 2.0f,
		-1.0f, 6.0f, 2.0f,

		0.0f, 8.0f, -1.0f,
		3.0f, 6.0f, 2.0f,
		-3.0f, 6.0f, 2.0f,
	};

	static const GLfloat chute_color_buffer_data[] = {
		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,

		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,

		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,

		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,

		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,

		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,

		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,

		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,

		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,

		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,

		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,
		1.0f, 0.73f, 0.0f,

		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,
		1.0f, 0.37f, 0.0f,
	};

	static const GLfloat* line_vertex_buffer_data = makeConeVertexData(0, 6, -1, -5.5, 3, 12);
	static const GLfloat line_color_buffer_data[] = {
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,

		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,

		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
		0.74f, 0.74f, 0.74f,
	};

	static const GLfloat mountine_vertex_buffer_data[] = {
		10.0f, 0.0f, 0.0f,
		20.0f, 0.0f, 0.0f, 
		15.0f, 7.0f, 0.0f, 

		17.0f, 0.0f, 0.0f, 
		23.0f, 0.0f, 0.0f, 
		20.0f, 4.0f, 0.0f
	};
	static const GLfloat mountine_color_buffer_data[] = {
		0.11f, 0.55f, 0.08f,
		0.11f, 0.55f, 0.08f,
		0.11f, 0.55f, 0.08f,

		0.11f, 0.55f, 0.08f,
		0.11f, 0.55f, 0.08f,
		0.11f, 0.55f, 0.08f
	};

	GLuint groundVertexbuffer;
	glGenBuffers(1, &groundVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, groundVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertex_buffer_data), ground_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint groundColorbuffer;
	glGenBuffers(1, &groundColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, groundColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 3, ground_color_buffer_data, GL_STATIC_DRAW);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	GLuint windowVertexbuffer;
	glGenBuffers(1, &windowVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, windowVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 37 * 3 * sizeof(GLfloat), window_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint windowColorbuffer;
	glGenBuffers(1, &windowColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, windowColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(window_color_buffer_data), window_color_buffer_data, GL_STATIC_DRAW);

	GLuint window2Vertexbuffer;
	glGenBuffers(1, &window2Vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, window2Vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 37 * 3 * sizeof(GLfloat), window2_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint window2Colorbuffer;
	glGenBuffers(1, &window2Colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, window2Colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(window2_color_buffer_data), window2_color_buffer_data, GL_STATIC_DRAW);


	GLuint wingVertexbuffer;
	glGenBuffers(1, &wingVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, wingVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wing_vertex_buffer_data), wing_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint wingColorbuffer;
	glGenBuffers(1, &wingColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, wingColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wing_color_buffer_data), wing_color_buffer_data, GL_STATIC_DRAW);

	GLuint headVertexbuffer;
	glGenBuffers(1, &headVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, headVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36 * 9, head_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint headColorbuffer;
	glGenBuffers(1, &headColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, headColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36 * 9, head_color_buffer_data, GL_STATIC_DRAW);

	GLuint chuteVertexbuffer;
	glGenBuffers(1, &chuteVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, chuteVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chute_vertex_buffer_data), chute_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint chuteColorbuffer;
	glGenBuffers(1, &chuteColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, chuteColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chute_color_buffer_data), chute_color_buffer_data, GL_STATIC_DRAW);

	GLuint lineVertexbuffer;
	glGenBuffers(1, &lineVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lineVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 * 9, line_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint lineColorbuffer;
	glGenBuffers(1, &lineColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lineColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 * 9, line_color_buffer_data, GL_STATIC_DRAW);

	GLuint mountineVertexbuffer;
	glGenBuffers(1, &mountineVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mountineVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mountine_vertex_buffer_data), mountine_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint mountineColorbuffer;
	glGenBuffers(1, &mountineColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mountineColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mountine_color_buffer_data), mountine_color_buffer_data, GL_STATIC_DRAW);

	double lastTime = glfwGetTime();
	do {
		double time = glfwGetTime();
		float delTime = time - lastTime;
		lastTime = time;

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);

		glm::mat4 RotationMatrix = eulerAngleYXZ(0.0f, getRotation(), 0.0f);
		glm::mat4 TranslationMatrix = translate(mat4(), glm::vec3(0, getHeight(), 0));

		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glm::mat4 MVPRocket = ProjectionMatrix * ViewMatrix * ModelMatrix * TranslationMatrix * RotationMatrix;
		glm::mat4 MVPChute = ProjectionMatrix * ViewMatrix * ModelMatrix * TranslationMatrix;

		//ground
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, groundVertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, groundColorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 2*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// mountine
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mountineVertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, mountineColorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 1 * 3 * 2); // 2*3 indices starting at 0 -> 12 triangles
		glDisable(GL_CULL_FACE);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// rocket
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPRocket[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// window

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, windowVertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, windowColorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLE_FAN, 0, 37 * 3); // 2*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// window2

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, window2Vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, window2Colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLE_FAN, 0, 37 * 3); // 2*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// wing

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, wingVertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, wingColorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 1 * 3 * 2); // 2*3 indices starting at 0 -> 12 triangles
		glDisable(GL_CULL_FACE);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// head

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, headVertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, headColorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 324); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);


		if (getChute() == true) {
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPChute[0][0]);
			// chute
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, chuteVertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, chuteColorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// line

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, lineVertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, lineColorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_LINES, 0, 12 * 9); // 12*3 indices starting at 0 -> 12 triangles
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		}

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();



	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &groundVertexbuffer);
	glDeleteBuffers(1, &groundColorbuffer);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);

	glDeleteBuffers(1, &windowVertexbuffer);
	glDeleteBuffers(1, &windowColorbuffer);

	glDeleteBuffers(1, &wingVertexbuffer);
	glDeleteBuffers(1, &wingColorbuffer);

	glDeleteBuffers(1, &headVertexbuffer);
	glDeleteBuffers(1, &headColorbuffer);

	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

GLfloat* makeCircleVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
	GLint numberOfVertices = numberOfSides + 1; // vertex 갯수

	GLfloat doublePi = 2.0f * 3.141592f; // 2pi

	// 동적할당 x, y, z
	GLfloat* circleVerticesX = new GLfloat[numberOfVertices]; // 넘겨준 값 side = 36 , numberOfVertices = 37
	GLfloat* circleVerticesY = new GLfloat[numberOfVertices];
	GLfloat* circleVerticesZ = new GLfloat[numberOfVertices];

	for (int i = 0; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i * doublePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * doublePi / numberOfSides));
		circleVerticesZ[i] = z;
	}
	// 배열 하나로 합쳐줌
	GLfloat* allCircleVertices = new GLfloat[numberOfVertices * 3];	// 37 * 3 (x, y, z)

	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	return allCircleVertices;
}

GLfloat* makeConeVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat height, GLfloat radius, GLint numberOfSides) {
	GLint numberOfVertices = numberOfSides + 1;
	GLfloat doublePi = 2.0f * 3.141592f;
	GLfloat* circleVerticesX = new GLfloat[numberOfVertices * 3.0];
	GLfloat* circleVerticesY = new GLfloat[numberOfVertices * 3.0];
	GLfloat* circleVerticesZ = new GLfloat[numberOfVertices * 3.0];

	for (int i = 0; i < numberOfVertices; i++) {
		circleVerticesX[(i * 3) + 0] = x + (radius * cos(i * doublePi / numberOfSides));
		circleVerticesY[(i * 3) + 0] = y;
		circleVerticesZ[(i * 3) + 0] = z + (radius * sin(i * doublePi / numberOfSides));
		circleVerticesX[(i * 3) + 1] = x;
		circleVerticesY[(i * 3) + 1] = y + height;
		circleVerticesZ[(i * 3) + 1] = z;
		circleVerticesX[(i * 3) + 2] = x + (radius * cos((i + 1) * doublePi / numberOfSides));
		circleVerticesY[(i * 3) + 2] = y;
		circleVerticesZ[(i * 3) + 2] = z + (radius * sin((i + 1) * doublePi / numberOfSides));
	}
	GLfloat* allCylinderVertices = new GLfloat[numberOfVertices * 9.0];

	for (int i = 0; i < numberOfVertices * 3; i++) {
		allCylinderVertices[(i * 3) + 0] = circleVerticesX[i];
		allCylinderVertices[(i * 3) + 1] = circleVerticesY[i];
		allCylinderVertices[(i * 3) + 2] = circleVerticesZ[i];
	}
	return allCylinderVertices;
}
