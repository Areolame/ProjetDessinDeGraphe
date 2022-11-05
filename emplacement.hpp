#ifndef EMPLACEMENT_HPP
#define EMPLACEMENT_HPP

#include "point.hpp"
#include "noeud.hpp"

class Emplacement {
public:
	Point* _position;
	int _id = -1;
	bool _estDisponible = true;

	Emplacement(Point* position) {
		_position = position;
	}

	Point getPosition() const { return *_position; }
	int getX() const { return _position->getX(); }
	int getY() const { return _position->getX(); }
	bool estDisponible() { return _estDisponible; }

	void setPosition(Point* position) { _position = position; }
	void setX(int* x) { _position->setX(x); }
	void setY(int* y) { _position->setY(y); }
	void setNoeud(Noeud* noeud)
	{
		_id = noeud->getId();
		estDisponible = false;
	}
	void removeNoeud()
	{
		_id = -1;
		estDisponible = true;
	}

};

#endif