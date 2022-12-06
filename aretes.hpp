#ifndef ARETES_HPP
#define ARETES_HPP

#include "noeud.hpp"

class Aretes {
public:
	Noeud* _noeud1, *_noeud2;
	Aretes(Noeud* noeud1, Noeud* noeud2) {
		_noeud1 = noeud1;
		_noeud2 = noeud2;

		noeud1->connect(noeud2);
		noeud2->connect(noeud1);
	}

	Noeud getNoeud(int number)  const
	{
		if (number == 1) return *_noeud1;
		return *_noeud2;
	}

	// A APPELER UNIQUEMENT SI LE LIEN A LE NOEUD EN PARAMETRE RENVOIE NULLPTR SINON
	Noeud* getAutreNoeud(Noeud* noeud)
	{
		if (_noeud1->getId() == noeud->getId()) return _noeud2;
		else if (_noeud2->getId() == noeud->getId()) return _noeud1;
		return nullptr;
	}
	Noeud* getNoeud1()  const { return _noeud1; }
	Noeud* getNoeud2()  const { return _noeud2; }

	void setNoeud1(Noeud* noeud1) { _noeud1 = noeud1; }
	void setNoeud2(Noeud* noeud2) { _noeud2 = noeud2; }

	bool contains(Noeud* noeud)
	{
		return (_noeud1->getId() == noeud->getId() || _noeud2->getId() == noeud->getId());
	}

	// A APPELER UNIQUEMENT SI LES LIENS ONT UN NOEUD EN COMMUN
	// Renvoie nullptr si aucun noeud en commun
	Noeud* nodeNotInCommon(Aretes l) {
		if (_noeud1->getId() == l.getNoeud1()->getId() || _noeud1->getId() == l.getNoeud2()->getId()) { return _noeud2; }
		return _noeud1;
	}

	bool estPlace() {
		return (_noeud1->getEmplacement() != nullptr && _noeud2->getEmplacement() != nullptr);
	}

};

#endif