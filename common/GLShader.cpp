/*
================================================================================================================
----------------------------------------------------------------------------------------------------------------
Author - Mason J. Markle
----------------------------------------------------------------------------------------------------------------
Date Created - 05/30/2018
Date Modified - 05/30/2018
----------------------------------------------------------------------------------------------------------------
Purpose:
* Load a GLSL shader into OpenGL
----------------------------------------------------------------------------------------------------------------
Inputs - file name strings: ("file1.vert", "file2.frag")
Outpus - program handle with attached shader(s)

Usage Example:
GLuint program = load_shader("shader.vert", "shader.frag");
glUseProgram(program)
----------------------------------------------------------------------------------------------------------------
Notes:
* Multiple shaders can be loaded by calling 'glUseProgram()' on different GLuints
* set 'glUseProgram()' input as '0' to disable
----------------------------------------------------------------------------------------------------------------
================================================================================================================
*/

#include "GLShader.hpp"

#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>

//read_file reads in a .vert or .frag shader file, and converts its contents into a string-format (and appends newline characters).
std::string read_file(const char *file_path) {
  std::string content;
  std::ifstream file_stream(file_path, std::ios::in);

  //Check if there is an input file:
  if(!file_stream.is_open()) {
    std::cerr << " *** Could not read file: '" << file_path << "'. File does not exist!" << std::endl;
    std::cout << " *** Exiting ..." << std::endl;
		exit(EXIT_FAILURE);
  }

  // Read in lines as strings, apped 'newline' to the end of each read line:
  std::string line = "";
  while(!file_stream.eof()) {
    std::getline(file_stream, line);
    content.append(line + "\n");
  }

  file_stream.close();
  return content;
}

GLuint load_shader(const char *vert_path, const char *frag_path) {
  // Initialize shaders:
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

  // Read-in shaders as strings:
  std::string vs_str = read_file(vert_path);
  std::string fs_str = read_file(frag_path);

  // Assign shader pointers:
  const GLchar *vs_source = (const GLchar *)vs_str.c_str();
  const GLchar *fs_source = (const GLchar *)fs_str.c_str();

  // Compile vertex shader (exit program upon failure):
  std::cout << "Compiling vertex shader..." << std::endl;
  glShaderSource(vs, 1, &vs_source, 0);
  glCompileShader(vs);
  GLint isCompiled = 0;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &isCompiled); //check compilation
  if (isCompiled == GL_FALSE) { //shader did not compile
		GLint maxLength = 0;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength); //maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vs, maxLength, &maxLength, &infoLog[0]);
		glDeleteShader(vs); //delete shader handle
		fprintf(stderr, " *** Failed to compile vertex shader! Info Log:\n");
		for (int i=0; i<maxLength; i++) { //print info log
			std::cout << infoLog[i];
		}
		std::cout << std::endl;
    std::cout << " *** Exiting ..." << std::endl;
		exit(EXIT_FAILURE);
	}
  else {
    std::cout << "    ...Vertex shader compiled sucessfully." << std::endl;
  }


  // Compile fragment shader (exit program upon failure):
  std::cout << "Compiling fragment shader..." << std::endl;
  glShaderSource(fs, 1, &fs_source, 0);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled); //check compilation
  if(isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength); //maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(fs, maxLength, &maxLength, &infoLog[0]);
    glDeleteShader(fs); //delete [both] shader handles (just in case)
    glDeleteShader(vs);
    fprintf(stderr, " *** Failed to compile fragment shader! Info Log:\n");
    for (int i=0; i<maxLength; i++) {
      std::cout << infoLog[i];
    }
    std::cout << std::endl;
    std::cout << " *** Exiting ..." << std::endl;
    exit(EXIT_FAILURE);
  }
  else {
    std::cout << "    ...Fragment shader compiled sucessfully." << std::endl;
  }

  // Create program, attach shaders, and link the program:
  GLuint prog = glCreateProgram(); //create program 'prog'
  glAttachShader(prog,vs);
  glAttachShader(prog,fs);
  std::cout << "Linking program..." << std::endl;
  GLint isLinked = 0;
  glLinkProgram(prog);
  glGetProgramiv(prog, GL_LINK_STATUS, &isLinked);
  if(isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength); //maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(prog, maxLength, &maxLength, &infoLog[0]);
    glDeleteShader(fs); //delete [both] shader handles (just in case)
    glDeleteShader(vs);
    glDeleteProgram(prog); //delete program
    fprintf(stderr, " *** Failed to link program! Info Log:\n");
    for (int i=0; i<maxLength; i++) {
      std::cout << infoLog[i];
    }
    std::cout << std::endl;
    std::cout << " *** Exiting ..." << std::endl;
    exit(EXIT_FAILURE);
  }
  else {
    std::cout << "    ...Program linked sucessfully." << std::endl;
  }

  // Cleanup:
  glDeleteShader(vs);
  glDeleteShader(fs);

  return prog;
}
