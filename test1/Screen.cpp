#include "Screen.h"
#include <cstdio>

Screen * Screen::currentInstance;
const int Screen::MAXN, Screen::MAXM;
const int Screen::DRAW_WIDTH, Screen::DRAW_HEIGHT;
const int Screen::WINDOW_INITSIZE_WIDTH, Screen::WINDOW_INITSIZE_HEIGHT;

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
	gluOrtho2D(0.0, WINDOW_INITSIZE_WIDTH, 0.0, WINDOW_INITSIZE_HEIGHT);
}

void Screen::DrawSquare()
{
	int StepW = DRAW_WIDTH / MAXN, StepH = DRAW_HEIGHT / MAXM;
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
			glVertex2i(i*StepW, DRAW_HEIGHT);
		}
		for (int i = 0; i <= MAXN; i++) {
			glVertex2i(0, i*StepH);
			glVertex2i(DRAW_WIDTH, i*StepH);
		}
	glEnd();

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
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h == 0)
		h = 1;
	if (w > h)
		gluOrtho2D(0.0, 1.0* DRAW_WIDTH*w / h, 0.0, DRAW_HEIGHT);
	else
		gluOrtho2D(0.0, DRAW_WIDTH, 0.0, 1.0* DRAW_HEIGHT*h / w);
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
		CUR_MULTIPLE *= 0.9;
		glScaled(0.9, 0.9, 1);
		::glutPostRedisplay();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		printf("L\n");
		CUR_MULTIPLE *= 1.1;
		glScaled(1.1, 1.1, 1);
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

void Screen::SpecialKeyBoard(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		glTranslatef(0.0, 3.0, 0.0);
		::glutPostRedisplay();
	}
	else if(key == GLUT_KEY_DOWN)
	{
		glTranslatef(0.0, -3.0, 0.0);
		::glutPostRedisplay();
	}
	else if(key == GLUT_KEY_LEFT)
	{
		glTranslatef(-3.0, 0.0, 0.0);
		::glutPostRedisplay();
	}
	else if(key == GLUT_KEY_RIGHT)
	{
		glTranslatef(3.0, 0.0, 0.0);
		::glutPostRedisplay();
	}
}

void Screen::SpecialKeyBoardCallback(int key, int x, int y) {
	currentInstance->SpecialKeyBoard(key, x, y);
}

void Screen::SetupSpecialKeyBoardCallback() {
	currentInstance = this;
	::glutSpecialFunc(SpecialKeyBoardCallback);
}

void Screen::DrawWindow(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_INITSIZE_WIDTH, WINDOW_INITSIZE_HEIGHT);
	glutCreateWindow("This is a simulation window");

	InitGlut();
	SetPixel(0, 0, 0.0, 0.0, 0.0);
	SetupDisplayCallback();
	SetupReshapeCallback();
	SetupMouseButtonCallback();
	SetupSpecialKeyBoardCallback();
	glutMainLoop();
}
