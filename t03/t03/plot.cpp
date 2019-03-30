#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include "glut.h"

using namespace std;

// dimensiunea ferestrei in pixeli
#define dim 300

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
		drawCircle(vx, vy);
	}

	void displaySegment3(double x0, double y0, double xn, double yn) {
		double vx0 = -0.95 + (x0 * cellWidth);
		double vy0 = -0.95 + (y0 * cellHeight);
		double vxn = -0.95 + (xn * cellWidth);
		double vyn = -0.95 + (yn * cellHeight);
		displayRightSegment(vx0, vy0, vxn, vyn);

		if (case1(x0, y0, xn, yn)) {
			drawLineSegmentHigh(x0, y0, xn, yn);
		}
		else {
			drawLineSegmentLow(x0, y0, xn, yn);
		}
	}
	void displayRightSegment(double x0, double y0, double xn, double yn) {
		glColor3d(0.9, 0, 0);
		glLineWidth(3);
		glBegin(GL_LINES);
		glVertex2d(x0, y0);
		glVertex2d(xn, yn);
		glEnd();

		glLineWidth(1);
	}

private:
	bool case1(double x0, double y0, double xn, double yn) {
		double m = (yn - y0) / (xn - x0);
		return x0 < xn && y0 < yn && (abs(m) <= 1);
	}

	// Resources: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Algorithm_for_integer_arithmetic
	void drawLineSegmentLow(int x0, int y0, int xn, int yn) {
		int dx = xn - x0;
		int dy = yn - y0;

		int yi = 1;
		if (dy < 0) {
			yi = -1;
			dy = -dy;
		}

		int d = 2 * dy - dx;
		int dE = 2 * dy;
		int dSE = 2 * dx;
		int x = x0, y = y0;

		while (x <= xn) {
			writePixel(x, y);
			writePixel(x, y - 1);
			writePixel(x, y + 1);

			if (d > 0) {
				d -= dSE;
				y = y + yi;
			}
		
			++x;
			d += dE;
		}
	}

	void drawLineSegmentHigh(int x0, int y0, int xn, int yn) {
		int dx = xn - x0;
		int dy = yn - y0;

		int d = 2 * dy - dx;
		int dE = 2 * dy;
		int dNE = 2 * (dy - dx);
		int x = x0, y = y0;

		while (x <= xn) {
			writePixel(x, y);

			if (d <= 0) {
				d += dE;
				++x;
			}
			else {
				d += dNE;
				++x;
				++y;
			}
		}
	}

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

void Display1() {
	grid.draw();
	grid.displaySegment3(0, 0, 15, 7);
	grid.displaySegment3(0, 15, 15, 10);
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

