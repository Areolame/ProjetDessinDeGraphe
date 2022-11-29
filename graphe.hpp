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
	int PENALITE_MAX;
	Graphe(std::vector<Aretes> liens, std::vector<Emplacement> emplacementsPossibles) {
		_emplacementsPossibles = emplacementsPossibles;
		_liens = liens;
		PENALITE_MAX = _liens.size() * 10;
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

	void clearNodeEmplacement() {
		for (int i = 0; i < _noeuds.size(); i++) {
			_noeuds[i].clearEmplacement();
		}
	}

	void placementAleatoire()
	{
		std::cout << "taille: " << _noeuds.size() << std::endl;
		for (int i = 0; i < _noeuds.size(); ++i)
		{
			int emplacementId = generateRand(_emplacementsPossibles.size()) - 1;
			while (!_emplacementsPossibles[emplacementId].estDisponible()) {
				emplacementId = (emplacementId + 1) % _emplacementsPossibles.size();
			}
			_noeuds[i].setEmplacement(&_emplacementsPossibles[emplacementId]);
		}
	}

	long getNbCroisement()
	{
		int debug = 0;
		long total = 0;
		for (int i = 0; i < _liens.size()-1; ++i)
		{
			for (int j = i + 1; j < _liens.size(); ++j)
			{
				//Aretes aretes1 = _liens[i], aretes2 = _liens[j];
				if (!(_liens[i].contains(_liens[j].getNoeud1()) 
					|| _liens[i].contains(_liens[j].getNoeud2())))
				{
					if (seCroisent(_liens[i], _liens[j]))
					{
						if (surSegment(_liens[i], *_liens[j].getNoeud1())|| surSegment(_liens[i], *_liens[j].getNoeud2()))
						{
							total += PENALITE_MAX;
						}
						else {
							++total;
						}
					}
				}
				else {
					Noeud* nodeNotInCommon = _liens[j].nodeNotInCommon(_liens[i]);
					if (surSegment(_liens[i], *nodeNotInCommon))
					{
						total += PENALITE_MAX;
					}
				}
			}
		}
		return total;
	}


	long getNbCroisementGlouton()
	{
		int debug = 0;
		long total = 0;
		for (int i = 0; i < _liens.size() - 1; ++i)
		{
			if (_liens[i].estPlace()) {
				for (int j = i + 1; j < _liens.size(); ++j)
				{
					if (_liens[j].estPlace()) {
						//Aretes aretes1 = _liens[i], aretes2 = _liens[j];
						if (!(_liens[i].contains(_liens[j].getNoeud1())
							|| _liens[i].contains(_liens[j].getNoeud2())))
						{
							if (seCroisent(_liens[i], _liens[j]))
							{
								if (surSegment(_liens[i], *_liens[j].getNoeud1())
									|| surSegment(_liens[i], *_liens[j].getNoeud2()))
								{
									total += PENALITE_MAX;
								}
								else {
									++total;
								}
							}
						}
						else {
							Noeud* nodeNotInCommon = _liens[j].nodeNotInCommon(_liens[i]);
							if (surSegment(_liens[i], *nodeNotInCommon))
							{
								total += PENALITE_MAX;
							}
						}
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
			//std::cout << "t: " << t << std::endl;
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


	Emplacement* getEmplacementPlusProche(const Point& origin)
	{
		Emplacement meilleurEmplacement = _emplacementsPossibles[0];
		int meilleurDistance = meilleurEmplacement.getPosition().distance(origin);
		for (int i = 1; i < _emplacementsPossibles.size(); ++i)
		{
			int distanceActuel = _emplacementsPossibles[i].getPosition().distance(origin);
			if (meilleurDistance > distanceActuel)
			{
				meilleurEmplacement = _emplacementsPossibles[i];
				meilleurDistance = distanceActuel;
			}
		}

		return &meilleurEmplacement;
	}

	Emplacement* getEmplacementPlusProche(Emplacement* origin)
	{
		Emplacement meilleurEmplacement = _emplacementsPossibles[0];
		int meilleurDistance = meilleurEmplacement.getPosition()
			.distance(origin->getPosition());
		for (int i = 1; i < _emplacementsPossibles.size(); ++i)
		{
			int distanceActuel = _emplacementsPossibles[i].getPosition()
				.distance(origin->getPosition());
			if (origin->getId() != _emplacementsPossibles[i].getId() && meilleurDistance > distanceActuel)
			{
				meilleurEmplacement = _emplacementsPossibles[i];
				meilleurDistance = distanceActuel;
			}
		}

		return &meilleurEmplacement;
	}

	Point getCentreGravite()
	{
		double totalX = 0.0, totalY = 0.0;
		for (Emplacement emplacement : _emplacementsPossibles)
		{
			totalX += emplacement.getX();
			totalY += emplacement.getY();
		}

		return Point(
			totalX / _emplacementsPossibles.size(),
			totalY / _emplacementsPossibles.size()
		);
	}

	void glouton() {
		for (int i = 0; i < _noeuds.size(); i++) {
			// Selection aléatoire du noeud
			int randomId = generateRand(_noeuds.size() - 1);
			while (_noeuds[randomId].getEmplacement() != nullptr) {
				randomId = (randomId + 1) % _noeuds.size();
			}
			// Selection aléatoire d'un emplacement disponible (pas tres équiprobable)
			int randomEmpId = generateRand(_emplacementsPossibles.size() - 1);
			while (!_emplacementsPossibles[randomEmpId].estDisponible()) {
				randomEmpId = (randomEmpId + 1) % _emplacementsPossibles.size();
			}
			_noeuds[randomId].setEmplacement(&_emplacementsPossibles[randomEmpId]);
			long bestScore = getNbCroisementGlouton();
			int bestId = randomEmpId;
			int index = (randomEmpId + 1) % _emplacementsPossibles.size();
			for (int j = 1; j < _emplacementsPossibles.size(); j++) {
				while (!_emplacementsPossibles[index].estDisponible()) {
					index = (index + 1) % _emplacementsPossibles.size();
				}
				_noeuds[randomId].setEmplacement(&_emplacementsPossibles[index]);
				long newScore = getNbCroisementGlouton();
				if (newScore < bestScore) {
					bestScore = newScore;
					bestId = index;
				}
			}
			_noeuds[randomId].setEmplacement(&_emplacementsPossibles[bestId]);
		}
	}
	void gloutonRevisite() 
	{
		Point centreGravite = getCentreGravite();
		Emplacement *depart = getEmplacementPlusProche(centreGravite);
		vector<int> nbAretesParNoeud(_noeuds.size(), 0);

		//Creation d'un tableau de noeuds avec leur nb d'utilisation
		for (int i = 0; i < _liens.size(); ++i)
		{
			++nbAretesParNoeud[_liens[i].getNoeud1()->getId()];
			++nbAretesParNoeud[_liens[i].getNoeud2()->getId()];
		}

		//Trouve le noeud le plus utilise du graphe
		int idNoeudPlusUtilise = nbAretesParNoeud[0];
		for (int i = 1; i < nbAretesParNoeud.size(); ++i)
		{
			if (nbAretesParNoeud[idNoeudPlusUtilise] < nbAretesParNoeud[i])
			{
				idNoeudPlusUtilise = i;
			}
		}

		Noeud* noeudDepart = &_noeuds[idNoeudPlusUtilise];

		depart->setNoeud(noeudDepart);

		//Cherche dans les liens du noeud le plus utilise parmis eux
		int idSecondMeilleurNoeud = 0;
		while(idSecondMeilleurNoeud == idNoeudPlusUtilise)
		{
			++idSecondMeilleurNoeud;
		}
		for (int i = 0; i < _liens.size(); ++i)
		{
			if (_liens[i].contains(noeudDepart))
			{
				int idSecondNoeud = _liens[i].getAutreNoeud(noeudDepart)->getId();
				if (nbAretesParNoeud[idSecondMeilleurNoeud] >
					nbAretesParNoeud[idSecondNoeud])
				{
					idSecondMeilleurNoeud = idSecondNoeud;
				}
			}
		}

		Emplacement* emplacementProche = getEmplacementPlusProche(depart);

		emplacementProche->setNoeud(&_noeuds[idSecondMeilleurNoeud]);

		


	}


};

#endif