#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 fColor;
layout(location = 1) in vec2 fTexture;
layout(location = 0) out vec4 outColor;

void main()
{
  // outColor = vec4(0.8f, 0.5f, 0.3f, 1.f);
  outColor = texture(texSampler, fTexture);
}