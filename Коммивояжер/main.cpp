#include <iostream>
#include <sstream>
#include "base/glbag.h"
#include "Kommi.h"
#include "Tree.h"
#include "Graph.h"
#include <windows.h>



using namespace std;

extern int WINDOW_WIDTH = 960;
extern int WINDOW_HEIGHT = 640;

Kommi* kommi = Kommi::instance();
Graph graph;
KTree* hard;

void initGL() {
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	glClearColor(bgColor.red, bgColor.green, bgColor.blue, 1);
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
}

void onReshape(int w, int h)
{
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

int dragIndex = -1;

void onMouseClick(int button, int state, int x, int y) {
	y = WINDOW_HEIGHT - y;
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && dragIndex == -1) {
		if (!(x <= 0 || x >= WINDOW_WIDTH || y <= 0 || y >= WINDOW_HEIGHT))
			dragIndex = graph.getByCoord(x, y);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		dragIndex = -1;
	}
}

void onMouseMove(int x, int y) {
	y = WINDOW_HEIGHT - y;
	graph.markHovered(x, y);
	if (dragIndex != -1) {
		if (!(x <= 0 || x >= WINDOW_WIDTH || y <= 0 || y >= WINDOW_HEIGHT))
			graph.setCoord(dragIndex, x, y);
	}

	glutPostRedisplay(); // redraw
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (kommi->recompute == true) {
		delete hard;
		hard = kommi->Solve(false);
		kommi->recompute = false;
	}
	graph.draw(kommi->getMatrix(), kommi->getNames(), hard);
	if (hard->haveSolution) {
		string solution = kommi->getSolutionPath(hard);
		drawText(WINDOW_WIDTH, 10, kommi->getSolutionPath(hard) + (solution != "No solution" ? " (" + to_string(hard->wayLen) + " miles)" : ""), GLUT_BITMAP_TIMES_ROMAN_24, GL_TEXT_ALIGN_RIGHT | GL_TEXT_ALIGN_BOTTOM);
	}
	else {
		drawText(WINDOW_WIDTH - 25, 0, "No solution", GLUT_BITMAP_TIMES_ROMAN_24, GL_TEXT_ALIGN_RIGHT);
	}
	graph.drawMatrix(kommi->getMatrix(), kommi->getNames());

	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");
	// 5 1 2 3 4 5 20 18 12 8 5 14 7 11 12 18 6 11 11 17 11 12 5 5 5 5
	kommi->init();
	kommi->PrintMatrix();
	KTree* easy = kommi->Solve(true);
	cout << easy->wayLen << endl;
	cout << kommi->getSolutionPath(easy) << endl;

	hard = kommi->Solve(false);
	cout << hard->wayLen << endl;
	cout << kommi->getSolutionPath(hard) << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Коммивояжёр");
	initGL();
	glutReshapeFunc(onReshape);
	glutDisplayFunc(display);
	glutMouseFunc(onMouseClick);
	glutMotionFunc(onMouseMove);
	glutPassiveMotionFunc(onMouseMove);
	glutMainLoop();
	return 0;
}