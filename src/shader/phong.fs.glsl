#version 330 core

in vec3 fPosition_worldspace;
in vec2 fTexcoord;
in vec3 fNormal_viewspace;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uLightColor;
uniform vec3 uLightPosition_worldspace;

void main() {
	vec3 albedo = texture2D(uTexture, fTexcoord).rgb;

	vec3 L = uLightPosition_worldspace - fPosition_worldspace;
	float lightDistance = length(L);
	L = normalize(L);

	vec3 N = normalize(fNormal_viewspace);
	float diffuseCoeff = max(dot(N, L), 0.0);

	vec3 V = normalize(vec3(0.0, 0.0, 0.0) - fPosition_worldspace);
	vec3 R = reflect(-L, N); // reflect() gets incoming light vector.
	float specularCoeff = pow(max(dot(V, R), 0.0), 5.0);

	FragColor = vec4(
		+ 0.2 * albedo
		+ diffuseCoeff * uLightColor * albedo / (lightDistance * lightDistance)
		+ specularCoeff * uLightColor * albedo / (lightDistance * lightDistance)
	, 1.0);
}