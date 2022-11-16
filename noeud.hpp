#ifndef NOEUD_HPP
#define NOEUD_HPP

#include "emplacement.hpp"

class Noeud {
public:
	Emplacement* _emplacement = nullptr;
	int _id;
	Noeud(Emplacement* emplacement, int id) {
		_emplacement = emplacement;
		_id = id;
	}
	Noeud(int id) {
		_id = id;
	}

	Emplacement* getEmplacement()  const { return _emplacement; }
	int getX()  const { return _emplacement->getX(); }
	int getY()  const { return _emplacement->getY(); }
	Point getPosition() const { return _emplacement->getPosition(); }
	int getId() const { return _id; }

	void setEmplacement(Emplacement* emplacement)
	{
		_emplacement = emplacement;
		emplacement->setNoeud(this);
	}

};


#endif