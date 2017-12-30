#include "gl\glew.h"
#include "gl\freeglut.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Shader.h"
#include "Texture.h"
#include "PrimitiveTorus.h"
#include "PrimitiveQuad.h"
#include "ArrayBuffer.h"
#include "Raycaster.h"
#include "Ray.h"
#include "FrameBuffer.h"
#include <iostream>
#include <memory>

static const glm::ivec2 SCREEN_SIZE(1024, 1024);

glm::ivec2 mousePosition(-1, -1);
std::shared_ptr<Shader> shader;
std::shared_ptr<Shader> billboardShader;
std::shared_ptr<Texture2D> texture;
std::shared_ptr<PrimitiveTorus> torus;
std::shared_ptr<PrimitiveQuad> quad;
std::shared_ptr<Raycaster> raycaster;
std::shared_ptr<Texture2D> colorBuffer;
std::shared_ptr<RenderBuffer> depthBuffer;
std::shared_ptr<FrameBuffer> frameBuffer;
std::shared_ptr<Shader> testShader;
glm::mat4 M, V, P;
float rotationAngle = 0.0f;
glm::vec3 cameraPosition(0.0f, 0.0f, 6.0f);

void Initialize() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	shader = std::shared_ptr<Shader>(Shader::create(
		ShaderUnit::createWithFile(GL_VERTEX_SHADER, "shader/phong.vs.glsl"),
		ShaderUnit::createWithFile(GL_FRAGMENT_SHADER, "shader/phong.fs.glsl")
	));
	shader->setUniform("uLightColor", glm::vec3(1.0, 0.957, 0.898));
	shader->setUniform("uTexture", 0);

	//billboardShader = std::shared_ptr<Shader>(Shader::create(
	//	ShaderUnit::createWithFile(GL_VERTEX_SHADER, "shader/billboard.vs.glsl"),
	//	ShaderUnit::createWithFile(GL_FRAGMENT_SHADER, "shader/billboard.fs.glsl")
	//));

	texture = std::shared_ptr<Texture2D>(Texture2D::createWithFile("image/fieldstone_DM.png"));
	texture->setWrap(GL_REPEAT, GL_REPEAT);
	texture->setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	texture->generateMipmap();

	torus = std::shared_ptr<PrimitiveTorus>(PrimitiveTorus::create(1.0f, 18, 3.0f, 36));

	quad = std::shared_ptr<PrimitiveQuad>(PrimitiveQuad::create());

	raycaster = std::shared_ptr<Raycaster>(Raycaster::create(SCREEN_SIZE, V, P));

	colorBuffer = std::shared_ptr<Texture2D>(Texture2D::createEmpty(GL_RGBA, SCREEN_SIZE.x, SCREEN_SIZE.y, GL_RGBA, GL_FLOAT
	));
	colorBuffer->setWrap(GL_CLAMP, GL_CLAMP);
	colorBuffer->setFilter(GL_LINEAR, GL_LINEAR);

	depthBuffer = std::shared_ptr<RenderBuffer>(RenderBuffer::create(GL_DEPTH_COMPONENT, SCREEN_SIZE.x, SCREEN_SIZE.y));

	frameBuffer = std::shared_ptr<FrameBuffer>(FrameBuffer::create());
	frameBuffer->attachColor(colorBuffer.get());
	frameBuffer->attachDepth(depthBuffer.get());
	frameBuffer->windup();

	testShader = std::shared_ptr<Shader>(Shader::create(
		ShaderUnit::createWithFile(GL_VERTEX_SHADER, "shader/test.vs.glsl"),
		ShaderUnit::createWithFile(GL_FRAGMENT_SHADER, "shader/test.fs.glsl")
	));
	shader->setUniform("uTexture", 0);
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
}

void display() {

	rotationAngle += 1;
	if (rotationAngle >= 360) {
		rotationAngle -= 360;
	}

	M = glm::mat4();
	M = glm::rotate(M, glm::radians(rotationAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	V = glm::lookAt(
		cameraPosition,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0)
	);
	P = glm::perspective(45.0f, 1.0f, 2.0f, 10000.0f);
	glm::mat4 VM = V * M;
	glm::mat4 inverseVM = glm::inverse(VM);
	raycaster->updateMatrix(V, P);

	frameBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	{
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
			quad->draw();
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
		}
	}

	FrameBuffer::bindDefault();
	{
		testShader->bind();
		{
			glActiveTexture(GL_TEXTURE0);
			colorBuffer->bind();

			glEnable(GL_CULL_FACE);
			quad->draw();
			glDisable(GL_CULL_FACE);
		}
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void timer(int value) {
	glutTimerFunc(1000 / 30, timer, 1);
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mousePosition = glm::ivec2(x, y);

			glm::vec3 ray = raycaster->getRay(mousePosition.x, mousePosition.y);
			printf("%f %f %f %f\n", ray.x, ray.y, ray.z, glm::length(ray));

			glm::mat4 inverseM = glm::inverse(M);
			Ray r(inverseM * glm::vec4(cameraPosition, 1.0), inverseM * glm::vec4(ray, 1.0));
			if (r.isHit(torus.get())) {
				printf("ok\n");
			}
		}
	}
}

int main(int argc, char **argv) {
	glutInitWindowSize(SCREEN_SIZE.x, SCREEN_SIZE.y);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Shader Programming");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(1000 / 30, timer, 1);
	glutMouseFunc(mouse);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glewInit();
	if (!glewIsSupported(
		"GL_VERSION_4_4 "
		"GL_ARB_pixel_buffer_object "
		"GL_ARB_framebuffer_object "
		"GL_ARB_copy_buffer "
	)) {
		return 1;
	}

	Initialize();
	glutMainLoop();

	return 0;
}