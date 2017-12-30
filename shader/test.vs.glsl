#version 330 core

layout(location = 0) in vec3 vQuadPosition;
layout(location = 1) in vec2 vQuadTexCoord;

out vec2 fTexCoord;

void main() {
	gl_Position = vec4(vQuadPosition, 1.0f);
	fTexCoord = vQuadTexCoord;
}
