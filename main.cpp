#include "include/glui.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <stdio.h>
#include <iostream>


using namespace std;

GLUI *glui;
GLUI_Panel *panel;
int displayWin;
float width = 700;

int show_ControlPolygon = 1,
show_Curve = 1,
show_CurveExtended = 0,
show_DeCasteljau = 0,
show_ConvexHull = 0,
show_SecondCurve = 0;

int flag = 1;

GLfloat curveR = 1.0,
curveG = 1.0,
curveB = 0.0;

GLfloat pointR = 1.0,
pointG = 0.0,
pointB = 0.0;

GLfloat lineR = 0.0,
lineG = 1.0,
lineB = 0.0;

GLfloat chR = 1.0,
chG = 1.0,
chB = 1.0;

struct Point
{
	int x, y;
	Point() {};
	Point(int tx, int ty)
	{
		x = tx;
		y = ty;
	}
};

vector<Point> p;
vector<Point> p2;
vector<Point> save;
vector<Point> save2;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glFlush();
}

void Bezier() {
	int n = p.size();
	float xc[32];
	float yc[32];
	float points[100][2];

	int tmp = 0;
	for (double t = 0; t <= 1; t += 0.01)
	{
		for (int i = 1; i < n; ++i)
		{
			for (int j = 0; j < n - i; ++j)
			{
				if (i == 1)
				{
					xc[j] = p[j].x * (1 - t) + p[j + 1].x * t;
					yc[j] = p[j].y * (1 - t) + p[j + 1].y * t;
					continue;
				}

				xc[j] = xc[j] * (1 - t) + xc[j + 1] * t;
				yc[j] = yc[j] * (1 - t) + yc[j + 1] * t;
			}

		}
		points[tmp][0] = xc[0];
		points[tmp][1] = yc[0];
		tmp++;
	}

	if (show_Curve == 1) {
		glColor3f(curveR, curveG, curveB);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 100; i++)
		{
			glVertex2fv(points[i]);
		}
		glEnd();
	}
}

void Bezier2() {
	glPointSize(5);
	glColor3f(pointR, pointG, pointB);
	glBegin(GL_POINTS);
	for (int i = 0; i < p2.size(); i++)
		glVertex2f(p2[i].x, p2[i].y);
	glEnd();

	// draw lines
	glLineWidth(2);
	glColor3f(lineR, lineG, lineB);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < p2.size(); i++)
		glVertex2f(p2[i].x, p2[i].y);
	glEnd();
	int n = p2.size();
	float xc[32];
	float yc[32];
	float points[100][2];

	int tmp = 0;
	for (double t = 0; t <= 1; t += 0.01)
	{
		for (int i = 1; i < n; ++i)
		{
			for (int j = 0; j < n - i; ++j)
			{
				if (i == 1)
				{
					xc[j] = p2[j].x * (1 - t) + p2[j + 1].x * t;
					yc[j] = p2[j].y * (1 - t) + p2[j + 1].y * t;
					continue;
				}

				xc[j] = xc[j] * (1 - t) + xc[j + 1] * t;
				yc[j] = yc[j] * (1 - t) + yc[j + 1] * t;
			}

		}
		points[tmp][0] = xc[0];
		points[tmp][1] = yc[0];
		tmp++;
	}

	if (show_Curve == 1) {
		glColor3f(curveR, curveG, curveB);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 100; i++)
		{
			glVertex2fv(points[i]);
		}
		glEnd();
	}
}

void convexHull() {
	if (p.size() > 2) {
		for (int i = 0; i < p.size(); i++) {
			for (int j = i + 1; j < p.size(); j++) {
				bool left = 0;
				bool right = 0;
				for (int k = 0; k < p.size(); k++) {
					if (((p[j].x - p[i].x) * (p[k].y - p[i].y) - (p[k].x - p[i].x) * (p[j].y - p[i].y)) > 0) {
						left = 1;
					}
					if (((p[j].x - p[i].x) * (p[k].y - p[i].y) - (p[k].x - p[i].x) * (p[j].y - p[i].y)) < 0) {
						right = 1;
					}
				}
				if ((left == 1 && right == 0) || (left == 0 && right == 1)) {
					glBegin(GL_LINE_STRIP);
					glColor3f(chR, chG, chB);
					glVertex2f(p[i].x, p[i].y);
					glVertex2f(p[j].x, p[j].y);
					glEnd();
				}
			}
		}
	}
	if (p2.size() > 2) {
		for (int i = 0; i < p2.size(); i++) {
			for (int j = i + 1; j < p2.size(); j++) {
				bool left = 0;
				bool right = 0;
				for (int k = 0; k < p2.size(); k++) {
					if (((p2[j].x - p2[i].x) * (p2[k].y - p2[i].y) - (p2[k].x - p2[i].x) * (p2[j].y - p2[i].y)) > 0) {
						left = 1;
					}
					if (((p2[j].x - p2[i].x) * (p2[k].y - p2[i].y) - (p2[k].x - p2[i].x) * (p2[j].y - p2[i].y)) < 0) {
						right = 1;
					}
				}
				if ((left == 1 && right == 0) || (left == 0 && right == 1)) {
					glBegin(GL_LINE_STRIP);
					glColor3f(chR, chG, chB);
					glVertex2f(p2[i].x, p2[i].y);
					glVertex2f(p2[j].x, p2[j].y);
					glEnd();
				}
			}
		}
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);  

	// draw points
	glPointSize(5);
	glColor3f(pointR, pointG, pointB);
	glBegin(GL_POINTS);
	for (int i = 0; i < p.size(); i++)
		glVertex2f(p[i].x, p[i].y);
	glEnd();

	// draw lines
	glLineWidth(2);
	glColor3f(lineR, lineG, lineB);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < p.size(); i++)
		glVertex2f(p[i].x, p[i].y);
	glEnd();

	// draw the Beizer Curve
	if (p.size() <= 32)
		Bezier();
	
	if (p2.size() <= 32)
		Bezier2();

	// draw the Convex Hull
	if (show_ConvexHull == 1)
		convexHull();

	glFlush();
	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && flag == 1) {
		if (p.size() < 32) {
			float dx = (float)(x - (width / 2)) / (width / 2);
			float dy = -(float)(y - (width / 2)) / (width / 2);
			Point t(x, y);
			p.push_back(t);
			std::cout << "x:" << dx << " y:" << dy << std::endl;
			glutPostRedisplay();
		}
		else {
			cout << "max points reached" << endl;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && flag == 2) {
		if (p2.size() < 32) {
			float dx = (float)(x - (width / 2)) / (width / 2);
			float dy = -(float)(y - (width / 2)) / (width / 2);
			Point t(x, y);
			p2.push_back(t);
			std::cout << "x:" << dx << " y:" << dy << std::endl;
			glutPostRedisplay();
		}
		else {
			cout << "max points reached" << endl;
		}
	}
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Exit(int) {
	exit(0);
}

void Clear(int) {
	save = p;
	save2 = p2;
	p.clear();
	p2.clear();
	glutSetWindow(displayWin);
	glutPostRedisplay();
	std::cout << "Clear all" << std::endl;
}

void Undo(int) {
	if (flag == 1) {
		if (!p.empty()) {
			p.pop_back();
			glutSetWindow(displayWin);
			glutPostRedisplay();
		}
		else {
			p = save;
			glutSetWindow(displayWin);
			glutPostRedisplay();
		}
	}
	else {
		if (!p2.empty()) {
			p2.pop_back();
			glutSetWindow(displayWin);
			glutPostRedisplay();
		}
		else {
			p2 = save2;
			glutSetWindow(displayWin);
			glutPostRedisplay();
		}
	}
	std::cout << "Undo the last operation" << std::endl;
}


void check(int r) {
	if (r == 1) {
		cout << "Show the control polygon ON/OFF." << endl;
	}
	if (r == 2) {
		cout << "Show the curve ON/OFF." << endl;
		glutSetWindow(displayWin);
		glutPostRedisplay();
	}
	if (r == 3) {
		cout << "Show the extended curve ON/OFF." << endl;
	}
	if (r == 4) {
		cout << "Show de Casteljau ON/OFF." << endl;
	}
	if (r == 5) {
		cout << "Show the convex hull ON/OFF." << endl;
		glutSetWindow(displayWin);
		glutPostRedisplay();
	}
	if (r == 6) {
		if (flag == 1) {
			flag = 2;
			cout << "Curve " << flag << ":" << endl;
		}
		else {
			flag = 1;
			cout << "Curve " << flag << ":" << endl;
		}
	}
}

void color_ctrl(int r) {
	if (r == 7) {
		printf("Curves' Red just changed to %d\n", curveR);
	}
	if (r == 8) {
		printf("Curves' Green just changed to %d\n", curveG);
	}
	if (r == 9) {
		printf("Curves' Blue just changed to %d\n", curveB);
	}
	if (r == 10) {
		printf("Points' Red just changed to %d\n", pointR);
	}
	if (r == 11) {
		printf("Points' Green just changed to %d\n", pointG);
	}
	if (r == 12) {
		printf("Points' Blue just changed to %d\n", pointB);
	}
	if (r == 13) {
		printf("Lines' Red just changed to %d\n", lineR);
	}
	if (r == 14) {
		printf("Lines' Green just changed to %d\n", lineG);
	}
	if (r == 15) {
		printf("Lines' Blue just changed to %d\n", lineB);
	}
	if (r == 16) {
		printf("Convex Hull's Red just changed to %d\n", lineR);
	}
	if (r == 17) {
		printf("Convex Hull's Green just changed to %d\n", lineG);
	}
	if (r == 18) {
		printf("Convex Hull's Blue just changed to %d\n", lineB);
	}
}

void buildGlui(int displayWin) {
	GLUI_Master.set_glutDisplayFunc(display);
	GLUI_Master.set_glutReshapeFunc(resize);
	glui = GLUI_Master.create_glui_subwindow(displayWin, GLUI_SUBWINDOW_RIGHT);

	GLUI_Panel * panel = new GLUI_Panel(glui, "  ", GLUI_PANEL_NONE);
	new GLUI_Button(panel, "EXIT", 1, Exit);
	new GLUI_Button(panel, "Clear", 2, Clear);
	new GLUI_Button(panel, "Undo", 3, Undo);

	GLUI_Panel *panel2 = new GLUI_Panel(glui, "Mouse Left", GLUI_PANEL_EMBOSSED);
	new GLUI_Checkbox(panel2, "Ctrl Polygon", &show_ControlPolygon, 1, check);
	new GLUI_Checkbox(panel2, "Curve", &show_Curve, 2, check);
	new GLUI_Checkbox(panel2, "Curve Extdd", &show_CurveExtended, 3, check);
	new GLUI_Checkbox(panel2, "De Casteljau", &show_DeCasteljau, 4, check);
	new GLUI_Checkbox(panel2, "Convex Hull", &show_ConvexHull, 5, check);
	new GLUI_Checkbox(panel2, "The Second Curve", &show_SecondCurve, 6, check);

	GLUI_Panel *panel3 = new GLUI_Panel(glui, "Colors", GLUI_PANEL_EMBOSSED);
	new GLUI_EditText(panel3, "Curves' Red",  &curveR, 7, color_ctrl);
	new GLUI_EditText(panel3, "Curves' Green", &curveG, 8, color_ctrl);
	new GLUI_EditText(panel3, "Curves' Blue", &curveB, 9, color_ctrl);
	new GLUI_EditText(panel3, "Points' Red", &curveR, 10, color_ctrl);
	new GLUI_EditText(panel3, "Points' Green", &curveG, 11, color_ctrl);
	new GLUI_EditText(panel3, "Points' Blue", &curveB, 12, color_ctrl);
	new GLUI_EditText(panel3, "Lines' Red", &curveR, 13, color_ctrl);
	new GLUI_EditText(panel3, "Lines' Green", &curveG, 14, color_ctrl);
	new GLUI_EditText(panel3, "Lines' Blue", &curveB, 15, color_ctrl);
	new GLUI_EditText(panel3, "Convex Hull's Red", &curveR, 16, color_ctrl);
	new GLUI_EditText(panel3, "Convex Hull's Green", &curveG, 17, color_ctrl);
	new GLUI_EditText(panel3, "Convex Hull's Blue", &curveB, 18, color_ctrl);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(width, width);
	glutInitWindowPosition(100, 100);
	displayWin = glutCreateWindow("Bezier Curve");
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	
	init();
	buildGlui(displayWin);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);

	glutMainLoop();
	return 0;
}