#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "glut.h"
// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

class GrilaCarteziana {
public:
	GrilaCarteziana(int n) {
		this->n = n;
	}

	void draw() {
		int width = glutGet(GLUT_WINDOW_WIDTH);
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		
		int length = fminl(width, height);
		cellHeight = 1.9 / n * (1.0 * length / height);
		cellWidth = 1.9 / n * (1.0 * length / width);

		for (int i = 0; i <= n; ++i) {
			drawLine(-0.95 + (i * cellHeight), -0.95, -0.95 + n * cellWidth);
		}

		for (int j = 0; j <= n; ++j) {
			drawColumn(-0.95 + (j * cellWidth), -0.95, -0.95 + n * cellHeight);

		}
	}

	void writePixel(int line, int column) {
		double x = -0.95 + column * cellWidth;
		double y = -0.95 + line * cellHeight;
		drawCircle(x, y);
	}
private:

	// Resources: https://gist.github.com/linusthe3rd/803118
	void drawCircle(double x, double y) {
		double PI = 3.14159265358979323846;

		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.0, 0.0, 0.0);
		glVertex2f(x, y); // center of circle
		int numberOfTrianglesToDraw = 200;
		for (int i = 0; i <= numberOfTrianglesToDraw; i++) {
			double vx = x + (cellWidth / 3 * cos(i * 2 * PI / numberOfTrianglesToDraw));
			double vy = y + (cellHeight / 3 * sin(i * 2 * PI / numberOfTrianglesToDraw));
			glVertex2f(vx, vy);
		}
		glEnd();
	}

	void drawLine(double y, double min, double max) {
		glColor3d(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex2d(min, y);
		glVertex2d(max, y);
		glEnd();
	}

	void drawColumn(double x, double min, double max) {
		glColor3d(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex2d(x, min);
		glVertex2d(x, max);
		glEnd();
	}

	int n;

	double cellHeight;
	double cellWidth;
};

GrilaCarteziana grid(15);

void Display1()
{
	double ymin = 0.0;
	double ymax = 0.9;
	double xmin = 0.0;
	double xmax = 0.9;
	double step = 0.06;

	glColor3f(0.0, 0.0, 0.0); // negru
	glBegin(GL_LINES);
		glVertex2i(0, 0); 
		glVertex2i(1, 1);
	glEnd();
}

void DisplayGrid() {
	grid.draw();
	grid.writePixel(2, 2);
	grid.writePixel(3, 3);
	grid.writePixel(4, 7);
}

void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glLineWidth(1);
	//   glPointSize(3);
	glPolygonMode(GL_FRONT, GL_LINE);
}
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {
	case '1':
		Display1();
		break;
	case '2':
		DisplayGrid();
	default:
		break;
	}

	glFlush();
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}

