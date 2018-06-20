// Preamble:
#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <glm/glm.hpp>

// Global variable definition:

// Program global variables:
extern GLuint program; //program handle
extern GLuint vao; //vertex array object
extern GLuint vb; //vertex buffer
extern GLuint cb; //color buffer

// Colors:
extern static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
extern static const GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};

// Vertexes:
extern static const GLfloat triangle_vertex_buffer_data[];
extern static const GLfloat cube_vertex_buffer_data[]
extern static GLfloat changing_color_buffer_data[12*3*3]

// Timing Variables
extern double old_time;
extern double new_time;
extern double delta_time;

// Misc.
extern static int num_color_elements;
extern GLfloat change_rate[12*3*3];
extern bool invert_rate[12*3*3];
extern GLfloat randn;
extern GLfloat randr;
extern int randb;

// function definition:
void global_vars_init();

#endif
