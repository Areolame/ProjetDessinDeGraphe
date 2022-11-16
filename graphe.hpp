#ifndef GRAPHE_HPP
#define GRAPHE_HPP

#include <vector>
#include "aretes.hpp"
#include "point.hpp"
#include "intersection.hpp"
#include "emplacement.hpp"
#include "utilitaire.hpp"
#include <iostream>

class Graphe {
public:
	std::vector<Emplacement> _emplacementsPossibles;
	std::vector<Aretes> _liens;
	std::vector<Noeud> _noeuds;
	Graphe(std::vector<Aretes> liens, std::vector<Emplacement> emplacementsPossibles) {
		_emplacementsPossibles = emplacementsPossibles;
		_liens = liens;
	}

	Graphe()
	{}

	void afficherLiens() {
		std::cout << "Nb Liens: " << _liens.size() << std::endl;
		for (int i = 0; i < _liens.size(); i++) {
			if (_liens[i].getNoeud1() == nullptr || _liens[i].getNoeud2() == nullptr)
				std::cout << "scream" << std::endl;
			if (_liens[i].getNoeud1()->getEmplacement() == nullptr || _liens[i].getNoeud2()->getEmplacement() == nullptr)
				std::cout << "scream2" << std::endl;
			std::cout << "id: " << i << " x: " << _liens[i].getNoeud1()->getX() << " y: " << _liens[i].getNoeud1()->getY() << std::endl;
			std::cout << "id: " << i << " x: " << _liens[i].getNoeud1()->getX() << " y: " << _liens[i].getNoeud1()->getY() << std::endl;
		}
	}

	void afficherNoeuds() {
		std::cout << "Nb Noeuds: " << _noeuds.size() << std::endl;
		for (int i = 0; i < _noeuds.size(); i++) {
			if (_noeuds[i].getEmplacement() == nullptr)
				std::cout << "scream" << std::endl;
			std::cout << "id: " << i << " x: " << _noeuds[i].getX() << " y: " << _noeuds[i].getY() << std::endl;
		}
	}

	void afficherEmplacement() {
		std::cout << "Nb Emplacements: " << _emplacementsPossibles.size() << std::endl;
		for (int i = 0; i < _emplacementsPossibles.size(); i++) {
			std::cout << "id: " << i << " x: " << _emplacementsPossibles[i].getX() << " y: " << _emplacementsPossibles[i].getY() << std::endl;
		}
	}

	void placementAleatoire()
	{
		std::cout << "taille: " << _noeuds.size() << std::endl;
		for (int i = 0; i < _noeuds.size(); ++i)
		{
			int emplacementPossibleSize = _emplacementsPossibles.size();
			Emplacement* emplacement = nullptr;
			do
			{
				emplacement = &_emplacementsPossibles[generateRand(emplacementPossibleSize) - 1];
			} while ( !emplacement->estDisponible());
			_noeuds[i].setEmplacement(emplacement);
		}
	}

	long getNbCroisement()
	{
		long total = 0;
		for (int i = 0; i < _liens.size(); ++i)
		{
			for (int noeud = 0; noeud < _noeuds.size(); ++noeud)
			{
				if (surSegment(_liens[i], noeud))
				{
					total += INT_MAX;
				}
			}
			for (int j = 0; i < _liens.size(); ++j)
			{
				if (i != j)
				{
					Aretes aretes1 = _liens[i], aretes2 = _liens[j];
					if (!(aretes1.contains(aretes2.getNoeud1()) || aretes1.contains(aretes2.getNoeud2())))
					{
						if (seCroisent(aretes1, aretes2))
						{
							++total;
						}
					}
				}
			}
		}
		return total;
	}

};

#endif