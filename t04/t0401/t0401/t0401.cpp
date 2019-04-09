#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include "glut.h"

using namespace std;

// dimensiunea ferestrei in pixeli
#define dim 300
#define PI 3.14159265358979323846

unsigned char prevKey;

struct Point {
	int x, y;

	Point() {

	}

	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

struct Edge {
	Point start, end;

	Edge() {

	}

	Edge(Point start, Point end) {
		this->start = start;
		this->end = end;
	}
};

struct Polygon {
	vector<Edge> edges;

	Polygon(vector<Edge> edges) {
		this->edges = edges;
	}
};

struct Intersection {
	double ymax;
	double xmin;
	double ratio;

	Intersection(double ymax, double xmin, double ratio) {
		this->ymax = ymax;
		this->xmin = xmin;
		this->ratio = ratio;
	}
};

struct DOM_SCAN {
	int n;
	vector<int> values;

	DOM_SCAN(int n) {
		this->n = n;
		for (int i = 0; i <= n; i++) {
			values[i] = i;
		}
	}
};

// edge table
struct ET {
	vector<vector<Intersection>*> et;
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

	void displayCircle(int x0, int y0, int R) {
		drawCircleLine(x0, y0, R);
		displayCircle4(R);
	}

	void displayFullCircle(int x0, int y0, int R) {
		drawFullCircleLine(x0, y0, R);
		displayFullCircle4(x0, y0, R);
	}

	void drawCircleLine(int x0, int y0, int R) {
		double x, y;
		double step = 0.001;

		glColor3d(0.9, 0.0, 0.0);
		glPointSize(4);
		glBegin(GL_POINTS);

		// cadran 1: 0 <= t <= PI/2
		for (double t = 0; t <= PI / 2; t += step)
		{
			x = x0 + R * cos(t);
			y = y0 + R * sin(t);

			double vx = -0.95 + x * cellWidth;
			double vy = -0.95 + y * cellHeight;
			glVertex2f(vx, vy);
		}

		glEnd();
		// reset point size
		glPointSize(1);
	}

	void drawFullCircleLine(int x0, int y0, int R) {
		double x, y;
		double step = 0.001;

		glColor3d(0.9, 0.0, 0.0);
		glPointSize(4);
		glBegin(GL_POINTS);

		// cadran 1: 0 <= t <= PI/2
		for (double t = 0; t <= 2 * PI; t += step)
		{
			x = x0 + R * cos(t);
			y = y0 + R * sin(t);

			double vx = -0.95 + x * cellWidth;
			double vy = -0.95 + y * cellHeight;
			glVertex2f(vx, vy);
		}

		glEnd();
		// reset point size
		glPointSize(1);
	}

	void displayEllipse(int x0, int y0, int a, int b) {
		drawEllipseLine(x0, y0, a, b);
		fillTheEllipse(x0, y0, a, b);
	}

	void drawEllipseLine(int x0, int y0, int a, int b) {
		double x, y;
		double step = 0.001;

		glColor3d(0.9, 0.0, 0.0);
		glPointSize(2);
		glBegin(GL_POINTS);

		for (double t = 0; t < 2 * PI; t += step) {
			x = x0 + a * cos(t);
			y = y0 + b * sin(t);

			double vx = -0.95 + x * cellWidth;
			double vy = -0.95 + y * cellHeight;
			glVertex2d(vx, vy);
		}

		glEnd();

		// reset point size
		glPointSize(1);
	}

	void displayPolygon(string filename) {
		drawPolygonLine(filename);
	}

	void drawPolygonLine(string filename) {
		vector<Point> points = readPolygonCoordinates(filename);

		glColor3d(0.9, 0.0, 0.0);
		glLineWidth(3);
		glBegin(GL_LINE_LOOP);

		for (int i = 0; i < points.size(); i++) {
			double vx = -0.95 + points[i].x * cellWidth;
			double vy = -0.95 + points[i].y * cellHeight;
			glVertex2d(vx, vy);
		}

		glEnd();

		//reset line width
		glLineWidth(1);
	}

	vector<Point> readPolygonCoordinates(string filename) {
		ifstream inputFileStream;
		inputFileStream.open(filename, ifstream::in);
		
		vector<Point> points;
		int x, y;

		int count = inputFileStream.get();
		while (count > 0) {
			inputFileStream >> x;
			inputFileStream >> y;

			Point point(x, y);
			points.push_back(point);

			--count;
		}
		inputFileStream.close();

		return points;
	}

private:
	void displayCirclePoints3(int x, int y, int thickening) {
		writePixel(y, x);

		while (thickening > 0) {
			writePixel(y + thickening, x);
			writePixel(y - thickening, x);
			--thickening;
		}
	}

	void displayCirclePointLeftRight(int x, int y, int thickening) {
		writePixel(x, y);

		while (thickening > 0) {
			writePixel(x + thickening, y);
			writePixel(x - thickening, y);
			--thickening;
		}
	}

	void displayCirclePointUpDown(int x, int y, int thickening) {
		writePixel(x, y);

		while (thickening > 0) {
			writePixel(x, y + thickening);
			writePixel(x, y - thickening);
			--thickening;
		}
	}

	void displayCircleFullPoints3(int x0, int y0, int x, int y) {
		writePixel(x, y);
		writePixel(x0 - (x - x0), y);
		writePixel(x0 - (x - x0), y0 - (y - y0));
		writePixel(x, y0 - (y - y0));

		if (x != y) {
			writePixel(y, x);
			writePixel(y, x0 - (x - x0));
			writePixel(y0 - (y - y0), x0 - (x - x0));
			writePixel(y0 - (y - y0), x);
		}
	}

	void displayCircle4(int R) {
		int x = 0;
		int y = R;
		int d = 1 - R;

		int dE = 3;
		int dSE = -2 * R + 5;
		displayCirclePoints3(x, y, 1);

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
			displayCirclePoints3(x, y, 1);
		}
	}

	void displayFullCircle4(int x0, int y0, int R) {
		int x = x0;
		int y = y0 + R;
		int d = 1 - R;

		int dE = 3;
		int dSE = -2 * R + 5;
		displayCircleFullPoints3(x0, y0, x, y);

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
			displayCircleFullPoints3(x0, y0, x, y);
		}
	}

	void fillTheEllipse(int x0, int y0, int a, int b) {
		int xi = 0, x = 0, y = b;
		double fxpyp = 0.0;
		double deltaE, deltaSE, deltaS;
		vector<Point> ssm;

		ssm.push_back(Point(x + x0, y + y0));
		ssm.push_back(Point(xi + x0, y + y0));

		// region 1
		while (a * a * (y - 0.5) > b * b * (x + 1)) {
			deltaE = b * b * (2 * x + 1);
			deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
			if (fxpyp + deltaE <= 0.0){
				// E este in interior
				fxpyp += deltaE;
				x++;
				ssm.push_back(Point(xi + x0, y + y0));
				ssm.push_back(Point(x + x0, y + y0));
			} else if (fxpyp + deltaSE <= 0.0)
			{
				// SE este in interior
				fxpyp += deltaSE;
				x++; y--;
				ssm.push_back(Point(xi + x0, y + y0));
				ssm.push_back(Point(x + x0, y + y0));
			}
		}

		// region 2
		while (y > 0){
			deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
			deltaS = a * a*(-2 * y + 1);
			if (fxpyp + deltaSE <= 0.0)
			{
				// SE este in interior
				fxpyp += deltaSE;
				x++; y--;
			} else
			{
				// S este in interior
				fxpyp += deltaS;
				y--;
			}
			ssm.push_back(Point(xi + x0, y + y0));
			ssm.push_back(Point(x + x0, y + y0));
		}

		for (int i = 0; i < ssm.size(); i += 2) {
			int xmin = ssm[i].x;
			int xmax = ssm[i + 1].x;
			int y = ssm[i].y;

			for (int x = xmin; x <= xmax; x++) {
				writePixel(x, y);
				writePixel(x0 - (x - xmin), y);
				writePixel(x, y0 -(y - y0));
				writePixel(x0 - (x - xmin), y0 -(y - y0));
			}
		}
	}

	void initializeEdgeTable(Polygon p, ET et) {
		int xm, ym, xM, yM;
		bool change;
		DOM_SCAN DOM_SCAN(100);

		for (int i = 0; i < DOM_SCAN.values.size(); i++) {
			et.et[i]->at[i] = vector<Intersection>();
		}

		for (int i = 0; i < p.edges.size(); i++) {
			Edge m(p.edges[i]);

			if (m.start.y != m.end.y) {
				ym = min(m.start.y, m.end.y);
				yM = max(m.start.y, m.end.y);
				xm = (ym == m.start.y) ? m.start.x : m.end.x;
				xM = (yM == m.start.y) ? m.start.x : m.end.x;

				double ratio = (xm - xM) / (ym - yM);
				et.et.at(ym)->push_back(Intersection(yM, xm, ratio));
			}
		}
		/*
			sortarea in ordine crescatoare conform cu
			xm a intersectiilor dreptei de scanare cu
			muchiile poligonului
		*/

		for (int i = 0; i < DOM_SCAN.values.size(); i++) {
			do {
				change = false;
				// ceata
			} while (change);
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
	grid.displayCircle(0, 0, 13);
}

void Display2() {
	GrilaCarteziana grid(30);

	grid.draw();
	grid.displayFullCircle(15, 15, 10);
}

void Display3() {
	GrilaCarteziana grid(26);

	grid.draw();
	grid.displayEllipse(13, 7, 13, 7);
}

void Display4() {
	GrilaCarteziana grid(14);

	grid.draw();
	grid.displayPolygon("poligon.txt");
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
	case '4':
		Display4();
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

