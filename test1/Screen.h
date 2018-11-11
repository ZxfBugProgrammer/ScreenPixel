#ifndef __SCREEN__H__
#define __SCREEN__H__

#include <GL/glut.h>
#include "Constant.h"

class Screen
{
public:
	GLfloat ScreenPixelColor[MAXM][MAXN][3];

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


	void DrawWindow(int argc, char** argv);
private:
	static Screen * currentInstance;
	double CUR_MULTIPLE;
};

#endif // !__PIXEL__H__

