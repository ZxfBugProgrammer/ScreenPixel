#include "Screen.h"
#include <cstdio>

Screen * Screen::currentInstance;
const int Screen::MAXN, Screen::MAXM;
const int Screen::WINDOW_INITSIZE_WIDTH, Screen::WINDOW_INITSIZE_HEIGHT;

Screen::Screen() {
	CUR_MULTIPLE = 1;
	START_X = 0, START_Y = 0,STEP_WIDTH = 20,STEP_HEIGHT = 20;
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

void Screen::GetWorldCoordinate(int &x, int &y) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	x = posX, y = posY;
}

void Screen::DrawSquare()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
		for (int i = 0; i < MAXM; i++) {
			for (int j = 0; j < MAXN; j++) {
				glColor3f(ScreenPixelColor[i][j][0], ScreenPixelColor[i][j][1], ScreenPixelColor[i][j][2]);
				glVertex2d(START_X + j * STEP_WIDTH, START_Y + i * STEP_HEIGHT);
				glVertex2d(START_X + (j + 1) * STEP_WIDTH, START_Y + i * STEP_HEIGHT);
				glVertex2d(START_X + (j + 1) * STEP_WIDTH, START_Y + (i + 1) * STEP_HEIGHT);
				glVertex2d(START_X + j * STEP_WIDTH, START_Y + (i + 1) * STEP_HEIGHT);
			}
		}
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
		for (int i = 0; i <= MAXM; i++) {
			glVertex2d(START_X + i*STEP_WIDTH, START_Y + 0);
			glVertex2d(START_X + i * STEP_WIDTH, START_Y + MAXN * STEP_HEIGHT);
		}
		for (int i = 0; i <= MAXN; i++) {
			glVertex2d(START_X + 0, START_Y + i*STEP_HEIGHT);
			glVertex2d(START_X + MAXM * STEP_WIDTH, START_Y + i * STEP_HEIGHT);
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
	NOW_WIDTH = w, NOW_HEIGHT = h;
	GLdouble DRAW_WIDTH = START_X + MAXM * STEP_WIDTH;
	GLdouble DRAW_HEIGHT = START_Y + MAXN * STEP_HEIGHT;
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
		STEP_HEIGHT *= 0.9;
		STEP_WIDTH *= 0.9;
		::glutPostRedisplay();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		printf("L\n");
		CUR_MULTIPLE *= 1.1;
		STEP_HEIGHT *= 1.1;
		STEP_WIDTH *= 1.1;
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

void Screen::MousePassiveMotion(GLint x, GLint y) {
	//y = NOW_HEIGHT - 1 - y;
	GetWorldCoordinate(x, y);
	printf("x: %d y: %d\n", x, y);
	if ((START_X <= x && x <= START_X + MAXM * STEP_WIDTH) &&
		(START_Y <= y && y <= START_Y + MAXN * STEP_HEIGHT)) {
		GLint n, m;
		m = ((x - START_X) / STEP_WIDTH);
		n = ((y - START_Y) / STEP_HEIGHT);
		printf("m:%d n:%d\n", m, n);
		ScreenPixelColor[n][m][0] = 1.0;
		ScreenPixelColor[n][m][1] = 1.0;
		ScreenPixelColor[n][m][2] = 1.0;
		::glutPostRedisplay();
	}
}

void Screen::MousePassiveMotionCallback(int x, int y) {
	currentInstance->MousePassiveMotion(x,y);
}

void Screen::SetupMousePassiveMotionCallback() {
	currentInstance = this;
	::glutPassiveMotionFunc(MousePassiveMotionCallback);
}

void Screen::SpecialKeyBoard(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		START_Y += 3.0;
		::glutPostRedisplay();
	}
	else if(key == GLUT_KEY_DOWN)
	{
		START_Y -= 3.0;
		::glutPostRedisplay();
	}
	else if(key == GLUT_KEY_LEFT)
	{
		START_X -= 3.0;
		::glutPostRedisplay();
	}
	else if(key == GLUT_KEY_RIGHT)
	{
		START_X += 3.0;
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
	SetupMousePassiveMotionCallback();
	glutMainLoop();
}
