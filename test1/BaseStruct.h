#pragma once
struct Point
{
	double x, y;
	Point(double tx = 0, double ty = 0) {
		x = tx, y = ty;
	}
};

struct IntPoint
{
	int x, y;
	IntPoint(int tx = 0, int ty = 0) {
		x = tx, y = ty;
	}
};

struct Edge {
	IntPoint s, e;
	int deltax, deltay, tempsy, tempey;
	Edge(IntPoint ts = IntPoint(0, 0), IntPoint te = IntPoint(0, 0), int tdeltay = 0, int tdeltax = 0) {
		s = ts, e = te, deltax = tdeltax, deltay = tdeltay, tempsy = ts.y, tempey = te.y;
	}
};