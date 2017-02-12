#include "gl\glew.h"
#include "gl\freeglut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Shader.h"
#include "Texture.h"
#include "ArrayBuffer.h"
#include <iostream>

Shader *shader;
Texture2D *texture;
glm::mat4 M, V, P;
ArrayBuffer *vertices, *texcoords, *normals;

void Initialize() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	shader = Shader::create(
		ShaderUnit::createWithFile(GL_VERTEX_SHADER, "src/shader/phong.vs.glsl"),
		ShaderUnit::createWithFile(GL_FRAGMENT_SHADER, "src/shader/phong.fs.glsl")
	);
	shader->setUniform("uLightColor", glm::vec3(1.0, 0.957, 0.898));
	shader->setUniform("uTexture", 0);

	texture = Texture2D::createWithFile("image/fieldstone_DM.png");
	texture->setWrap(GL_REPEAT, GL_REPEAT);
	texture->setFilter(GL_LINEAR, GL_NEAREST);

	vertices = ArrayBuffer::create(GL_VERTEX_ARRAY, 1, nullptr, GL_STATIC_DRAW);
	texcoords = ArrayBuffer::create(GL_VERTEX_ARRAY, 1, nullptr, GL_STATIC_DRAW);
	normals = ArrayBuffer::create(GL_VERTEX_ARRAY, 1, nullptr, GL_STATIC_DRAW);
}

void freeAll() {
	delete shader;
	delete texture;
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	M = glm::mat4();
	V = glm::lookAt(
		glm::vec3(3.0f, 3.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0)
	);
	P = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 PV = P * V;
	glm::mat4 normalMatrix = glm::inverse(V * M);


	shader->bind();
	{
		shader->setUniform("uM", M);
		shader->setUniform("uPV", PV);
		shader->setUniform("uNormalMatrix", normalMatrix, true);

		glActiveTexture(GL_TEXTURE0);
		texture->bind();

		glEnableVertexAttribArray(0);
		vertices->bind();
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void *)0
		);

		glEnableVertexAttribArray(1);
		texcoords->bind();
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void *)0
		);

		glEnableVertexAttribArray(2);
		normals->bind();
		glVertexAttribPointer(
			2,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void *)0
		);

		//glEnable(GL_DEPTH_TEST);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//
		//glDisable(GL_DEPTH_TEST);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	Shader::bindDefault();

	glutSwapBuffers();
}

void timer(int value) {
	glutTimerFunc(1000 / 30, timer, 1);
}

int main(int argc, char **argv) {
	glutInitWindowSize(512, 512);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Shader Programming");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(1000 / 30, timer, 1);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glewInit();
	if (!glewIsSupported(
		"GL_VERSION_3_1 "
		"GL_ARB_pixel_buffer_object "
		"GL_ARB_framebuffer_object "
		"GL_ARB_copy_buffer "
	)) {
		return 1;
	}

	Initialize();
	glutMainLoop();
	freeAll();

	return 0;
}