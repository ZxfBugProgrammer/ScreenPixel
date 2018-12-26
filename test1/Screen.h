#ifndef __SCREEN__H__
#define __SCREEN__H__

#include <GL/glut.h>
#include "BaseStruct.h"
#include "LinkList.h"
#include <vector>


class Screen
{
public:

	Screen();

	void SetPixel(int x, int y, double cr, double cg, double cb);
	void GetPixel(int x, int y, double &cr, double &cg, double &cb);

	void InitScreenPoint();

	void InitGlut();
	void GetWorldCoordinate(int &x, int &y);

	void DrawSquare();
	static void DisplayCallback();
	void SetupDisplayCallback();

	void ScreenChangeSize(GLsizei w, GLsizei h);
	static void ReshapeCallback(GLsizei w, GLsizei h);
	void SetupReshapeCallback();

	void MouseButton(int button, int state, int x, int y);
	static void MouseButtonCallback(int button, int state, int x, int y);
	void SetupMouseButtonCallback();

	void MousePassiveMotion(GLint x, GLint y);
	static void MousePassiveMotionCallback(GLint x, GLint y);
	void SetupMousePassiveMotionCallback();

	void SpecialKeyBoard(int key, int x, int y);
	static void SpecialKeyBoardCallback(int key, int x, int y);
	void SetupSpecialKeyBoardCallback();

	void DrawWindow(int argc, char** argv);

	void DrawLineDDA(int startx, int starty, int endx, int endy);
	void DrawLineBresenham(int startx, int starty, int endx, int endy);

	void DrawCircle(int xc, int yc, int radius);
	void circlePlotPoints(int xc, int yc, Point pt);
	void DrawEllipse(int xc, int yc, int rx,int ry);
	void ellipsePlotPoints(int xc, int yc, Point pt);

	void LineFill(int s, int e,int valY);
	void FillActiveEdgeTable(const LinkList& ActiveEdgeTable);
	void UpdateActiveEdgeTable(LinkList& ActiveEdgeTable);
	void PolygonFill(std::vector<IntPoint> vec);

private:
	static Screen * currentInstance;
	double CUR_MULTIPLE;
	GLint NOW_WIDTH, NOW_HEIGHT;
	GLint NOW_MOUSE_X, NOW_MOUSE_Y,LINE_START_X,LINE_START_Y;
	GLdouble START_X, START_Y,STEP_WIDTH,STEP_HEIGHT;

	static const int MAXN = 30, MAXM = 30;
	static const int WINDOW_INITSIZE_WIDTH = 600, WINDOW_INITSIZE_HEIGHT = 600;
	GLfloat ScreenPixelColor[MAXM][MAXN][3];
	GLfloat TempPixelColor[MAXM][MAXN][3];

	Point ScreenPoint[MAXM+10][MAXN+10];

	std::vector<IntPoint> vec;
};

#endif // !__SCREEN__H__

