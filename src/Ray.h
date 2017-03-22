#pragma once

#include "glm\glm.hpp"
#include "Mesh.h"

class Ray {
protected:
	glm::vec3 position;
	glm::vec3 direction;

public:
	Ray(const glm::vec3 &position, const glm::vec3 &direction)
	: position(position), direction(direction) { }

	//@ Input counter-clockwise triangle.
	bool isHit(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) const {
		// Plane equation: dot(p, n) + d = 0
		// p = p0 + vt
		// t = -(dot(p0, n) + d) / dot(v, n)
		glm::vec3 v1_v0 = v1 - v0;
		glm::vec3 v2_v0 = v2 - v0;
		glm::vec3 n = glm::normalize(glm::cross(v1_v0, v2_v0));
		float t = (-glm::dot(position, n) + glm::dot(v0, n)) / glm::dot(direction, n);
		glm::vec3 p = position + t * direction;
		glm::vec3 p_v0 = p - v0;

		// x * v1_v0 + y * v2_v0 = p_v0
		// It's an overdetermined system.
		float determinant = v1_v0.x * v2_v0.y - v2_v0.x * v1_v0.y;
		float x = (p_v0.x * v2_v0.y - p_v0.y * v2_v0.x) / determinant;
		float y = (-p_v0.x * v1_v0.y + p_v0.y * v1_v0.x) / determinant;
		return x >= 0.0f && y >= 0.0f && (1 - x - y) >= 0.0f;
	}

	bool isHit(const Mesh *mesh) {
		const std::vector<glm::vec3> &vertices = mesh->getVertices();
		const std::vector<unsigned int> &indices = mesh->getIndices();

		bool result = false;
		for (int i = 0; i < indices.size(); i += 3) {
			glm::vec3 v0 = vertices[indices[i]];
			glm::vec3 v1 = vertices[indices[i + 1]];
			glm::vec3 v2 = vertices[indices[i + 2]];
			result |= isHit(v0, v1, v2);
		}
		return result;
	}
};