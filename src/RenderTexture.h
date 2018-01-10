#pragma once

#include "Texture.h"
#include "RenderBuffer.h"
#include "FrameBuffer.h"
#include "glm\glm.hpp"

class RenderTexture : public FrameBuffer {
protected:
	Texture2D *m_colorBuffer;
	RenderBuffer *m_depthBuffer;

	RenderTexture(unsigned int width, unsigned int height) {
		m_colorBuffer = Texture2D::createEmpty(GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE);
		m_colorBuffer->setWrap(GL_CLAMP, GL_CLAMP);
		m_colorBuffer->setFilter(GL_LINEAR, GL_LINEAR);

		m_depthBuffer = RenderBuffer::create(GL_DEPTH_COMPONENT, width, height);

		attachColor(m_colorBuffer);
		attachDepth(m_depthBuffer);
		windup();
	}
public:
	virtual ~RenderTexture() { 
		delete m_colorBuffer;
		delete m_depthBuffer;
	}

	static RenderTexture* create(glm::uvec2 size) {
		return new RenderTexture(size.x, size.y);
	}

	Texture2D* getTexture() const {
		return m_colorBuffer;
	}
};