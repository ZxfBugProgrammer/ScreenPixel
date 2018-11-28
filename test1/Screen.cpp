#include "Screen.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>

Screen * Screen::currentInstance;
const int Screen::MAXN, Screen::MAXM;
const int Screen::WINDOW_INITSIZE_WIDTH, Screen::WINDOW_INITSIZE_HEIGHT;

Screen::Screen() {
	CUR_MULTIPLE = 1;
	START_X = 0, START_Y = 0,STEP_WIDTH = 20,STEP_HEIGHT = 20;
	LINE_START_X = -1, LINE_START_Y = -1;
	for (int i = 0; i < MAXM; i++) {
		for (int j = 0; j < MAXN; j++) {
			ScreenPixelColor[i][j][0] = 153.0 / 255.0;
			ScreenPixelColor[i][j][1] = 221.0 / 255.0;
			ScreenPixelColor[i][j][2] = 255.0 / 255.0;
		}
	}
	memcpy(TempPixelColor, ScreenPixelColor, sizeof(ScreenPixelColor));
}

void Screen::SetPixel(int x, int y, double cr, double cg, double cb)
{
	ScreenPixelColor[y][x][0] = cr;
	ScreenPixelColor[y][x][1] = cg;
	ScreenPixelColor[y][x][2] = cb;
}

void Screen::GetPixel(int x, int y, double &cr, double &cg, double &cb) {
	cr = ScreenPixelColor[y][x][0];
	cg = ScreenPixelColor[y][x][1];
	cb = ScreenPixelColor[y][x][2];
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

	glColor3f(0.74, 0.74, 0.74);
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		GetWorldCoordinate(x, y);
		if ((START_X <= x && x <= START_X + MAXM * STEP_WIDTH) &&
			(START_Y <= y && y <= START_Y + MAXN * STEP_HEIGHT)) {
			if (LINE_START_X == -1 && LINE_START_Y == -1) {
				GLint n, m;
				m = ((x - START_X) / STEP_WIDTH);
				n = ((y - START_Y) / STEP_HEIGHT);
				LINE_START_X = m, LINE_START_Y = n;
			}
			else
			{
				memcpy(TempPixelColor, ScreenPixelColor, sizeof(ScreenPixelColor));
				LINE_START_X = -1, LINE_START_Y = -1;
			}
			
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		LINE_START_X = -1, LINE_START_Y = -1;
		memcpy(ScreenPixelColor, TempPixelColor, sizeof(TempPixelColor));
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
	GetWorldCoordinate(x, y);
	if ((START_X <= x && x <= START_X + MAXM * STEP_WIDTH) &&
		(START_Y <= y && y <= START_Y + MAXN * STEP_HEIGHT)) {
		GLint n, m;
		m = ((x - START_X) / STEP_WIDTH);
		n = ((y - START_Y) / STEP_HEIGHT);
		if (LINE_START_X != -1 && LINE_START_Y != -1) {
			memcpy(ScreenPixelColor, TempPixelColor, sizeof(TempPixelColor));
			DrawLineDDA(LINE_START_X, LINE_START_Y, m, n);
			//DrawLineBresenham(LINE_START_X, LINE_START_Y, m, n);
		}
		::glutPostRedisplay();
	}
}

void Screen::MousePassiveMotionCallback(GLint x, GLint y) {
	currentInstance->MousePassiveMotion(x,y);
}

void Screen::SetupMousePassiveMotionCallback() {
	currentInstance = this;
	::glutPassiveMotionFunc(MousePassiveMotionCallback);
}

void Screen::SpecialKeyBoard(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		START_Y += 5.0*CUR_MULTIPLE;
		::glutPostRedisplay();
	}
	else if(key == GLUT_KEY_DOWN)
	{
		START_Y -= 5.0*CUR_MULTIPLE;
		::glutPostRedisplay();
	}
	else if(key == GLUT_KEY_LEFT)
	{
		START_X -= 5.0*CUR_MULTIPLE;
		::glutPostRedisplay();
	}
	else if(key == GLUT_KEY_RIGHT)
	{
		START_X += 5.0*CUR_MULTIPLE;
		::glutPostRedisplay();
	}
	else if (key == GLUT_KEY_PAGE_UP)
	{
		CUR_MULTIPLE *= 1.1;
		STEP_HEIGHT *= 1.1;
		STEP_WIDTH *= 1.1;
		::glutPostRedisplay();
	}
	else if(key == GLUT_KEY_PAGE_DOWN)
	{
		CUR_MULTIPLE *= 0.9;
		STEP_HEIGHT *= 0.9;
		STEP_WIDTH *= 0.9;
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
	SetupDisplayCallback();
	SetupReshapeCallback();
	SetupMouseButtonCallback();
	SetupSpecialKeyBoardCallback();
	SetupMousePassiveMotionCallback();
	glutMainLoop();
}

void Screen::DrawLineDDA(int startx, int starty, int endx, int endy) {
	GLfloat delta_x, delta_y, x = startx, y= starty;
	int dx, dy, steps;
	dx = endx - startx;
	dy = endy - starty;
	steps = std::max(abs(dx), abs(dy));
	delta_x = (GLfloat)dx / (GLfloat)steps;
	delta_y = (GLfloat)dy / (GLfloat)steps;
	SetPixel(round(x), round(y), 0.0, 0.0, 0.0);
	for (int i = 0; i < steps; i++)
	{
		x += delta_x;
		y += delta_y;
		SetPixel(round(x), round(y), 0.0, 0.0, 0.0);
	}
}

void Screen::DrawLineBresenham(int startx, int starty, int endx, int endy) {
	int dx = abs(endx - startx),dy = abs(endy - starty);
	int x = startx,y = starty,stepX = 1, stepY = 1;
	if (startx > endx)
		stepX = -1;
	if (starty > endy)
		stepY = -1;

	if (dx > dy)
	{
		int p = dy * 2 - dx;
		for (int i = 0; i <= dx; i++)
		{
			SetPixel(x, y, 0.0, 0.0, 0.0);
			x += stepX;
			p += 2 * dy;
			if (p >= 0)
			{
				y += stepY;
				p -= 2 * dx;
			}
		}
	}
	else
	{
		int p = 2 * dx - dy;
		for (int i = 0; i <= dy; i++)
		{
			SetPixel(x, y, 0.0, 0.0, 0.0);
			y += stepY;
			p += 2 * dx;
			if (p >= 0)
			{
				x += stepX;
				p -= 2 * dy;
			}
		}
	}
}

