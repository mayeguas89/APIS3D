#version 450

vec2 vertex_position[3] = vec2[](vec2(-0.5, 0.5), vec2(0.0, -0.5), vec2(0.5, 0.5));
vec4 vertex_color[3] = vec4[](vec4(1.0f,0.f,0.f,1.f), vec4(0.0f,1.f,0.f,1.f), vec4(0.0f,0.f,1.f,1.f));

layout(location = 0) out vec4 vColor;

void main()
{
  gl_Position = vec4(vertex_position[gl_VertexIndex], 0.0, 1.0);
  vColor = vertex_color[gl_VertexIndex];
}