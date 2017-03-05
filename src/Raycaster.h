#pragma once

#include "glm\glm.hpp"

class Raycaster {
protected:
	glm::ivec2 viewport;
	glm::mat4 V, P;

	Raycaster(const glm::ivec2 &viewport, const glm::mat4 &V, const glm::mat4 &P)
	: viewport(viewport), V(V), P(P) {

	}
public:
	static Raycaster* create(const glm::ivec2 &viewport, const glm::mat4 &V, const glm::mat4 &P) {
		return new Raycaster(viewport, V, P);
	}

	void updateMatrix(const glm::mat4 V, const glm::mat4 P) {
		this->V = V;
		this->P = P;
	}

	glm::vec2 getNormalizedDeviceCoordinates(int mouseX, int mouseY) {
		float ndcX = 2.0f * (static_cast<float>(mouseX) / static_cast<float>(viewport.x)) - 1.0f;
		float ndcY = 2.0f * (static_cast<float>(mouseY) / static_cast<float>(viewport.y)) - 1.0f;
		return glm::vec2(ndcX, ndcY);
	}

	glm::vec3 getRay(int mouseX, int mouseY) {
		glm::vec2 ndc = getNormalizedDeviceCoordinates(mouseX, mouseY);

		return glm::vec3(ndc.x, ndc.y, 0);
	}
};