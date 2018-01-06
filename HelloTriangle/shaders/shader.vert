#version 450
#extension GL_ARB_separate_shader_objects : enable

// Color to be passed to the fragment shader for the given vertex
layout(location = 0) out vec3 fragColor;

// Position in Normalized Device Coords passed to the frag shader
out gl_PerVertex{
  vec4 gl_Position;
};

// Hard-Coded Normalized Device Coords for vertices of triangle
vec2 positions[4] = vec2[](
			   vec2(0.0, -0.5),
			   vec2(0.5, 0.5),
			   vec2(-0.5, 0.5),
			   vec2(0.0, .75)
			   );

// Color of each vertex on the triangle
vec3 colors[4] = vec3[](
			vec3(1.0, 0.0, 0.0),
			vec3(0.0, 1.0, 0.0),
			vec3(0.0, 0.0, 1.0),
			vec3(0.0, 1.0, 0.0)
			);

void main(){
  gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
  fragColor = colors[gl_VertexIndex];
}
