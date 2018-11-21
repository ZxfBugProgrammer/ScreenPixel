#ifndef __SCREEN__H__
#define __SCREEN__H__

#include <GL/glut.h>

class Screen
{
public:

	Screen();

	void SetPixel(int x, int y, double cr, double cg, double cb);
	void GetPixel(int x, int y, double &cr, double &cg, double &cb);

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

private:
	static Screen * currentInstance;
	double CUR_MULTIPLE;
	GLint NOW_WIDTH, NOW_HEIGHT;
	GLint NOW_MOUSE_X, NOW_MOUSE_Y,LINE_START_X,LINE_START_Y;
	GLdouble START_X, START_Y,STEP_WIDTH,STEP_HEIGHT;

	static const int MAXN = 80, MAXM = 80;
	static const int WINDOW_INITSIZE_WIDTH = 600, WINDOW_INITSIZE_HEIGHT = 600;
	GLfloat ScreenPixelColor[MAXM][MAXN][3];
	GLfloat TempPixelColor[MAXN][MAXN][3];
};

#endif // !__SCREEN__H__

