#include "Screen.h"
#include <cstdio>

Screen * Screen::currentInstance;

Screen::Screen() {
	CUR_MULTIPLE = 1;
	int flag;
	for (int i = 0; i < MAXM; i++) {
		flag = i % 2;
		for (int j = 0; j < MAXN; j++) {
			ScreenPixelColor[i][j][0] = 1.0;
			ScreenPixelColor[i][j][1] = flag;
			ScreenPixelColor[i][j][2] = 0.0;
			flag ^= 1;
		}
	}
}

void Screen::SetPixel(int x, int y, double cr, double cg, double cb)
{
	ScreenPixelColor[y][x][0] = cr;
	ScreenPixelColor[y][x][1] = cg;
	ScreenPixelColor[y][x][2] = cb;
}

void Screen::InitGlut() {
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 300.0, 0.0, 300.0);
}

void Screen::DrawSquare()
{
	int StepW = WINDOW_WIDTH / MAXN, StepH = WINDOW_HEIGHT / MAXM;
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
		for (int i = 0; i < MAXM; i++) {
			for (int j = 0; j < MAXN; j++) {
				glColor3f(ScreenPixelColor[i][j][0], ScreenPixelColor[i][j][1], ScreenPixelColor[i][j][2]);
				glVertex2i(j * StepW, i * StepH);
				glVertex2i((j + 1) * StepW, i * StepH);
				glVertex2i((j + 1) * StepW, (i + 1) * StepH);
				glVertex2i(j * StepW, (i + 1) * StepH);
			}
		}
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
		for (int i = 0; i <= MAXM; i++) {
			glVertex2i(i*StepW, 0);
			glVertex2i(i*StepW, WINDOW_HEIGHT);
		}
		for (int i = 0; i <= MAXN; i++) {
			glVertex2i(0, i*StepH);
			glVertex2i(WINDOW_WIDTH, i*StepH);
		}
	glEnd();
	glScaled(CUR_MULTIPLE,CUR_MULTIPLE, 1);

	glFlush();
}

void Screen::SetupDisplayCallback() {
	currentInstance = this;
	::glutDisplayFunc(Screen::DisplayCallback);
}

void Screen::DisplayCallback()
{
	currentInstance->DrawSquare();
}

void Screen::ScreenChangeSize(GLsizei w, GLsizei h) {
	glViewport(0, 0, 2*w, 2*h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, w, 0.0, h);
}

void Screen::SetupReshapeCallback() {
	currentInstance = this;
	::glutReshapeFunc(Screen::ReshapeCallback);
}

void Screen::ReshapeCallback(GLsizei w, GLsizei h)
{
	currentInstance->ScreenChangeSize(w,h);
}

void Screen::MouseButton(int button, int state, int x, int y) {
	printf("BUTTON:%d  STATE:%d  X:%d  Y:%d\n", button, state, x, y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		printf("S\n");
		CUR_MULTIPLE = 1-0.1;
		::glutPostRedisplay();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		printf("L\n");
		CUR_MULTIPLE = 1+0.1;
		::glutPostRedisplay();
	}
}

void Screen::MouseButtonCallback(int button, int state, int x, int y) {
	currentInstance->MouseButton(button,state,x,y);
}

void Screen::SetupMouseButtonCallback() {
	currentInstance = this;
	::glutMouseFunc(MouseButtonCallback);
}

void Screen::DrawWindow(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);
	glutCreateWindow("This is a simulation window");

	InitGlut();
	SetPixel(0, 0, 0.0, 0.0, 0.0);
	SetupDisplayCallback();
	SetupReshapeCallback();
	SetupMouseButtonCallback();
	glutMainLoop();
}
