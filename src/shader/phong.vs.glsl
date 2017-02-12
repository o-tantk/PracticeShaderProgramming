#version 330 core

layout(location = 0) in vec3 vPosition_modelspace;
layout(location = 1) in vec2 vTexcoord;
layout(location = 2) in vec3 vNormal_modelspace;

out vec3 fPosition_worldspace;
out vec2 fTexcoord;
out vec3 fNormal_viewspace;

uniform mat4 uNormalMatrix;
uniform mat4 uM;
uniform mat4 uPV;

void main() {
	fPosition_worldspace = (uM * vec4(vPosition_modelspace, 1.0)).xyz;
	gl_Position = uPV * vec4(fPosition_worldspace, 1.0);
	fTexcoord = vTexcoord;
	fNormal_viewspace = (uNormalMatrix * vec4(vNormal_modelspace, 0.0)).xyz;
}