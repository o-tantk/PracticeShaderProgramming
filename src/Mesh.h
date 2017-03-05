#pragma once

#include "glm\glm.hpp"
#include "ArrayBuffer.h"
#include <vector>

enum Primitive_Type {
	Sphere, Donut
};

class Mesh {
protected:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_texcoords;
	std::vector<glm::vec3> m_normals;
	std::vector<unsigned int> m_indices;

	ArrayBuffer *m_vertexBuffer;
	ArrayBuffer *m_texcoordBuffer;
	ArrayBuffer *m_normalBuffer;
	ArrayBuffer *m_elementBuffer;

	void createBuffers() {
		m_vertexBuffer = ArrayBuffer::create(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
		m_texcoordBuffer = ArrayBuffer::create(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_texcoords.size(), m_texcoords.data(), GL_STATIC_DRAW);
		m_normalBuffer = ArrayBuffer::create(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_normals.size(), m_normals.data(), GL_STATIC_DRAW);
		m_elementBuffer = ArrayBuffer::create(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
	}

	Mesh() : m_vertexBuffer(nullptr), m_texcoordBuffer(nullptr), m_normalBuffer(nullptr), m_elementBuffer(nullptr) { }

public:
	virtual ~Mesh(){
		delete m_vertexBuffer, m_texcoordBuffer, m_normalBuffer, m_elementBuffer;
	}

	void draw() {
		assert(m_vertexBuffer != nullptr);

		glEnableVertexAttribArray(0);
		m_vertexBuffer->bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glEnableVertexAttribArray(1);
		m_texcoordBuffer->bind();
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glEnableVertexAttribArray(2);
		m_normalBuffer->bind();
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		m_elementBuffer->bind();
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void *)0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	const std::vector<glm::vec3>& getVertices() const {
		return m_vertices;
	}
	const std::vector<glm::vec2>& getTexcoords() const {
		return m_texcoords;
	}
	const std::vector<glm::vec3>& getNormals() const {
		return m_normals;
	}
	const std::vector<unsigned int>& getIndices() const {
		return m_indices;
	}
};