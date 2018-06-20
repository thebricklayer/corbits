/*
================================================================================================================
----------------------------------------------------------------------------------------------------------------
Author - Mason J. Markle
----------------------------------------------------------------------------------------------------------------
Date Created - 05/30/2018
Date Modified - 05/30/2018
----------------------------------------------------------------------------------------------------------------
Purpose:
* OpenGL Main Program
	- Sets OpenGL parameters
	- Initializes editing enviornment
----------------------------------------------------------------------------------------------------------------
Inputs - N/A
Outpus - N/A
----------------------------------------------------------------------------------------------------------------
Notes:
* Current configuration: Triangle
* OpenGL Vertex Protocol: <X,Y,Z,W> = <Right,Up,Back,Type>
	- Follows right hand rule for cartesian parameters (XYZ)
	- W = 1: Vector is a position in space, W = 0: Vector is a direction.
----------------------------------------------------------------------------------------------------------------
================================================================================================================
*/

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp> // variable display (debugging)
using namespace glm; //allows for 'vec3' instead of 'glm::vec3', for example

#include "GLShader.hpp"
#include "createMVPmat.hpp"

// Define global variables:
GLuint program; //program handle
GLuint vao; //vertex array object
GLuint vb; //vertex buffer
GLuint cb; //color buffer

// timer variables:
double old_time;
double new_time;
double delta_time;

// Define static variables:
static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f}; //black background color
static const GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f}; //white background color

// Generate 3 vertices from an array of three vectors:
static const GLfloat triangle_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 0.0f,  1.0f, 0.0f,
 };

// Generate 36 vertices for a cube, with two triangles per face:
static const GLfloat cube_vertex_buffer_data[] = {
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
 };

// changing color buffer:
static GLfloat changing_color_buffer_data[12*3*3] = {}; //empty color array
static int num_color_elements = sizeof(changing_color_buffer_data)/sizeof(changing_color_buffer_data[0]);
GLfloat change_rate[12*3*3] = {};
bool invert_rate[12*3*3] = {};
GLfloat randn;
GLfloat randr;
int randb;

// Callback registered with GLFW for errors (prints out error to STDERR stream):
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

// Callback registered with GLFW for keyboard handling
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		std::cout << "'ESC' key pressed - PROGRAM EXIT" << std::endl;
	}
	// if (key == (
}


// // Some error checking lines (debugging):
// std::cout << "1?" << std::endl; // error target
// std::cout << to_string(matrix) << std::endl; //show mat4 matrix contents
// printf("Number of color elements: %i \n", num_color_elements);

int main() {

	// populate color arrays randomly:
	srand (static_cast <unsigned> (time(0))); // seed random number generator
	for (int n=0; n<num_color_elements; n++) {
		randn = static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX/(1-0)));
		randr = static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX/(0.02-.001)));
		randb = rand() % 2;
		changing_color_buffer_data[n] = randn;
		change_rate[n] = randr;
		if (randb == 0) {
			invert_rate[n] = true;
			change_rate[n] = -change_rate[n];
		}
			else invert_rate[n] = false;
	}

	// Initialize GLFW, and if it fails to do so, print out the reason to STDERR:
	if(!glfwInit()) {
		fprintf(stderr, "Failed to Initialize GLFW.");
		exit(EXIT_FAILURE);
	}

	// Set the error callback:
	glfwSetErrorCallback(error_callback);

	// Set up OpenGL options:
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL version 4.1
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // OPENGL forward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // newest core profile
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // allow the window to be re-sized

	// Generate a window:
	int width = 1100;
	int height = 700;
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);

	// If there is a window failure, print the error and clean up GLFW. Exit program:
	if(!window) {
		fprintf(stderr, "Failed to create GLFW window.");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Use the created window as the current context (all additions are added to this window):
	glfwMakeContextCurrent(window);

	// Verson (only available after context decleration):
	printf("OpenGL version supported by this platform: (%s): \n", glGetString(GL_VERSION));

	// Make sure all extensions will be exposed in GLEW and initialize GLEW:
	glewExperimental = GL_TRUE;
	glewInit();

	// Set the keyboard callback
	glfwSetKeyCallback(window, key_callback);

	// Generate vertex array object for the program:
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Vertex buffer configuration:
	glGenBuffers(1, &vb); //generate a vertex buffer:
	glBindBuffer(GL_ARRAY_BUFFER, vb); //bind the vertex buffer?
	// glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex_buffer_data), triangle_vertex_buffer_data, GL_STATIC_DRAW); //give verticies to OpenGL
	// glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_buffer_data), cube_vertex_buffer_data, GL_STATIC_DRAW); //give verticies to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_buffer_data), cube_vertex_buffer_data, GL_STATIC_DRAW); //give verticies to OpenGL

	// Color buffer configuration:
	glGenBuffers(1, &cb); //generate a color buffer:
	glBindBuffer(GL_ARRAY_BUFFER, cb); //bind the color buffer?
	// glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex_buffer_data), triangle_vertex_buffer_data, GL_STATIC_DRAW); //give verticies to OpenGL
	// glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color_buffer_data), cube_color_buffer_data, GL_STATIC_DRAW); //give colors to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(changing_color_buffer_data), changing_color_buffer_data, GL_STATIC_DRAW); //give colors to OpenGL

	// Add vertex depth:
	glEnable(GL_DEPTH_TEST); //enable vertex depth test
	glDepthFunc(GL_LESS); //only accept closer fragments


	// // Declare variables for MVP matrix:
	// float FOV = 45.0;
	// float near = 0.1;
	// float far = 100.0;
	// vec3 cam_pos = vec3(4,3,-3);
	// vec3 cam_aim = vec3(0,0,0); // camera looks at the origin
	// vec3 cam_up = vec3(0,1,0); // head is up
	// float model_matrix = 1.0; // identity matrix - model is at the origin.
	//
	// // Generate MVP matrix:
	// mat4 mvp = generate_MVP(FOV, width, height, near, far, cam_pos, cam_aim, cam_up, model_matrix);

	// Declaring initial MVP params:
	float near = 0.1;
	float far = 100000.0;
	vec3 direction = vec3(0,0,0);
	vec3 pos = vec3(5,5,10); //camera position variable for MVP matrix
	vec3 aim = pos+direction;
	vec3 up = vec3(0,1,0); //initially head is up
	float model_matrix = 1.0; //identity matrix - model is at the origin
	float horizontal_angle = 3.14; //horizonatal angle towards: -z
	float vertical_angle = 0.0; //vertical angle towards: horizion
	vec3 right = vec3(sin(horizontal_angle - 3.14159f/2.0f),
										0,
										cos(horizontal_angle - 3.14159f/2.0f)
								 );
	float FOV = 45.0; //initial feild of view
	float speed = 10.0; //3[units/second]
	float mouse_speed = 0.05;
	double xpos, ypos = 0; //mouse positions
	glfwGetWindowSize(window,&width,&height);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	int wstate;
	int astate;
	int sstate;
	int dstate;
	int spacestate;
	int shiftstate;
	printf("Width = %d\n",width);
	printf("Height = %d\n",height);


	// Load and manipulate shaders with GLShader.cpp.
	// GLuint program = load_shader("shaders/simple_triangle.vert", "shaders/simple_triangle.frag");
	GLuint program = load_shader("shaders/color_cube.vert", "shaders/color_cube.frag");

	// get a handle for the "MVP" uniform:
	GLuint MVP_ID = glGetUniformLocation(program, "MVP");

	// Specify that we are using the prescribed program:
	glUseProgram(program);

	// set timer:
	glfwSetTime(0.0);
	old_time = glfwGetTime();

	glfwSetCursorPos(window,width/2,height/2); //reset mouse position for next frame

	// render loop (continues until interrupt [ESC]):
	while(!glfwWindowShouldClose(window)) {

		glClearBufferfv(GL_COLOR, 0, black); //clear entire buffer with color (sets background to black)
		glClear(GL_DEPTH_BUFFER_BIT); //clear Z-Buffer

		// frame times:
		new_time = glfwGetTime();
		delta_time = new_time - old_time;
		old_time = new_time;


		glfwGetCursorPos(window,&xpos,&ypos); //get mouse position
		glfwSetCursorPos(window,width/2,height/2); //reset mouse position for next frame
		// printf("Cursor x = %f\n",xpos);
		// printf("Cursor y = %f\n",ypos);
		horizontal_angle += mouse_speed*(float)delta_time*float(width/2 - xpos);
		vertical_angle += mouse_speed*(float)delta_time*float(height/2 - ypos);
		// printf("Horizontal Angle = %f\n",horizontal_angle);
		// printf("Vertical Angle   = %f\n",vertical_angle);
		direction = vec3(cos(vertical_angle)*sin(horizontal_angle),
		 					 sin(vertical_angle),
							 cos(vertical_angle)*cos(horizontal_angle)
						  );
		right = vec3(sin(horizontal_angle - 3.1415926535f/2.0f),
								 0,
								 cos(horizontal_angle - 3.1415926535f/2.0f)
							 );
		up = cross(right,direction);

		wstate = glfwGetKey(window, GLFW_KEY_W);
		if (wstate == GLFW_PRESS)
			pos += aim*(float)delta_time*speed;

		astate = glfwGetKey(window, GLFW_KEY_A);
		if (astate == GLFW_PRESS)
			pos -= right*(float)delta_time*speed;

		sstate = glfwGetKey(window, GLFW_KEY_S);
		if (sstate == GLFW_PRESS)
			pos -= aim*(float)delta_time*speed;

		dstate = glfwGetKey(window, GLFW_KEY_D);
		if (dstate == GLFW_PRESS)
			pos += right*(float)delta_time*speed;

		spacestate = glfwGetKey(window, GLFW_KEY_SPACE);
		if (spacestate == GLFW_PRESS)
			pos += up*(float)delta_time*speed;

		shiftstate = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
		if (shiftstate == GLFW_PRESS)
			pos -= up*(float)delta_time*speed;

		aim = pos+direction;

		// Generate MVP matrix:
		mat4 mvp = generate_MVP(FOV, width, height, near, far, pos, aim, up, model_matrix);

		//set transformation to currently bound shader.
		glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &mvp[0][0]);

		// Attribute buffer for verticies:
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glVertexAttribPointer(
			0,				//attrubute 0: vertex shader
			3,				//size
			GL_FLOAT, //type
			GL_FALSE, //normalized?
			0,				//stride
			(void*)0  //array buffer offset
		);

		// Attribute buffer for colors:
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cb);
		glVertexAttribPointer(
			1,				// attrubute 1: color shader
			3,				//size
			GL_FLOAT, //type
			GL_FALSE, //normalized?
			0,				//stride
			(void*)0  //array buffer offset
		);

		// glDrawArrays(GL_TRIANGLES, 0, 3); //draw triangle
		glDrawArrays(GL_TRIANGLES, 0, 12*3); //draw cube: 12 triangles, or 6 squares
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);


		for (int i=0; i<num_color_elements; i++) {

			if ((changing_color_buffer_data[i] > 1.00000000000f) && (invert_rate[i] == false)) {
				changing_color_buffer_data[i] = 1.0f;
				randr = static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX/(0.02-.001)));
				change_rate[i] = -randr;
				invert_rate[i] = true;
			}
			if ((changing_color_buffer_data[i] < 0.00000000000f) && (invert_rate[i] == true)) {
				changing_color_buffer_data[i] = 0.0f;
				randr = static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX/(0.02-.001)));
				change_rate[i] = randr;
				invert_rate[i] = false;
			}

			changing_color_buffer_data[i] = changing_color_buffer_data[i] + change_rate[i];
		}

		glBindBuffer(GL_ARRAY_BUFFER, cb); //bind the color buffer?
		glBufferData(GL_ARRAY_BUFFER, sizeof(changing_color_buffer_data), changing_color_buffer_data, GL_STATIC_DRAW); //give colors to OpenGL


		// Swap buffers so that the drawing will appear on the screen:
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up the program when the window is closed:
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
}
