#include "Screen.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>

Screen * Screen::currentInstance;
const int Screen::MAXN, Screen::MAXM;
const int Screen::WINDOW_INITSIZE_WIDTH, Screen::WINDOW_INITSIZE_HEIGHT;

void Screen::InitScreenPoint() {
	for (int i = 0; i <= MAXM; i++)
		for (int j = 0; j <= MAXN; j++)
			ScreenPoint[i][j] = Point(START_X + j * STEP_WIDTH, START_Y + i * STEP_HEIGHT);
}

Screen::Screen() {
	CUR_MULTIPLE = 1;
	START_X = 0, START_Y = 0, STEP_WIDTH = 20, STEP_HEIGHT = 20;
	LINE_START_X = -1, LINE_START_Y = -1;
	for (int i = 0; i < MAXM; i++) {
		for (int j = 0; j < MAXN; j++) {
			ScreenPixelColor[i][j][0] = 153.0 / 255.0;
			ScreenPixelColor[i][j][1] = 221.0 / 255.0;
			ScreenPixelColor[i][j][2] = 255.0 / 255.0;
		}
	}
	InitScreenPoint();
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
			glVertex2d(ScreenPoint[i][j].x, ScreenPoint[i][j].y);
			glVertex2d(ScreenPoint[i][j + 1].x, ScreenPoint[i][j + 1].y);
			glVertex2d(ScreenPoint[i + 1][j + 1].x, ScreenPoint[i + 1][j + 1].y);
			glVertex2d(ScreenPoint[i + 1][j].x, ScreenPoint[i + 1][j].y);
		}
	}
	glEnd();

	glColor3f(0.74, 0.74, 0.74);
	glBegin(GL_LINES);
	for (int i = 0; i <= MAXM; i++) {
		glVertex2d(ScreenPoint[i][0].x, ScreenPoint[i][0].y);
		glVertex2d(ScreenPoint[i][MAXN].x, ScreenPoint[i][MAXN].y);
	}
	for (int i = 0; i <= MAXN; i++) {
		glVertex2d(ScreenPoint[0][i].x, ScreenPoint[0][i].y);
		glVertex2d(ScreenPoint[MAXM][i].x, ScreenPoint[MAXM][i].y);
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
	GLdouble DRAW_WIDTH = START_X + MAXN * STEP_WIDTH;
	GLdouble DRAW_HEIGHT = START_Y + MAXM * STEP_HEIGHT;
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
	currentInstance->ScreenChangeSize(w, h);
}

void Screen::MouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		GetWorldCoordinate(x, y);
		if ((START_X <= x && x <= START_X + MAXM * STEP_WIDTH) &&
			(START_Y <= y && y <= START_Y + MAXN * STEP_HEIGHT)) {
			GLint n, m;
			m = ((x - START_X) / STEP_WIDTH);
			n = ((y - START_Y) / STEP_HEIGHT);
			if (LINE_START_X == -1 && LINE_START_Y == -1) {
				LINE_START_X = m, LINE_START_Y = n;
			}
			else
			{
				memcpy(TempPixelColor, ScreenPixelColor, sizeof(ScreenPixelColor));
				LINE_START_X = -1, LINE_START_Y = -1;
			}
			SetPixel(m, n ,0.0, 0.0, 0.0);
			vec.push_back(IntPoint(m, n));
			PolygonFill(vec);
			::glutPostRedisplay();
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		LINE_START_X = -1, LINE_START_Y = -1;
		memcpy(ScreenPixelColor, TempPixelColor, sizeof(TempPixelColor));
		vec.clear();
	}
}

void Screen::MouseButtonCallback(int button, int state, int x, int y) {
	currentInstance->MouseButton(button, state, x, y);
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
			
			//!!!!画线算法
			//DrawLineDDA(LINE_START_X, LINE_START_Y, m, n);
			//DrawLineBresenham(LINE_START_X, LINE_START_Y, m, n);

			//!!!!画圆算法
			/*int r = sqrt((m - LINE_START_X)*(m - LINE_START_X) + (n - LINE_START_Y)*(n - LINE_START_Y));
			//防止越界
			if (LINE_START_X + r < MAXN && LINE_START_X - r >= 0 && LINE_START_Y + r < MAXM && LINE_START_Y - r >= 0) {
				DrawCircle(LINE_START_X, LINE_START_Y, r);
			}*/

			//!!!!画椭圆算法
			/*int rx = abs(m - LINE_START_X) / 2, ry = abs(n - LINE_START_Y) / 2;
			int xc = (m + LINE_START_X) / 2, yc = (n + LINE_START_Y) / 2;
			DrawEllipse(xc, yc, rx, ry);*/

		}
		::glutPostRedisplay();
	}
}

void Screen::MousePassiveMotionCallback(GLint x, GLint y) {
	currentInstance->MousePassiveMotion(x, y);
}

void Screen::SetupMousePassiveMotionCallback() {
	currentInstance = this;
	::glutPassiveMotionFunc(MousePassiveMotionCallback);
}

void Screen::SpecialKeyBoard(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		START_Y += 10.0*CUR_MULTIPLE;
		InitScreenPoint();
		::glutPostRedisplay();
	}
	else if (key == GLUT_KEY_DOWN)
	{
		START_Y -= 10.0*CUR_MULTIPLE;
		InitScreenPoint();
		::glutPostRedisplay();
	}
	else if (key == GLUT_KEY_LEFT)
	{
		START_X -= 10.0*CUR_MULTIPLE;
		InitScreenPoint();
		::glutPostRedisplay();
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		START_X += 10.0*CUR_MULTIPLE;
		InitScreenPoint();
		::glutPostRedisplay();
	}
	else if (key == GLUT_KEY_PAGE_UP)
	{
		CUR_MULTIPLE *= 1.1;
		STEP_HEIGHT *= 1.1;
		STEP_WIDTH *= 1.1;
		InitScreenPoint();
		::glutPostRedisplay();
	}
	else if (key == GLUT_KEY_PAGE_DOWN)
	{
		CUR_MULTIPLE *= 0.9;
		STEP_HEIGHT *= 0.9;
		STEP_WIDTH *= 0.9;
		InitScreenPoint();
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
	//SetupMousePassiveMotionCallback();
	glutMainLoop();
}

void Screen::DrawLineDDA(int startx, int starty, int endx, int endy) {
	GLfloat delta_x, delta_y, x = startx, y = starty;
	int dx, dy, steps;
	dx = endx - startx;
	dy = endy - starty;
	steps = std::max(abs(dx), abs(dy));
	delta_x = (GLfloat)dx / (GLfloat)steps;
	delta_y = (GLfloat)dy / (GLfloat)steps;
	SetPixel(round(x), round(y), 1.0, 1.0, 1.0);
	for (int i = 0; i < steps; i++)
	{
		x += delta_x;
		y += delta_y;
		SetPixel(round(x), round(y), 1.0, 1.0, 1.0);
	}
}

void Screen::DrawLineBresenham(int startx, int starty, int endx, int endy) {
	int dx = abs(endx - startx), dy = abs(endy - starty);
	int x = startx, y = starty, stepX = 1, stepY = 1;
	if (startx > endx)
		stepX = -1;
	if (starty > endy)
		stepY = -1;

	if (dx > dy)
	{
		int p = dy * 2 - dx;
		for (int i = 0; i <= dx; i++)
		{
			SetPixel(x, y, 1.0, 1.0, 1.0);
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
			SetPixel(x, y, 1.0, 1.0, 1.0);
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

void Screen::DrawCircle(int xc, int yc, int radius) {
	int p = 1 - radius;
	Point circPt(0, radius);

	circlePlotPoints(xc, yc, circPt);
	while (circPt.x < circPt.y)
	{
		circPt.x++;
		if (p < 0)
			p += 2 * circPt.x + 1;
		else
		{
			circPt.y--;
			p += 2 * (circPt.x - circPt.y) + 1;
		}
		circlePlotPoints(xc, yc, circPt);
	}
}

void Screen::circlePlotPoints(int xc, int yc, Point pt) {
	SetPixel(xc + pt.x, yc + pt.y, 0.0, 0.0, 0.0);
	SetPixel(xc - pt.x, yc + pt.y, 0.0, 0.0, 0.0);
	SetPixel(xc + pt.x, yc - pt.y, 0.0, 0.0, 0.0);
	SetPixel(xc - pt.x, yc - pt.y, 0.0, 0.0, 0.0);
	SetPixel(xc + pt.y, yc + pt.x, 0.0, 0.0, 0.0);
	SetPixel(xc - pt.y, yc + pt.x, 0.0, 0.0, 0.0);
	SetPixel(xc + pt.y, yc - pt.x, 0.0, 0.0, 0.0);
	SetPixel(xc - pt.y, yc - pt.x, 0.0, 0.0, 0.0);
}

void Screen::DrawEllipse(int xc, int yc, int rx, int ry) {
	int rx2 = rx * rx, ry2 = ry * ry;
	Point pt(0, ry);
	int px = 0, py = 2 * rx2*pt.y, p = round(ry2 - (rx2*ry) + 0.25*rx2);
	ellipsePlotPoints(xc, yc, pt);
	while (px < py)
	{
		pt.x++;
		px += 2 * ry2;
		if (p < 0)
			p += ry2 + px;
		else
		{
			pt.y--;
			py -= 2 * rx2;
			p += ry2 + px - py;
		}
		ellipsePlotPoints(xc, yc, pt);
	}

	p = round(ry2*(pt.x + 0.5)*(pt.x + 0.5) + rx2 * (pt.y - 1)*(pt.y - 1) - rx2 * ry2);
	while (pt.y > 0) {
		pt.y--;
		py -= 2 * rx2;
		if (p > 0)
			p += rx2 - py;
		else
		{
			pt.x++;
			px += 2 * ry2;
			p += rx2 - py + px;
		}
		ellipsePlotPoints(xc, yc, pt);
	}
}

void Screen::ellipsePlotPoints(int xc, int yc, Point pt) {
	SetPixel(xc + pt.x, yc + pt.y, 0.0, 0.0, 0.0);
	SetPixel(xc - pt.x, yc + pt.y, 0.0, 0.0, 0.0);
	SetPixel(xc + pt.x, yc - pt.y, 0.0, 0.0, 0.0);
	SetPixel(xc - pt.x, yc - pt.y, 0.0, 0.0, 0.0);
}

void Screen::LineFill(int s, int e, int valY)
{
	for (int i = s; i <= e; i++)
		SetPixel(i, valY, 0.0, 0.0, 0.0);
}

void Screen::FillActiveEdgeTable(const LinkList & ActiveEdgeTable) {
	LNode *pa = ActiveEdgeTable.firstNode.next;
	while (pa) {
		LineFill(pa->data.x, pa->next->data.x, ActiveEdgeTable.val);
		pa = pa->next->next;
	}
}

void Screen::UpdateActiveEdgeTable(LinkList & ActiveEdgeTable) {
	puts("~~~~~~~~~~~~");
	ActiveEdgeTable.val++;
	LNode *p = ActiveEdgeTable.firstNode.next, *pre = &ActiveEdgeTable.firstNode;
	while (p) {
		if (p->data.y < ActiveEdgeTable.val) {
			pre->next = p->next;
			free(p);
			p = pre->next;
			continue;
		}
		p->data.cur += 2*p->data.deltax;
		while (abs(p->data.cur) > p->data.deltay) {
			p->data.cur -= (p->data.deltax > 0 ? 2 * p->data.deltay : -2 * p->data.deltay);
			p->data.x += (p->data.deltax > 0 ? 1 : -1);
		}
		printf("%d\n", p->data.x);

		pre = p;
		p = p->next;
	}
}

void Screen::PolygonFill(std::vector<IntPoint> vec) {
	//如果无法构成多边形，返回
	if (vec.size() <= 2) {
		return;
	}

	//将定点向量处理为边向量 计算出每条边的斜率倒数，去除水平线
	std::vector<Edge> ed;
	int tlen = (int)vec.size();
	int maxVal = -(int)1e9, minVal = (int)1e9;
	for (int i = 0; i < tlen; i++) {
		int nextPoint = (i + 1) % tlen;
		if (vec[i].y == vec[nextPoint].y) continue;
		int heighterPoint = vec[i].y > vec[nextPoint].y ? i : nextPoint;
		int lowerPoint = i + nextPoint - heighterPoint;
		ed.push_back(Edge(vec[i], vec[nextPoint],
			(vec[heighterPoint].y - vec[lowerPoint].y), (vec[heighterPoint].x - vec[lowerPoint].x)));
		maxVal = std::max(vec[i].y, maxVal);
		minVal = std::min(vec[i].y, minVal);
		printf("!!!%d x:%d y:%d\n", i, vec[i].x, vec[i].y);
	}

	//将边分离开 
	tlen = (int)ed.size();
	for (int i = 0; i < tlen; i++) {
		int nextEdge = (i + 1) % tlen;
		IntPoint sharePoint = ed[i].e;
		if ((ed[i].s.y - sharePoint.y)*(ed[nextEdge].e.y - sharePoint.y) < 0) {
			ed[i].s.y < ed[nextEdge].e.y ? ed[i].e.y-- : ed[nextEdge].s.y--;
		}
	}

	//初始化排序边表
	LinkList* SortedEdgeTable = new LinkList[maxVal - minVal + 10];
	for (int i = 0; i <= (maxVal - minVal); i++)
		SortedEdgeTable[i].val = i+minVal;
	for (int i = 0; i < tlen; i++) {
		IntPoint minPoint = ed[i].s.y < ed[i].e.y ? ed[i].s : ed[i].e;
		IntPoint maxPoint = ed[i].s.y < ed[i].e.y ? ed[i].e : ed[i].s;
		int pos = minPoint.y - minVal;
		::LinkListInsert(SortedEdgeTable[pos], Data(maxPoint.y, minPoint.x, ed[i].deltay, ed[i].deltax));
	}

	//初始化活化边表
	LinkList ActiveEdgeTable;
	ActiveEdgeTable.val = minVal;

	for (int i = 0; i <= (maxVal - minVal); i++) {
		//合并活化边表与相应的排序边表
		::LinkListMerge(ActiveEdgeTable, SortedEdgeTable[i]);
		//填充当前扫描线确定的多边形的内部区域
		FillActiveEdgeTable(ActiveEdgeTable);
		//更新活化边表的X值
		UpdateActiveEdgeTable(ActiveEdgeTable);
	}

	for (int i = 0; i < tlen; i++) {
		int nextPoint = (i + 1) % tlen;
		DrawLineBresenham(vec[i].x, vec[i].y, vec[nextPoint].x, vec[nextPoint].y);
	}
}
