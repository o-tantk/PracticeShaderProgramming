#version 330 core

in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
	FragColor = texture2D(uTexture, fTexCoord);
}
