#version 410 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

uniform mat4 MVP;

// Output data (interpolated for each fragment)
out vec3 fragmentColor;

void main(void)	{

  gl_Position = MVP * vec4(vertexPosition_modelspace,1);

  fragmentColor = vertexColor;
}
