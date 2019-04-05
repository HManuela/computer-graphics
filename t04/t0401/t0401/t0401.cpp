#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include "glut.h"

using namespace std;

// dimensiunea ferestrei in pixeli
#define dim 300
#define PI 3.14159265358979323846

unsigned char prevKey;

struct Point {
	int x, y;
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

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

	void writePixel(int x, int y) {
		double vx = -0.95 + x * cellWidth;
		double vy = -0.95 + y * cellHeight;
		drawDisk(vx, vy);
	}

	void displayCircle(int R) {
		drawCircleLine(R, 90);
		displayCircle4(R);
	}

	// https://cboard.cprogramming.com/c-programming/57934-drawing-circle-using-glut.html
	void drawCircleLine(int R, int max) {
		double x, y;

		glColor3d(0.9, 0.0, 0.0);
		glLineWidth(5);
		glBegin(GL_LINES);

		x = R * cos(360 * PI / 180.0);
		y = R * sin(360 * PI / 180.0);

		double vx = -0.95 + x * cellWidth;
		double vy = -0.95 + y * cellHeight;

		for (int i = 0; i < max; i++)
		{
			glVertex2f(vx, vy);
			x = R * cos(i * PI / 180.0);
			y = R * sin(i * PI / 180.0);

			vx = -0.95 + x * cellWidth;
			vy = -0.95 + y * cellHeight;
			glVertex2f(vx, vy);
		}
		glEnd();

		// reset line width
		glLineWidth(1);
	}

private:
	void displayCirclePoints3(int x, int y) {
		writePixel(y, x);
		writePixel(y + 1, x);
		writePixel(y - 1, x);
	}

	void displayCircle4(int R) {
		int x = 0;
		int y = R;
		int d = 1 - R;

		int dE = 3;
		int dSE = -2 * R + 5;
		displayCirclePoints3(x, y);

		while (y > x) {
			if (d < 0) {
				d += dE;
				dE += 2;
				dSE += 2;
			}
			else {
				d += dSE;
				dE += 2;
				dSE += 4;
				--y;
			}
			x++;
			displayCirclePoints3(x, y);
		}
	}

	// Resources: https://gist.github.com/linusthe3rd/803118
	void drawDisk(double x, double y) {
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



void Display1() {
	GrilaCarteziana grid(15);

	grid.draw();
	grid.displayCircle(13);
}

void Display2() {
	GrilaCarteziana grid(26);

	grid.draw();
}

void Display3() {
	GrilaCarteziana grid(14);

	grid.draw();
}

void Init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {
	case '1':
		Display1();
		break;
	case '2':
		Display2();
		break;
	case '3':
		Display3();
		break;
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

