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
	std::vector<unsigned short> m_indices;

	ArrayBuffer *m_vertexBuffer;
	ArrayBuffer *m_texcoordBuffer;
	ArrayBuffer *m_normalBuffer;
	ArrayBuffer *m_elementBuffer;

	Mesh(Primitive_Type type) {

	}
public:
	static Mesh* createPrimitive(Primitive_Type type) {
		return new Mesh(type);
	}

	~Mesh(){
		delete m_vertexBuffer, m_texcoordBuffer, m_normalBuffer, m_elementBuffer;
	}

	void draw() {
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
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, (void *)0);

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
	const std::vector<unsigned short>& getIndices() const {
		return m_indices;
	}
};