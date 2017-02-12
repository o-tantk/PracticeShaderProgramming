#pragma once

#include "gl\glew.h"
#include "GLObject.h"
#include <iostream>

class ArrayBuffer : public GLObject {
protected:
	GLenum m_target, m_usage;
	unsigned int m_size;

	ArrayBuffer(GLenum target, unsigned int size, GLvoid *data, GLenum usage)
	: m_target(target), m_size(size), m_usage(usage) {
		glGenBuffers(1, &m_id);
		glBindBuffer(target, m_id);
		glBufferData(target, size, data, usage);
	}
public:
	virtual ~ArrayBuffer(){
		glDeleteBuffers(1, &m_id);
	}

	static ArrayBuffer* create(GLenum target, unsigned int size, GLvoid *data, GLenum usage){
		return new ArrayBuffer(target, size, data, usage);
	}

	virtual void bind() const {
		glBindBuffer(m_target, m_id);
	}

	void update(unsigned int size, GLvoid *data) const{
		if (size > m_size) {
			std::cerr << "Updating buffer failure. Data is larger than buffer size." << std::endl;
			exit(EXIT_FAILURE);
		}
		if (m_usage != GL_DYNAMIC_DRAW) {
			std::cerr << "Buffer is not for dynamic drawing." << std::endl;
			exit(EXIT_FAILURE);
		}

		glBindBuffer(m_target, m_id);
		GLvoid *bufferData = glMapBuffer(m_target, GL_WRITE_ONLY);
		memcpy(bufferData, data, size);
		glUnmapBuffer(m_target);
	}

	unsigned int getSize() const{
		return m_size;
	}
};

/*
class TFRenderBuffer : public TFBuffer {
protected:
	GLint m_internalFormat;
	unsigned int m_width, m_height;

	TFRenderBuffer(GLenum internalFormat, unsigned int width, unsigned int height)
	: m_internalFormat(internalFormat), m_width(width), m_height(height) {
		glGenRenderbuffers(1, &m_id);
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	}
public:
	virtual ~TFRenderBuffer(){
		glDeleteRenderbuffers(1, &m_id);
	}

	static TFRenderBuffer* create(GLenum internalFormat, unsigned int width, unsigned int height) {
		return static_cast<TFRenderBuffer *>((new TFRenderBuffer(internalFormat, width, height))->autorelease());
	}

	virtual void bind() const{
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);
	}

	inline GLint getInternalFormat() const { return m_internalFormat; }
	inline unsigned int getWidth() const { return m_width; }
	inline unsigned int getHeight() const { return m_height; }
};

class TFRenderBufferMultisample : public TFBuffer {
protected:
	GLint m_internalFormat;
	unsigned int m_width, m_height, m_samples;

	TFRenderBufferMultisample(GLsizei samples, GLenum internalFormat, unsigned int width, unsigned int height)
		: m_internalFormat(internalFormat), m_width(width), m_height(height) {
		glGenRenderbuffers(1, &m_id);
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
	}
public:
	virtual ~TFRenderBufferMultisample(){
		glDeleteRenderbuffers(1, &m_id);
	}

	static TFRenderBufferMultisample* create(GLsizei samples, GLenum internalFormat, unsigned int width, unsigned int height) {
		return static_cast<TFRenderBufferMultisample *>((new TFRenderBufferMultisample(samples, internalFormat, width, height))->autorelease());
	}

	virtual void bind() const{
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);
	}

	inline GLint getInternalFormat() const { return m_internalFormat; }
	inline unsigned int getWidth() const { return m_width; }
	inline unsigned int getHeight() const { return m_height; }
};

class TFFrameBuffer : public TFBuffer {
protected:
	int m_numColorAttachments;

	TFFrameBuffer()
	: m_numColorAttachments(0){
		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}
public:
	virtual ~TFFrameBuffer() {
		glDeleteFramebuffers(1, &m_id);
	}

	static TFFrameBuffer* create() {
		return static_cast<TFFrameBuffer *>((new TFFrameBuffer())->autorelease());
	}

	static TFFrameBuffer* createSingleChannel(TFTexture *color, TFRenderBuffer *depth){
		TFFrameBuffer *instance = new TFFrameBuffer();
		instance->attachColor(color);
		instance->attachDepth(depth);
		instance->autorelease();
		return instance;
	}

	inline void attachColor(TFTexture *texture) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_numColorAttachments, texture->getID(), 0);
		++m_numColorAttachments;
	}
	inline void attachColor(TFRenderBuffer *buffer){
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_numColorAttachments, GL_RENDERBUFFER, buffer->getID());
		++m_numColorAttachments;
	}

	inline void attachDepth(TFTexture *texture) const {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getID(), 0);
	}
	inline void attachDepth(TFRenderBuffer *buffer) const {
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer->getID());
	}

	inline void attachDepthStencil(TFTexture *texture) const{
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, texture->getID(), 0);
	}
	inline void attachDepthStencil(TFRenderBuffer *buffer) const{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->getID());
	}
	inline void attachDepthStencil(TFRenderBufferMultisample *buffer) const{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->getID());
	}

	void windup() {
		GLenum *drawBuffers = new GLenum[m_numColorAttachments];
		for (int i = 0; i < m_numColorAttachments; ++i){
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(m_numColorAttachments, drawBuffers);
		delete[] drawBuffers;

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			TFEXIT("Failed to initiate framebuffer.");
		}
		m_numColorAttachments = -1;
	}

	void blitToDefault(){
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, 1024, 768, 0, 0, 1024, 768, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	virtual void bind() const {
		TFASSERT(m_numColorAttachments == -1, "Trying to use the framebuffer not wound up.");

		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}
	
	static inline void bindDefault() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};
*/