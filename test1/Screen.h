#ifndef __SCREEN__H__
#define __SCREEN__H__

#include <GL/glut.h>

class Screen
{
public:

	Screen();

	void SetPixel(int x, int y, double cr, double cg, double cb);

	void InitGlut();

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

	void SpecialKeyBoard(int key, int x, int y);
	static void SpecialKeyBoardCallback(int key, int x, int y);
	void SetupSpecialKeyBoardCallback();

	void DrawWindow(int argc, char** argv);
private:
	static Screen * currentInstance;
	double CUR_MULTIPLE;
	GLint NOW_WIDTH, NOW_HEIGHT;
	GLdouble START_X, START_Y,STEP_WIDTH,STEP_HEIGHT;

	static const int MAXN = 50, MAXM = 50;
	static const int WINDOW_INITSIZE_WIDTH = 600, WINDOW_INITSIZE_HEIGHT = 600;
	GLfloat ScreenPixelColor[MAXM][MAXN][3];
};

#endif // !__PIXEL__H__

