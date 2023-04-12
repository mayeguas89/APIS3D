#version 450

layout(binding = 0) uniform UniformBufferObject {
  mat4 model;
  mat4 view;
  mat4 proj;
} ubo;

layout(location = 0) in vec4 vPos;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexture;

layout(location = 0) out vec4 fColor;
layout(location = 1) out vec2 fTexture;

void main()
{
  gl_Position = ubo.proj * ubo.view * ubo.model * vPos;
  fColor = vColor;
  fTexture = vTexture;
}