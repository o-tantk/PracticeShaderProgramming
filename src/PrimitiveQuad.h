#pragma once

#include "Mesh.h"
#include "glm\glm.hpp"

class PrimitiveQuad : public Mesh {
protected:
	PrimitiveQuad() {
		m_vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
		m_texcoords.push_back(glm::vec2(0.0f, 0.0f));
		m_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

		m_vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
		m_texcoords.push_back(glm::vec2(1.0f, 0.0f));
		m_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

		m_vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
		m_texcoords.push_back(glm::vec2(1.0f, 1.0f));
		m_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

		m_vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
		m_texcoords.push_back(glm::vec2(0.0f, 1.0f));
		m_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

		m_indices.reserve(6);
		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);
		m_indices.push_back(0);
		m_indices.push_back(2);
		m_indices.push_back(3);

		createBuffers();
	}

public:
	static PrimitiveQuad* create() {
		return new PrimitiveQuad();
	}

	virtual ~PrimitiveQuad() { }
};