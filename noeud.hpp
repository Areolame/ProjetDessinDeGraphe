#ifndef NOEUD_HPP
#define NOEUD_HPP

#include "emplacement.hpp"

class Noeud {
public:
	Emplacement* _emplacement;
	int _id;
	Noeud(Emplacement* emplacement, int id) {
		_emplacement = emplacement;
		_id = id;
	}
	Noeud(int id) {
		_emplacement = nullptr;
		_id = id;
	}

	Emplacement getEmplacement()  const { return *_emplacement; }
	int getX()  const { return _emplacement->getX(); }
	int getY()  const { return _emplacement->getY(); }
	int getId() const { return _id; }

	void setEmplacement(Emplacement emplacement)
	{
		emplacement.setNoeud(this);
	}

};


#endif