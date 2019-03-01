
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include "glut.h"

using namespace std;

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

// functii ajutatoare
void renderString(float x, float y, void* font, const char* string, float r, float g, float b){
	// stabilim culoarea fontului
	glColor3f(r, g, b);

	// stabilim pozitia in fereastra de afisare
	glRasterPos2f(x, y);

	for (const char* c = string; *c != '\0'; ++c){
		glutBitmapCharacter(font, *c);
	}
}

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1() {
	double xmax, ymax, xmin, ymin;
	double a = 1, b = 2;
	double pi = 4 * atan(1.0);
	double ratia = 0.05;
	double t;

	// calculul valorilor maxime/minime ptr. x si y
	// aceste valori vor fi folosite ulterior la scalare
	xmax = a - b - 1;
	xmin = a + b + 1;
	ymax = ymin = 0;
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = a + b * cos(t);
		xmax = (xmax < x1) ? x1 : xmax;
		xmin = (xmin > x1) ? x1 : xmin;

		x2 = a - b * cos(t);
		xmax = (xmax < x2) ? x2 : xmax;
		xmin = (xmin > x2) ? x2 : xmin;

		y1 = a * tan(t) + b * sin(t);
		ymax = (ymax < y1) ? y1 : ymax;
		ymin = (ymin > y1) ? y1 : ymin;

		y2 = a * tan(t) - b * sin(t);
		ymax = (ymax < y2) ? y2 : ymax;
		ymin = (ymin > y2) ? y2 : ymin;
	}

	xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
	ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x2, y2);
	}
	glEnd();
}

// graficul functiei 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
	double pi = 4 * atan(1.0);
	double xmax = 8 * pi;
	double ymax = exp(1.1);
	double ratia = 0.05;

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double x = 0; x < xmax; x += ratia) {
		double x1, y1;
		x1 = x / xmax;
		y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();
}


// https://brainly.ro/tema/1174971
//d(x) = distanta de la x la cel mai apropiat intreg
double d(double x) {
	double distance;

	if (x - floor(x) > 0.5) {
		distance = ceil(x);
	}
	else {
		distance = floor(x);
	}

	return abs(distance - x);
}

/*
f(x) = {
		1, x == 0
		d(x) / x, x > 0
		}
d(x) = distanta de la x la cel mai apropiat intreg
*/
void Display3() {
	double ratio = 0.05;
	double xmin = 0.0;
	double xmax = 100.0;
	double ymax = 1.0;
	double distanceFromRight = 10.0;
	double y;

	glColor3f(1.0, 0.1, 0.1); // culoare rosie
	glBegin(GL_LINE_STRIP);
	for (double x = xmin + ratio; x <= xmax - distanceFromRight; x += ratio)
	{
		y = (d(x) / x);
		glVertex2f(x / xmax, (y - ratio) / ymax);
	}
	glEnd();
}

/* melcul lui Pascal
x = 2 * (a * cos(t) + b) * cos(t)
y = 2 * (a * cos(t) + b) * sin(t)
t - (-pi, pi)
*/
void Display4() {
	double ratio = 0.001;
	double a = 0.3;
	double b = 0.2;
	double pi = 4 * atan(1.0);
	double tmin = -2 * pi - pi;
	double tmax = 2 * pi + pi;
	double distanceFromRight = 0.15;
	double x, y;

	glColor3f(1.0, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double t = -pi + ratio; t < pi; t += ratio) {
		x = 2 * (a * cos(t) + b) * cos(t);
		y = 2 * (a * cos(t) + b) * sin(t);

		glVertex2f(x - distanceFromRight, y);
	}
	glEnd();

	char message[50];

	sprintf_s(message, "Melcul lui Pascal");
	renderString(-0.9f, 0.8f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);

	sprintf_s(message, "a = %.1lf", a);
	renderString(-0.9f, 0.7f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);

	sprintf_s(message, "b = %.1lf", b);
	renderString(-0.9f, 0.6f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);
}

/* trisectoarea lui Longchamps

*/
void Display5() {
	double ratio = 0.005;
	double a = 0.2;
	double pi = 4 * atan(1.0);
	double tmin = -pi / 2;
	double tmax = -pi / 5.35;
	double x, y;
	int count = 1;

	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.0, 0.0); // negru
	for (double t = tmin + ratio; t < tmax; t += ratio) {
		if ((t != pi / 6) && (t != -pi / 6)) {
			x = a / (4 * cos(t) * cos(t) - 3);
			y = (a * tan(t)) / (4 * cos(t) * cos(t) - 3);

			glVertex2f(x, y);
		}
	}
	glEnd();

	// desenare contur stanga
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.0, 0.0); // negru
	for (double start = y; start <= 1.0; start += ratio) {
		y = start;
		glVertex2f(x, y);
	}
	glEnd();

	// am ajuns cu x, y in colt stanga sus
	// desenare triunghiuri interioare
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);

	for (double t = tmin + ratio; t < tmax; t += ratio) {
		x = a / (4 * cos(t) * cos(t) - 3);
		y = (a * tan(t)) / (4 * cos(t) * cos(t) - 3);

		if (count % 3 == 0) {
			glColor3f(1.0, 0.1, 0.1); // rosu
			glVertex2f(x, y);
		}
		else {
			glColor3f(1.0, 1.0, 1.0); // alb
			glVertex2f(x, y);
		}
		++count;
	}
	glEnd();

	char message[50];

	sprintf_s(message, "Trisectoarea lui Longchamps");
	renderString(-0.8f, 0, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);

	sprintf_s(message, "a = %.1lf", a);
	renderString(-0.8f, -0.1f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);
}

/* cicloida
x = a * t - b * sin(t)
y = a - b * cos(t)
t - R

https://en.wikipedia.org/wiki/Cycloid
Un arc al unei cicloide generat de un cerc cu raza r poate fi parametrizat
	x = r(t - sin(t))
	y = r(1 - cos(t))
cu
	0 <= t <= 2*pi
*/
void Display6() {
	double ratio = 0.001;
	double a = 0.1;
	double b = 0.2;
	double pi = 4 * atan(1.0);
	double tmin = -2 * pi - pi;
	double tmax = 2 * pi + pi;
	double x, y;

	glColor3f(1.0, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double t = tmin; t <= tmax; t += ratio) {
		x = a * t - b * sin(t);
		y = a - b * cos(t);

		glVertex2f(x, y);
	}
	glEnd();

	char message[50];

	sprintf_s(message, "Cicloida");
	renderString(-0.5f, -0.6f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);

	sprintf_s(message, "a = %.1lf", a);
	renderString(-0.5f, -0.7f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);

	sprintf_s(message, "b = %.1lf", b);
	renderString(-0.5f, -0.8f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);
}

/* epicicloida
x = (R + r) * cos(r / R * t) - r * cos(t + r / R * t)
y = (R + r) * sin(r / R * t) - r * sin(t + r / R * t)
t - [0, 2*pi]
*/
void Display7() {
	double ratio = 0.001;
	double R = 0.1;
	double r = 0.3;
	double pi = 4 * atan(1.0);
	double tmin = 0.0;
	double tmax = 2 * pi;
	double x, y;

	glColor3f(1.0, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double t = tmin; t <= tmax; t += ratio) {
		x = (R + r) * cos(r / R * t) - r * cos(t + r / R * t);
		y = (R + r) * sin(r / R * t) - r * sin(t + r / R * t);

		glVertex2f(x, y);
	}
	glEnd();

	char message[50];

	sprintf_s(message, "Epicicloida");
	renderString(0.2f, -0.8f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);

	sprintf_s(message, "R = %.1lf   r = %.1lf", R, r);
	renderString(0.2f, -0.9f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);
}

/* hipocicloida
x = (R - r) * cos(r / R * t) - r * cos(t - r / R * t)
y = (R - r) * sin(r / R * t) - r * sin(t - r / R * t)
t - [0, 2*pi]
*/
void Display8() {
	double ratio = 0.001;
	double R = 0.1;
	double r = 0.3;
	double pi = 4 * atan(1.0);
	double tmin = 0.0;
	double tmax = 2 * pi;
	double x, y;

	glColor3f(1.0, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double t = tmin; t <= tmax; t += ratio) {
		x = (R - r) * cos(r / R * t) - r * cos(t - r / R * t);
		y = (R - r) * sin(r / R * t) - r * sin(t - r / R * t);

		glVertex2f(x, y);
	}
	glEnd();

	char message[50];

	sprintf_s(message, "Hipocicloida");
	renderString(0.2f, -0.5f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);

	sprintf_s(message, "R = %.1lf", R);
	renderString(0.2f, -0.6f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);

	sprintf_s(message, "r = %.1lf", r);
	renderString(0.2f, -0.7f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);
}

/* lemniscata lui Bernoulli
r = (+/-)a * sqrt(2 * cos(2 * t))
t - (-pi/4, pi/4) : ca sa se uneasca merg cu t: [-pi/4,pi/4]
*/
void Display9() {
	double ratio = 0.001;
	double a = 0.4;
	double pi = 4 * atan(1.0);
	double tmin = -pi / 4;
	double tmax = pi / 4;
	double r, x, y;

	glColor3f(1.0, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);

	// partea dreapta
	for (double t = tmin; t <= tmax; t += ratio) {
		r = a * sqrt(2 * cos(2 * t));
		x = r * cos(t);
		y = r * sin(t);

		glVertex2f(x, y);
	}
	glEnd();

	// partea stanga
	glColor3f(1.0, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double t = tmin; t <= tmax; t += ratio) {
		r = -a * sqrt(2 * cos(2 * t));
		x = r * cos(t);
		y = r * sin(t);

		glVertex2f(x, y);
	}
	glEnd();

	char message[50];

	sprintf_s(message, "Lemniscata lui Bernoulli");
	renderString(-0.6f, -0.6f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);

	sprintf_s(message, "a = %.1lf", a);
	renderString(-0.6f, -0.7f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);
}

/* spirala logaritmica
r = a * exp(1 + t)
t - (0, inf) , am considerat t: (0, pi)
*/
void Display10() {
	double ratio = 0.001;
	double a = 0.02;
	double pi = 4 * atan(1.0);
	double tmin = ratio;
	double tmax = pi;
	double r, x, y;
	double distanceFromLeft = 0.3;

	glColor3f(1.0, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double t = tmin; t < tmax; t += ratio) {
		r = a * exp(1 + t);
		x = r * cos(t);
		y = r * sin(t);

		glVertex2f(x + distanceFromLeft, y);
	}
	glEnd();

	char message[50];

	sprintf_s(message, "Spirala logaritmica");
	renderString(-0.7f, -0.4f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);

	sprintf_s(message, "a = %.2lf", a);
	renderString(-0.7f, -0.5f, GLUT_BITMAP_HELVETICA_12, message, 0, 0, 0);
}

void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	//   glPointSize(4);

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
	case '5':
		Display5();
		break;
	case '6':
		Display6();
		break;
	case '7':
		Display7();
		break;
	case '8':
		Display8();
		break;
	case '9':
		Display9();
		break;
	case '0':
		Display10();
		break;
	default:
		break;
	}

	glFlush();
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
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