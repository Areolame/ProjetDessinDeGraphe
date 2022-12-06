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
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Affichage DEBUG Aretes:" << std::endl;
		std::cout << "Nb Aretes: " << _liens.size() << std::endl;
		for (int i = 0; i < _liens.size(); i++) {
			if (_liens[i].getNoeud1() == nullptr || _liens[i].getNoeud2() == nullptr)
				std::cout << "scream" << std::endl;
			if (_liens[i].getNoeud1()->getEmplacement() == nullptr || _liens[i].getNoeud2()->getEmplacement() == nullptr)
				std::cout << "scream2" << std::endl;
			std::cout << "id: " << i << " idnode1: " << _liens[i].getNoeud1()->getId() << " idnode2: " << _liens[i].getNoeud2()->getId();
			std::cout << " x1: " << _liens[i].getNoeud1()->getX() << " y1: " << _liens[i].getNoeud1()->getY();
			std::cout << " x2: " << _liens[i].getNoeud2()->getX() << " y2: " << _liens[i].getNoeud2()->getY() << std::endl;
		}
		std::cout << "-----------------------------------------------" << std::endl;
	}

	void afficherNoeuds() {
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Affichage DEBUG Noeuds:" << std::endl;
		std::cout << "Nb Noeuds: " << _noeuds.size() << std::endl;
		for (int i = 0; i < _noeuds.size(); i++) {
			if (_noeuds[i].getEmplacement() == nullptr)
				std::cout << "scream" << std::endl;
			std::cout << "id: " << _noeuds[i].getId() << " x: " << _noeuds[i].getX() << " y: " << _noeuds[i].getY() << " empid: ";
			if (_noeuds[i].getEmplacement() == nullptr) {
				std::cout << " aucun" << std::endl;
			}
			else {
				std::cout << _noeuds[i].getEmplacement()->_id << std::endl;
			}
		}
		std::cout << "-----------------------------------------------" << std::endl;
	}

	void afficherEmplacement() {
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Affichage DEBUG Emplacements:" << std::endl;
		std::cout << "Nb Emplacements: " << _emplacementsPossibles.size() << std::endl;
		for (int i = 0; i < _emplacementsPossibles.size(); i++) {
			std::cout << "id: " << _emplacementsPossibles[i]._id << " x: " << _emplacementsPossibles[i].getX() << " y: " << _emplacementsPossibles[i].getY() << " idnode: ";
			if (_emplacementsPossibles[i]._noeud == nullptr) {
				std::cout << "aucun: " << _emplacementsPossibles[i].estDisponible() << std::endl;
			}
			else {
				std::cout << _emplacementsPossibles[i]._noeud->getId() << std::endl;
			}
		}
		std::cout << "-----------------------------------------------" << std::endl;
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

			//Verification des points isoles
			for (int j = 0; j < _noeuds.size(); ++j)
			{
				if (_noeuds[j].getVoisins().size() == 0)
				{
					if (surSegment(_liens[i], _noeuds[j]))
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
				//Verification des points isoles
				for (int  j = 0; j < _noeuds.size(); ++j)
				{
					if (_noeuds[j].getVoisins().size() == 0)
					{
						if (surSegment(_liens[i], _noeuds[j]))
						{
							total += PENALITE_MAX;
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

	bool brancheTermine(Noeud* origin, std::vector<int>& idNoeudPasses)
	{
		for (int id : idNoeudPasses)
		{
			if (id == origin->getId()) return true;
		}
		idNoeudPasses.push_back(origin->getId());
		for (Noeud* noeud : origin->getVoisins())
		{
			if (!noeud->estPlace() || brancheTermine(noeud, idNoeudPasses))
			{
				return false;
			}
		}
		return true;
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
		int idNoeud = 0;
		int nbRencontre = 0;
		for (int i = 1; i < _noeuds.size(); ++i)
		{
			if (!_noeuds[i].estPlace() &&
				_noeuds[i].getVoisins().size() > _noeuds[idNoeud].getVoisins().size())
			{
				idNoeud = i;
				nbRencontre = 1;
			}
			else if (!_noeuds[i].estPlace() &&
				_noeuds[i].getVoisins().size() == _noeuds[idNoeud].getVoisins().size())
			{
				++nbRencontre;
				int aleatoire = generateRand(nbRencontre);
				if (aleatoire == 1)
				{
					idNoeud = i;
				}
			}
		}

		Point centreGravite = getCentreGravite();
		_noeuds[idNoeud].setEmplacement(getEmplacementPlusProche(centreGravite));

		while (!estPlace())
		{
			int meilleurVoisinPlace = 0;
			Noeud* meilleurVoisin = nullptr;
			for (int i = 0; i < _noeuds.size(); ++i)
			{
				if (!_noeuds[i].estPlace())
				{
					int nbVoisinPlace = 0;
					//Compte le nombre de voisin placés
					for (int j = 0; j < _noeuds[i].getVoisins().size(); ++j)
					{
						if (_noeuds[i].getVoisins()[j]->getEmplacement() != nullptr)
						{
							++nbVoisinPlace;
						}
					}

					if (nbVoisinPlace > meilleurVoisinPlace)
					{
						nbVoisinPlace = meilleurVoisinPlace;
						meilleurVoisin = &_noeuds[i];
					}
				}

			}

			if (meilleurVoisin != nullptr)
			{
				int randomEmpId = generateRand(_emplacementsPossibles.size() - 1);
				while (!_emplacementsPossibles[randomEmpId].estDisponible()) {
					randomEmpId = (randomEmpId + 1) % _emplacementsPossibles.size();
				}
				meilleurVoisin->setEmplacement(&_emplacementsPossibles[randomEmpId]);
				long bestScore = getNbCroisementGlouton();
				int bestId = randomEmpId;
				int index = (randomEmpId + 1) % _emplacementsPossibles.size();
				for (int j = 0; j < _emplacementsPossibles.size(); j++) {
					while (!_emplacementsPossibles[index].estDisponible()) {
						index = (index + 1) % _emplacementsPossibles.size();
					}
					meilleurVoisin->setEmplacement(&_emplacementsPossibles[index]);
					long newScore = getNbCroisementGlouton();
					if (newScore < bestScore) {
						bestScore = newScore;
						bestId = index;
					}
				}
				meilleurVoisin->setEmplacement(&_emplacementsPossibles[bestId]);
			}

		}
		
	}


	bool estPlace()
	{
		for (Noeud noeud : _noeuds)
		{
			if (!noeud.estPlace()) return false;
		}
		return true;
	}


};

#endif