#version 450
#extension GL_ARB_separate_shader_objects : enable

// Vertex Color and Position Passed in from Vertex Buffer
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

// Color to be passed to the fragment shader for the given vertex
layout(location = 0) out vec3 fragColor;

// Position in Normalized Device Coords passed to the frag shader
out gl_PerVertex{
  vec4 gl_Position;
};

void main(){
  gl_Position = vec4(inPosition, 0.0, 1.0);
  fragColor = inColor;
}
