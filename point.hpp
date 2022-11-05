#ifndef POINT_HPP
#define POINT_HPP

class Point {
public:
	int* _y;
	int* _x;
	Point(int* x, int* y) {
		_x = x;
		_y = y;
	}

	int getX() const { return *_x; }
	int getY() const { return *_y; }

	void setX(int* x) { _x = x; }
	void setY(int* y) { _y = y; }

};

#endif