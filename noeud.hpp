#ifndef NOEUD_HPP
#define NOEUD_HPP

#include "emplacement.hpp"

class Noeud {
public:
	Emplacement* _emplacement = nullptr;
	int _id;
	std::vector<Noeud*> voisins;
	Noeud(Emplacement* emplacement, int id) {
		_emplacement = emplacement;
		_id = id;
	}
	Noeud(int id) {
		_id = id;
	}

	bool estPlace() const { return _emplacement != nullptr; }
	Emplacement* getEmplacement()  const { return _emplacement; }
	void connect(Noeud* noeud) { voisins.push_back(noeud); };
	std::vector<Noeud*> getVoisins() const { return voisins; }
	bool voisinsSontPlaces() const {
		for (Noeud* noeud : voisins)
		{
			if (!noeud->estPlace()) return false;
		} 
		return true;
	}
	int getX()  const { return _emplacement->getX(); }
	int getY()  const { return _emplacement->getY(); }
	Point getPosition() const { return _emplacement->getPosition(); }
	int getId() const { return _id; }

	void setEmplacement(Emplacement* emplacement)
	{
		if (_emplacement != nullptr) {
			_emplacement->removeNoeud();
		}
		_emplacement = emplacement;
		emplacement->setNoeud(this);
	}

	void clearEmplacement() {
		if (_emplacement != nullptr) {
			_emplacement->removeNoeud();
		}
		_emplacement = nullptr;
	}

};


#endif