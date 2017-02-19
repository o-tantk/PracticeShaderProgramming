#pragma once

#include "Mesh.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <vector>

static const int C1_NUM_OF_PIECES = 18, C2_NUM_OF_PIECES = 36;
static const float C1_DEGREE = 20.0f, C2_DEGREE = 10.0f;

class PrimitiveTorus : public Mesh {
private:
	int getIndex(int innerIndex, int innerNumSlice, int outerIndex, int outerNumSlice) {
		while (innerIndex > innerNumSlice) {
			innerIndex -= innerNumSlice;
		}
		while (innerIndex < 0) {
			innerIndex += innerNumSlice;
		}
		while (outerIndex > outerNumSlice) {
			outerIndex -= outerNumSlice;
		}
		while (outerIndex < 0) {
			outerIndex += outerNumSlice;
		}
		
		return outerIndex * (innerNumSlice + 1) + innerIndex;
	}

protected:
	PrimitiveTorus(float innerRadius, int innerNumSlice, float outerRadius, int outerNumSlice) {
		float innerTheta = glm::radians(360.0f / static_cast<float>(innerNumSlice));
		float outerTheta = glm::radians(360.0f / static_cast<float>(outerNumSlice));
		glm::mat4 inner_T = glm::translate(glm::mat4(), glm::vec3(innerRadius, 0.0f, 0.0f));
		glm::mat4 outer_T = glm::translate(glm::mat4(), glm::vec3(outerRadius, 0.0f, 0.0f));

		m_vertices.reserve((innerNumSlice + 1) * (outerNumSlice + 1));
		m_texcoords.reserve((innerNumSlice + 1) * (outerNumSlice + 1));
		m_normals.reserve((innerNumSlice + 1) * (outerNumSlice + 1));
		for (int outerIndex = 0; outerIndex <= outerNumSlice; ++outerIndex) {
			glm::mat4 outer_R = glm::rotate(glm::mat4(), outerTheta * outerIndex, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec3 innerCenter = outer_R * outer_T * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			for (int innerIndex = 0; innerIndex <= innerNumSlice; ++innerIndex) {
				glm::mat4 inner_R = glm::rotate(glm::mat4(), innerTheta * innerIndex, glm::vec3(0.0f, 0.0f, 1.0f));
				glm::vec3 vertex = outer_R * outer_T * inner_R * inner_T * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				m_vertices.push_back(vertex);

				float texcoordX = static_cast<float>(outerIndex) / static_cast<float>(outerNumSlice);
				float texcoordY = static_cast<float>(innerIndex) / static_cast<float>(innerNumSlice);
				m_texcoords.push_back(glm::vec2(texcoordX, texcoordY));

				glm::vec3 normal = glm::normalize(vertex - innerCenter);
				m_normals.push_back(normal);
			}
		}

		m_indices.reserve(6 * innerNumSlice * outerNumSlice);
		for (int outerIndex = 0; outerIndex < outerNumSlice; ++outerIndex) {
			for (int innerIndex = 0; innerIndex < innerNumSlice; ++innerIndex) {
				int centerIndex = getIndex(innerIndex, innerNumSlice, outerIndex, outerNumSlice);
				int rightIndex = getIndex(innerIndex, innerNumSlice, outerIndex + 1, outerNumSlice);
				int topIndex = getIndex(innerIndex + 1, innerNumSlice, outerIndex, outerNumSlice);
				int topRightIndex = getIndex(innerIndex + 1, innerNumSlice, outerIndex + 1, outerNumSlice);

				m_indices.push_back(centerIndex);
				m_indices.push_back(topRightIndex);
				m_indices.push_back(topIndex);
				m_indices.push_back(centerIndex);
				m_indices.push_back(rightIndex);
				m_indices.push_back(topRightIndex);
			}
		}

		createBuffers();
	}

public:
	static PrimitiveTorus* create(float innerRadius, int innerNumSlice, float outerRadius, int outerNumSlice) {
		return new PrimitiveTorus(innerRadius, innerNumSlice, outerRadius, outerNumSlice);
	}

	virtual ~PrimitiveTorus() { }
};
