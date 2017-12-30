#version 330 core

in vec3 fPosition_viewspace;
in vec2 fTexcoord;
in vec3 fNormal_viewspace;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uLightColor;
uniform vec3 uLightPosition_viewspace;

void main() {
	vec3 albedo = texture2D(uTexture, fTexcoord).rgb;

	vec3 lightVector = uLightPosition_viewspace - fPosition_viewspace;
	float lightDistance = length(lightVector);
	vec3 L = lightVector / lightDistance;

	vec3 N = normalize(fNormal_viewspace);
	float diffuseCoeff = max(dot(N, L), 0.0);

	vec3 V = normalize(vec3(0.0, 0.0, 0.0) - fPosition_viewspace);
	vec3 R = reflect(-L, N); // reflect() gets incoming light vector.
	float specularCoeff = pow(max(dot(V, R), 0.0), 5.0);

	FragColor = vec4(
		+ 0.2 * albedo
		+ diffuseCoeff * uLightColor * albedo
		+ specularCoeff * uLightColor * albedo
	, 1.0);
}