#ifndef NOEUD_HPP
#define NOEUD_HPP

#include "point.hpp"

class Noeud {
public:
	Point* _position;
	Noeud(Point* position) {
		_position = position;
	}
	Noeud(int* x, int* y) {
		_position = new Point(x, y);
	}

	Point getPosition()  const { return *_position; }
	int getX()  const { return _position->getX(); }
	int getY()  const { return _position->getY(); }

	void setPosition(int* x, int* y) { _position->setX(x); _position->setY(y); }
	void setPosition(Point* point) { _position = point; }
	void setX(int* x) { _position->setX(x); }
	void setY(int* y) { _position->setY(y); }

};


#endif