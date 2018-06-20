#version 410 core

// Interpolated values from the vertex shaders:
in vec3 fragmentColor;

// Output:
out vec3 color;

void main(void)	{

  color = fragmentColor;

}
