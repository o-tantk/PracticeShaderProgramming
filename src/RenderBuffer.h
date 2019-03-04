#pragma once

#include "gl\glew.h"
#include "GLObject.h"

class RenderBuffer : public GLObject
{
protected:
    GLint m_internalFormat;
    unsigned int m_width, m_height;

    RenderBuffer()
    {
        glGenRenderbuffers(1, &m_id);
        glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    }

    RenderBuffer(GLenum internalFormat, unsigned int width, unsigned int height)
        : m_internalFormat(internalFormat), m_width(width), m_height(height)
    {
        glGenRenderbuffers(1, &m_id);
        glBindRenderbuffer(GL_RENDERBUFFER, m_id);
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    }

public:
    virtual ~RenderBuffer()
    {
        glDeleteRenderbuffers(1, &m_id);
    }

    static RenderBuffer* create(GLenum internalFormat, unsigned int width, unsigned int height)
    {
        return new RenderBuffer(internalFormat, width, height);
    }

    void bind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    }

    GLint getInternalFormat() const { return m_internalFormat; }
    unsigned int getWidth() const { return m_width; }
    unsigned int getHeight() const { return m_height; }
};

class RenderBufferMultisample : public RenderBuffer
{
protected:
    unsigned int m_samples;

    RenderBufferMultisample(GLsizei samples, GLenum internalFormat, unsigned int width, unsigned int height)
        : m_samples(samples)
    {
        m_internalFormat = internalFormat;
        m_width = width;
        m_height = height;
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
    }

public:
    virtual ~RenderBufferMultisample() { }

    static RenderBufferMultisample* create(GLsizei samples, GLenum internalFormat, unsigned int width, unsigned int height)
    {
        return new RenderBufferMultisample(samples, internalFormat, width, height);
    }

    unsigned int getSamples() const { return m_samples; }
};
