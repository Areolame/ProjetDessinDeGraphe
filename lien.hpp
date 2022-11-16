#ifndef LIEN_HPP
#define LIEN_HPP

#include "noeud.hpp"

class Lien {
public:
	Noeud* _noeud1, *_noeud2;
	Lien(Noeud* noeud1, Noeud* noeud2) {
		_noeud1 = noeud1;
		_noeud2 = noeud2;
	}

	Noeud getNoeud(int number)  const
	{
		if (number == 1) return *_noeud1;
		return *_noeud2;
	}
	Noeud* getNoeud1()  const { return _noeud1; }
	Noeud* getNoeud2()  const { return _noeud2; }

	void setNoeud1(Noeud* noeud1) { _noeud1 = noeud1; }
	void setNoeud2(Noeud* noeud2) { _noeud2 = noeud2; }

	bool contains(Noeud* noeud)
	{
		return (_noeud1->getId() == noeud->getId() || _noeud2->getId() == noeud->getId());
	}

};

#endif