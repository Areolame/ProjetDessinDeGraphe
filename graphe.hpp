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
			} while (!emplacement->estDisponible());
			_noeuds[i].setEmplacement(emplacement);
		}
	}

	long getNbCroisement()
	{
		long total = 0;
		for (int i = 0; i < _liens.size(); ++i)
		{
			for (int j = i + 1; j < _liens.size(); ++j)
			{
				Aretes aretes1 = _liens[i], aretes2 = _liens[j];
				if (!(aretes1.contains(aretes2.getNoeud1()) || aretes1.contains(aretes2.getNoeud2())))
				{
					if (seCroisent(aretes1, aretes2))
					{
						++total;
						if (surSegment(_liens[i], *_liens[j].getNoeud1())|| surSegment(_liens[i], *_liens[j].getNoeud2()))
						{
							total += INT_MAX;
						}
					}
				}
				else {
					if (surSegment(_liens[i], *_liens[j].getNoeud1()) || surSegment(_liens[i], *_liens[j].getNoeud2()))
					{
						total += INT_MAX;
					}
				}
			}
		}
		return total;
	}

	// Lance l'algorithme de recuit simulé sur le graphe pour minimiser le nombre d'intersection
	void recuitSimule() {
		double cool = 0.99999;
		double t = 100;
		int nbImprove = 0;
		int nbSame = 0;
		int nbWorst = 0;
		long nbIntersection = getNbCroisement(); // Remplacer par la fonction qui calcule le score d'intersection
		for (int iter = 0; (t > 0.0001) && (nbIntersection > 0); iter++) {
			// Selection aléatoire du noeud
			int randomId = generateRand(_noeuds.size() - 1);
			// Selection aléatoire d'un emplacement disponible (pas tres équiprobable)
			int randomEmpId = generateRand(_emplacementsPossibles.size() - 1);
			while (!_emplacementsPossibles[randomEmpId].estDisponible()) {
				randomEmpId = (randomEmpId + 1) % _emplacementsPossibles.size();
			}
			Emplacement* oldEmplacement = _noeuds[randomId].getEmplacement();
			_noeuds[randomId].setEmplacement(&_emplacementsPossibles[randomEmpId]);
			long newNbIntersection = getNbCroisement(); // Remplacer par la fonction qui calcule le score d'intersection
			// improve < 0 = amélioration, improve > 0 = moin bien, improve = 0 = pareil
			int improve = newNbIntersection - nbIntersection;
			if (improve < 0) {
				nbIntersection = newNbIntersection;
				nbImprove++;
			}
			else {
				double randDouble = generateDoubleRand(1.0);
				if (randDouble < exp(-improve / t)) {
					if (improve == 0) {
						nbSame++;
					}
					else {
						nbWorst++;
					}
				}
				else {
					_noeuds[randomId].setEmplacement(oldEmplacement);
				}
			}
			t *= cool;
		}
	}

};

#endif