#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
  mat4 model;
  mat4 view;
  mat4 proj;
} ubo;

// Vertex Color and Position Passed in from Vertex Buffer
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

// Color to be passed to the fragment shader for the given vertex
layout(location = 0) out vec3 fragColor;

// Position in Normalized Device Coords passed to the frag shader
out gl_PerVertex{
  vec4 gl_Position;
};

void main(){
  gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
  fragColor = inColor;
}
