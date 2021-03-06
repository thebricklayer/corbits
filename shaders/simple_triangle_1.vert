#version 410 core

uniform mat4 MVP;

void main(void) {
  const vec4[] vertices = vec4[](vec4(-1.0, -1.0, 0.0, 1.0),
																 vec4( 1.0, -1.0, 0.0, 1.0),
																 vec4( 0.0,  1.0, 0.0, 1.0));

  gl_Position = vertices[gl_VertexID];
}
