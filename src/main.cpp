#include "gl\glew.h"
#include "gl\freeglut.h"
#include <iostream>

void reshape(int width, int height) {

}

void display() {

}

int main(int argc, char **argv) {
	glutInitWindowSize(640, 480);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Shader Programming");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glutMainLoop();

	return 0;
}