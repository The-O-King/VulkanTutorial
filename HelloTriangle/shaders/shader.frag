#version 450
#extension GL_ARB_separate_shader_objects : enable

// Color to be used by color blending stage
layout(location = 0) out vec4 outColor;
// Color we pass in from vertex shader
layout(location = 0) in vec3 fragColor;

void main() {
  outColor = vec4(fragColor, 1.0);
}
