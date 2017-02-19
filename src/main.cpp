#include "gl\glew.h"
#include "gl\freeglut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Shader.h"
#include "Texture.h"
#include "PrimitiveTorus.h"
#include "ArrayBuffer.h"
#include <iostream>

Shader *shader;
Texture2D *texture;
PrimitiveTorus *torus;
glm::mat4 M, V, P;
float rotationAngle = 0.0f;

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
	texture->setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texture->generateMipmap();

	torus = PrimitiveTorus::create(1.0f, 18, 3.0f, 36);
}

void freeAll() {
	delete shader;
	delete texture;
	delete torus;
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rotationAngle += 1;
	if (rotationAngle >= 360) {
		rotationAngle -= 360;
	}

	M = glm::mat4();
	M = glm::rotate(M, glm::radians(rotationAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	V = glm::lookAt(
		glm::vec3(5.0f, 5.0f, 5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0)
	);
	P = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 VM = V * M;
	glm::mat4 inverseVM = glm::inverse(VM);

	shader->bind();
	{
		shader->setUniform("uVM", VM);
		shader->setUniform("uP", P);
		shader->setUniform("uNormalMatrix", inverseVM, true);
		shader->setUniform("uLightPosition_viewspace", glm::vec3(0.0f, 0.0f, 0.0f));

		glActiveTexture(GL_TEXTURE0);
		texture->bind();

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		torus->draw();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	}
	Shader::bindDefault();

	glutSwapBuffers();
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, 1);
}

int main(int argc, char **argv) {
	glutInitWindowSize(1024, 1024);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

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