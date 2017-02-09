#pragma once

#include "gl\glew.h"

class GLObject {
protected:
	GLuint m_id;
public:
	virtual		~GLObject() { }
	GLuint		getID() const { return m_id; }
};