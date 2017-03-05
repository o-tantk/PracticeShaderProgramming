#pragma once

#include "gl\glew.h"
#include "GLObject.h"
#include "Texture.h"
#include "RenderBuffer.h"
#include <iostream>

class FrameBuffer : public GLObject {
protected:
	int m_numColorAttachments = 0;
	GLbitfield m_mask = GL_COLOR_BUFFER_BIT;

	FrameBuffer() {
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}
public:
	virtual ~FrameBuffer() {
		glDeleteFramebuffers(1, &m_id);
	}

	static FrameBuffer* create() {
		return new FrameBuffer();
	}

	void attachColor(Texture2D *texture) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_numColorAttachments, texture->getID(), 0);
		++m_numColorAttachments;
	}
	void attachColor(RenderBuffer *buffer) {
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_numColorAttachments, GL_RENDERBUFFER, buffer->getID());
		++m_numColorAttachments;
	}

	void attachDepth(Texture2D *texture){
		m_mask |= GL_DEPTH_BUFFER_BIT;
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getID(), 0);
	}
	void attachDepth(RenderBuffer *buffer) {
		m_mask |= GL_DEPTH_BUFFER_BIT;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer->getID());
	}

	void attachDepthStencil(Texture2D *texture) {
		m_mask |= (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, texture->getID(), 0);
	}
	void attachDepthStencil(RenderBuffer *buffer) {
		m_mask |= (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->getID());
	}
	void attachDepthStencil(RenderBufferMultisample *buffer) {
		m_mask |= (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->getID());
	}

	void windup() {
		std::vector<GLenum> drawBuffers;
		drawBuffers.reserve(m_numColorAttachments);
		for (int i = 0; i < m_numColorAttachments; ++i) {
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(m_numColorAttachments, drawBuffers.data(
		));

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "Failed to initiate framebuffer." << std::endl;
			exit(EXIT_FAILURE);
		}
		m_numColorAttachments = -1;
	}

	void blitToDefault(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLenum filter) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, m_mask, filter);
	}

	void bind() const {
		if (m_numColorAttachments != -1) {
			std::cerr << "Trying to use the framebuffer not wound up.") << std::endl;
			exit(EXIT_FAILURE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	static void bindDefault() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};