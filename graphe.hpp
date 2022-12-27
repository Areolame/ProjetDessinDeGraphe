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
	int PENALITE_MAX = 10000;

	Graphe()
	{}

	void afficherLiens() {
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Affichage DEBUG Aretes:" << std::endl;
		std::cout << "Nb Aretes: " << _liens.size() << std::endl;
		for (int i = 0; i < _liens.size(); i++) {
			std::cout << "id: " << _liens[i]._id << " idnode1: " << _liens[i].getNoeud1()->getId() << " idnode2: " << _liens[i].getNoeud2()->getId();
			std::cout << " x1: " << _liens[i].getNoeud1()->getX() << " y1: " << _liens[i].getNoeud1()->getY();
			std::cout << " x2: " << _liens[i].getNoeud2()->getX() << " y2: " << _liens[i].getNoeud2()->getY();
			std::cout << std::endl;
		}
		std::cout << "-----------------------------------------------" << std::endl;
	}

	void afficherNoeuds() {
		std::cout << "-----------------------------------------------" << std::endl;
		std::cout << "Affichage DEBUG Noeuds:" << std::endl;
		std::cout << "Nb Noeuds: " << _noeuds.size() << std::endl;
		for (int i = 0; i < _noeuds.size(); i++) {
			std::cout << "id: " << _noeuds[i].getId() << " empid: ";
			if (_noeuds[i].getEmplacement() == nullptr) {
				std::cout << " aucun" << std::endl;
			}
			else {
				std::cout << _noeuds[i].getEmplacement()->_id << " x: " << _noeuds[i].getX() << " y: " << _noeuds[i].getY() << std::endl;
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
		std::cout << "Placement aleatoire" << std::endl;
		for (int i = 0; i < _noeuds.size(); ++i)
		{
			int emplacementId = generateRand(_emplacementsPossibles.size()) - 1;
			while (!_emplacementsPossibles[emplacementId].estDisponible()) {
				emplacementId = (emplacementId + 1) % _emplacementsPossibles.size();
			}
			_noeuds[i].setEmplacement(&_emplacementsPossibles[emplacementId]);
		}
	}

	std::vector<Noeud> noeudsNonPlaces()
	{
		std::vector<Noeud> noeudsNonPlaces;
		for (Noeud noeud : _noeuds)
		{
			noeudsNonPlaces.push_back(noeud);
		}
		return noeudsNonPlaces;
	}

	void placementNoeudAleatoire(int idNoeud)
	{
		int emplacementId = generateRand(_emplacementsPossibles.size()) - 1;
		while (!_emplacementsPossibles[emplacementId].estDisponible()) {
			emplacementId = (emplacementId + 1) % _emplacementsPossibles.size();
		}
		_noeuds[idNoeud].setEmplacement(&_emplacementsPossibles[emplacementId]);
	}
	long getNbCroisement()
	{
		int debug = 0;
		long total = 0;
		for (int i = 0; i < _liens.size() - 1; ++i)
		{
			for (int j = i + 1; j < _liens.size(); ++j)
			{
				//Aretes aretes1 = _liens[i], aretes2 = _liens[j];
				if (!(_liens[i].contains(_liens[j].getNoeud1())
					|| _liens[i].contains(_liens[j].getNoeud2())))
				{
					if (seCroisent(_liens[i], _liens[j]))
					{
						if (surSegment(_liens[i], *_liens[j].getNoeud1()) || surSegment(_liens[i], *_liens[j].getNoeud2()))
						{
							total += PENALITE_MAX;
						}
						else if (surSegment(_liens[j], *_liens[i].getNoeud1()) || surSegment(_liens[j], *_liens[i].getNoeud2())) {
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
					else {
						nodeNotInCommon = _liens[i].nodeNotInCommon(_liens[j]);
						if (surSegment(_liens[j], *nodeNotInCommon)) {
							total += PENALITE_MAX;
						}
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
		}
		return total;
	}

	// Lance l'algorithme de recuit simulé sur le graphe pour minimiser le nombre d'intersection
	void recuitSimule(double cool = 0.99999, double t = 100) {
		int nbImprove = 0;
		int nbSame = 0;
		int nbWorst = 0;
		std::vector<int> bestResult = saveCopy();
		int nbCroisement = getNbCroisement();
		std::cout << "Nb Croisement avant recuit: " << nbCroisement << std::endl;
		for (int iter = 0; t > 0.0001 && nbCroisement > 0; iter++) {
			//if (iter % 100000 == 0) {
			//	std::cout << "Iter: " << iter << " t: " << t << " intersections: " << newNbIntersection << std::endl;
			//}
			int randomId = generateRand(_noeuds.size() - 1); // Selection aléatoire du noeud
			int randomEmpId = generateRand(_emplacementsPossibles.size() - 1); // Selection aléatoire d'un emplacement disponible (pas tres équiprobable)
			// on retire si on pioche le meme emplacement
			while (_noeuds[randomId].getEmplacement()->getId() == randomEmpId) {
				randomEmpId = generateRand(_emplacementsPossibles.size() - 1);
			}
			int scoreNode;
			bool swapped = false;
			int idSwappedNode = -1;
			Emplacement* oldEmplacement = _noeuds[randomId].getEmplacement();
			if (!_emplacementsPossibles[randomEmpId].estDisponible()) {
				idSwappedNode = _emplacementsPossibles[randomEmpId]._noeud->getId();
				scoreNode = getScoreCroisementNode(randomId, idSwappedNode);
				scoreNode += getScoreCroisementNode(idSwappedNode);
				_noeuds[randomId].swap(&_emplacementsPossibles[randomEmpId]);
				swapped = true;
			}
			else {
				scoreNode = getScoreCroisementNode(randomId);
				_noeuds[randomId].setEmplacement(&_emplacementsPossibles[randomEmpId]);
			}
			int newScoreNode;
			if (swapped) {
				newScoreNode = getScoreCroisementNode(randomId, idSwappedNode);
				newScoreNode += getScoreCroisementNode(idSwappedNode);
			}
			else {
				newScoreNode = getScoreCroisementNode(randomId);
			}
			int improve = newScoreNode - scoreNode;
			if (improve < 0) {
				bestResult = saveCopy();
				nbCroisement += improve;
			}
			else {
				double randDouble = generateDoubleRand(1.0);
				if (randDouble >= exp(-improve / t)) {
					if (swapped) {
						_noeuds[randomId].swap(oldEmplacement);
					}
					else {
						_noeuds[randomId].setEmplacement(oldEmplacement);
					}
				}
				else {
					nbCroisement += improve;
				}
			}
			t *= cool;
		}
		loadCopy(bestResult);
		std::cout << "Meilleur resultat du recuit: " << nbCroisement << std::endl;
	}

	void rerecuitSimule(int iter = 10) {
		std::cout << "Starting Rerecuit " << iter << " iterations." << std::endl;
		double cool = 0.99999, t = 100, coolt = 0.99;
		for (int i = 0; i < iter; i++) {
			std::cout << "Starting Recuit Number: " << i << " t: " << t << " cool " << cool << std::endl;
			recuitSimule(cool, t);
			t *= coolt;
		}
	}

	void bestDeplacement() {
		long nbIntersection = getNbCroisement();
		int bestImprove = 0;
		int indexNoeud = -1, indexEmplacement = -1;
		do {
			bestImprove = 0;
			for (int i = 0; i < _noeuds.size(); i++) {
				for (int j = 0; j < _emplacementsPossibles.size(); j++) {
					if (_noeuds[i].getEmplacement()->getId() != j) {
						int score, newScore, swappedId;
						bool swapped = false;
						Emplacement* oldEmplacement = _noeuds[i].getEmplacement();
						if (_emplacementsPossibles[j].estDisponible()) {
							score = getScoreCroisementNode(i);
							_noeuds[i].setEmplacement(&_emplacementsPossibles[j]);
						}
						else {
							swappedId = _emplacementsPossibles[j]._noeud->getId();
							score = getScoreCroisementNode(i);
							score += getScoreCroisementNode(swappedId, i);
							_noeuds[i].swap(&_emplacementsPossibles[j]);
							swapped = true;
						}
						int NbInterReelNew = getNbCroisement();
						if (!swapped) {
							newScore = getScoreCroisementNode(i);
							_noeuds[i].setEmplacement(oldEmplacement);
						}
						else {
							newScore = getScoreCroisementNode(i);
							newScore += getScoreCroisementNode(swappedId, i);
							_noeuds[i].swap(oldEmplacement);
						}
						int improve = newScore - score;
						if (improve < bestImprove) {
							bestImprove = improve;
							indexNoeud = i;
							indexEmplacement = j;
						}
						if (NbInterReelNew != nbIntersection + improve) {
							return;
						}
					}
				}
			}
			if (bestImprove < 0) {
				if (_emplacementsPossibles[indexEmplacement].estDisponible()) {
					_noeuds[indexNoeud].setEmplacement(&_emplacementsPossibles[indexEmplacement]);
				}
				else {
					_noeuds[indexNoeud].swap(&_emplacementsPossibles[indexEmplacement]);
				}
				nbIntersection += bestImprove;
			}
		} while (bestImprove < 0);
		std::cout << "Meilleur resultat de l'algo meilleur deplacement: " << nbIntersection << std::endl;
	}


	Emplacement* getEmplacementPlusProche(const Point& origin)
	{
		Emplacement* meilleurEmplacement  = &_emplacementsPossibles[generateRand(_emplacementsPossibles.size() - 1)];
		while (!meilleurEmplacement->estDisponible()) {
			meilleurEmplacement = &_emplacementsPossibles[(meilleurEmplacement->getId() + 1) % _emplacementsPossibles.size()];
		}
		int meilleurDistance = meilleurEmplacement->getPosition().distance(origin);
		int nbRencontre = 0;
		for (int i = 0; i < _emplacementsPossibles.size(); ++i)
		{
			if (_emplacementsPossibles[i].estDisponible() && i != meilleurEmplacement -> getId())
			{
				int distanceActuel = _emplacementsPossibles[i].getPosition().distance(origin);
				if (meilleurDistance > distanceActuel)
				{
					meilleurEmplacement = &_emplacementsPossibles[i];
					meilleurDistance = distanceActuel;
					nbRencontre = 0;
				}
				else if (meilleurDistance == distanceActuel)
				{
					++nbRencontre;
					int aleatoire = generateRand(nbRencontre);
					if (aleatoire == 1)
					{
						meilleurEmplacement = &_emplacementsPossibles[i];
						meilleurDistance = distanceActuel;
					}
				}
			}

		}

		return meilleurEmplacement;
	}

	Emplacement* getEmplacementPlusProche(Emplacement* origin)
	{
		Emplacement meilleurEmplacement = _emplacementsPossibles[0];
		int meilleurDistance = meilleurEmplacement.getPosition()
			.distance(origin->getPosition());
		int nbRencontre = 0;
		for (int i = 1; i < _emplacementsPossibles.size(); ++i)
		{
			int distanceActuel = _emplacementsPossibles[i].getPosition()
				.distance(origin->getPosition());
			if (origin->getId() != _emplacementsPossibles[i].getId() && meilleurDistance > distanceActuel)
			{
				meilleurEmplacement = _emplacementsPossibles[i];
				meilleurDistance = distanceActuel;
				nbRencontre = 0;
			}
			else if (meilleurDistance == distanceActuel)
			{
				++nbRencontre;
				int aleatoire = generateRand(nbRencontre);
				if (aleatoire == 1)
				{
					meilleurEmplacement = _emplacementsPossibles[i];
					meilleurDistance = distanceActuel;
				}
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

	Point getCentreGraviteNoeudPlaces()
	{
		double totalX = 0.0, totalY = 0.0;
		for (Noeud noeud : _noeuds)
		{
			if (noeud.estPlace())
			{
				totalX += noeud.getX();
				totalY += noeud.getY();
			}
		}

		return Point(
			totalX / _emplacementsPossibles.size(),
			totalY / _emplacementsPossibles.size()
		);
	}

	Point getCentreGraviteVoisin(Noeud* noeud)
	{
		double totalX = 0.0, totalY = 0.0;
		for (Noeud* noeudParcouru : noeud->getVoisins())
		{
			if (noeudParcouru->estPlace())
			{
				totalX += noeudParcouru->getX();
				totalY += noeudParcouru->getY();
			}
		}

		return Point(
			totalX / _emplacementsPossibles.size(),
			totalY / _emplacementsPossibles.size()
		);
	}

	Emplacement* getMeilleurEmplacement(Noeud& meilleurNoeud)
	{
		Emplacement* oldEmplacement = meilleurNoeud.getEmplacement();
		int nbRencontre = 0;
		int randomEmpId = generateRand(_emplacementsPossibles.size() - 1);
		while (!_emplacementsPossibles[randomEmpId].estDisponible()) {
			randomEmpId = (randomEmpId + 1) % _emplacementsPossibles.size();
		}
		meilleurNoeud.setEmplacement(&_emplacementsPossibles[randomEmpId]);
		long bestScore = getNbCroisementGlouton();
		int bestId = randomEmpId;
		int index = (randomEmpId + 1) % _emplacementsPossibles.size();
		if (emplacementRestant())
		{
			for (int j = 0; j < _emplacementsPossibles.size(); j++) {
				while (!_emplacementsPossibles[index].estDisponible()) {
					std::cout << "Emplacement 2\n";
					index = (index + 1) % _emplacementsPossibles.size();
				}
				meilleurNoeud.setEmplacement(&_emplacementsPossibles[index]);
				long newScore = getNbCroisementGlouton();
				if (newScore < bestScore) {
					bestScore = newScore;
					bestId = index;
				}
				else if (newScore == bestScore)
				{
					++nbRencontre;
					int aleatoire = generateRand(nbRencontre);
					if (aleatoire == 1)
					{
						bestScore = newScore;
						bestId = index;
					}
				}
			}
		}
		if (oldEmplacement == nullptr)
		{
			meilleurNoeud.clearEmplacement();
		}
		else
		{
			meilleurNoeud.setEmplacement(oldEmplacement);
		}
		return &_emplacementsPossibles[bestId];
	}
	Emplacement* getMeilleurEmplacementGravite(Noeud* meilleurNoeud, Point gravite)
	{
		Emplacement* oldEmplacement = meilleurNoeud->getEmplacement();
		int nbRencontre = 0;
		int randomEmpId = generateRand(_emplacementsPossibles.size() - 1);
		while (!_emplacementsPossibles[randomEmpId].estDisponible()) {
			randomEmpId = (randomEmpId + 1) % _emplacementsPossibles.size();
		}
		meilleurNoeud->setEmplacement(&_emplacementsPossibles[randomEmpId]);
		long bestScore = getNbCroisementGlouton();
		int bestId = randomEmpId;
		int index = (randomEmpId + 1) % _emplacementsPossibles.size();
		if (emplacementRestant())
		{
			for (int j = 0; j < _emplacementsPossibles.size(); j++) {
				while (!_emplacementsPossibles[index].estDisponible()) {
					index = (index + 1) % _emplacementsPossibles.size();
				}
				meilleurNoeud->setEmplacement(&_emplacementsPossibles[index]);
				long newScore = getNbCroisementGlouton();
				if (newScore < bestScore) {
					bestScore = newScore;
					bestId = index;
				}
				else if (newScore == bestScore && 
					_emplacementsPossibles[bestId].getPosition().distance(gravite) > 
					_emplacementsPossibles[index].getPosition().distance(gravite))
				{
					bestScore = newScore;
					bestId = index;
				}
			}
		}
		if (oldEmplacement == nullptr)
		{
			meilleurNoeud->clearEmplacement();
		}
		else
		{
			meilleurNoeud->setEmplacement(oldEmplacement);
		}
		return &_emplacementsPossibles[bestId];
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
		Noeud* noeud = nullptr;
		int nbRencontre = 0;
		for (int i = 0; i < _noeuds.size(); ++i)
		{
			if (noeud == nullptr)
			{
				noeud = &_noeuds[i];
			}
			else if (!_noeuds[i].estPlace() &&
				_noeuds[i].getVoisins().size() > noeud->getVoisins().size())
			{
				noeud = &_noeuds[i];
				nbRencontre = 1;
			}
			else if (!_noeuds[i].estPlace() &&
				_noeuds[i].getVoisins().size() == noeud->getVoisins().size())
			{
				++nbRencontre;
				int aleatoire = generateRand(nbRencontre);
				if (aleatoire == 1)
				{
					noeud = &_noeuds[i];
				}
			}
		}

		Point centreGravite = getCentreGravite();
		noeud->setEmplacement(getEmplacementPlusProche(centreGravite));

		completeBasicGlouton();

	}
	void gloutonRevisiteGravite()
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
			Noeud* meilleurNoeud = nullptr;
			nbRencontre = 0;
			for (int i = 0; i < _noeuds.size(); ++i)
			{
				if (!_noeuds[i].estPlace())
				{
					Noeud* currentVoisin = &_noeuds[i];
					if (meilleurNoeud == nullptr)
					{
						meilleurNoeud = currentVoisin;
					}
					else if (_noeuds[i].getVoisinsPlaces() > _noeuds[idNoeud].getVoisinsPlaces())
					{
						idNoeud = i;
						nbRencontre = 1;
					}
					else if (_noeuds[i].getVoisinsPlaces() == _noeuds[idNoeud].getVoisinsPlaces())
					{
						if (_noeuds[i].getVoisins().size() > _noeuds[idNoeud].getVoisins().size())
						{
							idNoeud = i;
							nbRencontre = 1;
						}
						else if (_noeuds[i].getVoisins().size() == _noeuds[idNoeud].getVoisins().size())
						{
							++nbRencontre;
							int aleatoire = generateRand(nbRencontre);
							if (aleatoire == 1)
							{
								idNoeud = i;
							}
						}
					}
				}
			}

			if (meilleurNoeud != nullptr)
			{
				centreGravite = getCentreGraviteNoeudPlaces();
				Emplacement* emplacement = getMeilleurEmplacementGravite(meilleurNoeud, centreGravite);
				meilleurNoeud->setEmplacement(emplacement);
			}

		}

	}
	void gloutonRevisiteVoisin()
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
			Noeud* meilleurNoeud = nullptr;
			nbRencontre = 0;
			for (int i = 0; i < _noeuds.size(); ++i)
			{
				if (!_noeuds[i].estPlace())
				{
					Noeud* currentVoisin = &_noeuds[i];
					if (meilleurNoeud == nullptr)
					{
						meilleurNoeud = currentVoisin;
					}
					else if (_noeuds[i].getVoisinsPlaces() > _noeuds[idNoeud].getVoisinsPlaces())
					{
						idNoeud = i;
						nbRencontre = 1;
					}
					else if (_noeuds[i].getVoisinsPlaces() == _noeuds[idNoeud].getVoisinsPlaces())
					{
						if (_noeuds[i].getVoisins().size() > _noeuds[idNoeud].getVoisins().size())
						{
							idNoeud = i;
							nbRencontre = 1;
						}
						else if (_noeuds[i].getVoisins().size() == _noeuds[idNoeud].getVoisins().size())
						{
							++nbRencontre;
							int aleatoire = generateRand(nbRencontre);
							if (aleatoire == 1)
							{
								idNoeud = i;
							}
						}
					}
				}
			}

			if (meilleurNoeud != nullptr)
			{
				centreGravite = getCentreGraviteVoisin(meilleurNoeud);
				Emplacement* emplacement = getMeilleurEmplacementGravite(meilleurNoeud, centreGravite);
				meilleurNoeud->setEmplacement(emplacement);
			}

		}

	}
	void completeBasicGlouton()
	{
		while (!estPlace())
		{

			Noeud* meilleurNoeud = nullptr;
			int nbRencontre = 0;
			for (int i = 0; i < _noeuds.size(); ++i)
			{
				if (!_noeuds[i].estPlace())
				{
					Noeud* currentVoisin = &_noeuds[i];
					if (meilleurNoeud == nullptr)
					{
						meilleurNoeud = currentVoisin;
					}
					else if (_noeuds[i].getVoisinsPlaces() > meilleurNoeud->getVoisinsPlaces())
					{
						meilleurNoeud = &_noeuds[i];
						nbRencontre = 1;
					}
					else if (_noeuds[i].getVoisinsPlaces() == meilleurNoeud->getVoisinsPlaces())
					{
						if (_noeuds[i].getVoisins().size() > meilleurNoeud->getVoisins().size())
						{
							meilleurNoeud = &_noeuds[i];
							nbRencontre = 1;
						}
						else if (_noeuds[i].getVoisins().size() == meilleurNoeud->getVoisins().size())
						{
							++nbRencontre;
							int aleatoire = generateRand(nbRencontre);
							if (aleatoire == 1)
							{
								meilleurNoeud = &_noeuds[i];
							}
						}
					}
				}
			}

			if (meilleurNoeud != nullptr)
			{
				meilleurNoeud->setEmplacement(getMeilleurEmplacement(*meilleurNoeud));
			}

		}
	}

	bool emplacementRestant()
	{
		for (int i = 0; i < _emplacementsPossibles.size(); ++i)
		{
			if (_emplacementsPossibles[i].estDisponible())
			{
				return true;
			}
		}
		return false;
	}


	bool estPlace()
	{
		for (Noeud noeud : _noeuds)
		{
			if (!noeud.estPlace()) return false;
		}
		return true;
	}

	// Ajoute (n-1)*_emplacements.size emplacements dans le graphe si possible
	void generateMoreEmplacement(int n, int gridWidth, int gridHeight) {
		int nbTotal = gridWidth * gridHeight;
		if (n * _emplacementsPossibles.size() > nbTotal) {
			std::cout << "Pas assez de place dans la grille. Grille: " << nbTotal << " " << n << " * emp: " << n * _emplacementsPossibles.size() << std::endl;
		}
		else {
			int nbAjout = (n - 1) * _emplacementsPossibles.size();
			std::vector<std::vector<bool>> marque;
			for (int i = 0; i <= gridHeight; i++) {
				std::vector<bool> tmpVec(gridWidth + 1);
				marque.push_back(tmpVec);
			}
			for (int i = 0; i < _emplacementsPossibles.size(); i++) {
				int x = _emplacementsPossibles[i].getX();
				int y = _emplacementsPossibles[i].getY();
				marque[y][x] = true;
			}
			int x, y;
			for (int i = 0; i < nbAjout; i++) {
				do {
					x = generateRand(gridWidth + 1) - 1;
					y = generateRand(gridHeight + 1) - 1;
				} while (marque[y][x]);
				_emplacementsPossibles.push_back(Emplacement(Point(x, y), _emplacementsPossibles.size()));
			}
		}
	}

	std::vector<int> saveCopy() {
		std::vector<int> vectorId;
		for (int i = 0; i < _noeuds.size(); i++) {
			vectorId.push_back(_noeuds[i].getEmplacement()->getId());
		}
		return vectorId;
	}

	void loadCopy(std::vector<int> vectorId) {
		for (int i = 0; i < _noeuds.size(); i++) {
			_noeuds[i].clearEmplacement();
		}
		for (int i = 0; i < _noeuds.size(); i++) {
			_noeuds[i].setEmplacement(&_emplacementsPossibles[vectorId[i]]);
		}
	}

	void clearGraphe() {
		_noeuds.clear();
		_emplacementsPossibles.clear();
		_liens.clear();
	}

	bool isInVector(std::vector<int>& vectorInt, int x) {
		for (int i = 0; i < vectorInt.size(); i++) {
			if (vectorInt[i] == x) return true;
		}
		return false;
	}

	void copyFromGraphe(Graphe &graphe)
	{
		for (int i = 0; i < graphe._emplacementsPossibles.size(); ++i)
		{
			_emplacementsPossibles.push_back(
				Emplacement(
					Point(graphe._emplacementsPossibles[i].getX(), graphe._emplacementsPossibles[i].getY())
					, i)
			);
		}
		for (int i = 0; i < graphe._noeuds.size(); ++i)
		{
			_noeuds.push_back(Noeud(i));
		}

		for (int i = 0; i < graphe._liens.size(); ++i)
		{
			int id1 = graphe._liens[i].getNoeud1()->getId();
			int id2 = graphe._liens[i].getNoeud2()->getId();
			_liens.push_back(Aretes(&_noeuds[id1], &_noeuds[id2] ,i));
		}
		for (int i = 0; i < graphe._noeuds.size(); ++i)
		{
			int idEmplacement = graphe._noeuds[i].getEmplacement()->getId();
			_noeuds[i].setEmplacement(&_emplacementsPossibles[idEmplacement]);
		}
	}


	void croisementVoisinageFrom(Graphe& originalGraphe1, Graphe& originalGraphe2)
	{
		Graphe* currentGraphe;
		Graphe graphe1, graphe2;
		graphe1.copyFromGraphe(originalGraphe1);
		graphe2.copyFromGraphe(originalGraphe2);

		int currentGrapheNumber = generateRand(2);
		if (currentGrapheNumber == 1) currentGraphe = &graphe1;
		else currentGraphe = &graphe2;

		int nbNoeudATraiter = graphe1._noeuds.size() - nbNoeudEnCommun(graphe1, graphe2);
		std::cout << "Nb noeud a traiter au debut: " << nbNoeudATraiter << "\n";

		while (nbNoeudATraiter > 0)
		{
			Noeud* meilleurNoeud = nullptr;
			int meilleurScore;
			int nbRencontre = 0;
			std::cout << "Nb noeud a traiter encore: " << nbNoeudATraiter << "\n";

			//Trouve le meilleur noeud du graphe en cours d'analyse
			for (int i = 0; i < _noeuds.size(); ++i)
			{
				if (!_noeuds[i].estPlace())
				{
					if (meilleurNoeud == nullptr || meilleurScore < currentGraphe->getScoreCroisementNode(i))
					{
						meilleurNoeud = &currentGraphe->_noeuds[i];
						meilleurScore = currentGraphe->getScoreCroisementNode(i);
						nbRencontre = 1;
					}
					else if (meilleurScore == currentGraphe->getScoreCroisementNode(i))
					{
						++nbRencontre;
						int aleatoire = generateRand(nbRencontre);
						if (aleatoire == 1)
						{
							meilleurNoeud = &currentGraphe->_noeuds[i];
							meilleurScore = currentGraphe->getScoreCroisementNode(i);
						}
					}
				}
			}

			Emplacement* meilleurEmplacement = meilleurNoeud->getEmplacement();
			_noeuds[meilleurNoeud->getId()].setEmplacement(&_emplacementsPossibles[meilleurEmplacement->getId()]);
			if (!graphe1._noeuds[meilleurNoeud->getId()].compare(&graphe2._noeuds[meilleurNoeud->getId()]))
			{
				--nbNoeudATraiter;
			}
			graphe1._noeuds[meilleurNoeud->getId()].ecraseNoeud(&graphe1._emplacementsPossibles[meilleurEmplacement->getId()]);
			graphe2._noeuds[meilleurNoeud->getId()].ecraseNoeud(&graphe2._emplacementsPossibles[meilleurEmplacement->getId()]);

			//Place tout les voisins du point choisis
			for (Noeud* noeudVoisin : meilleurNoeud->getVoisins())
			{
				if (!_noeuds[noeudVoisin->getId()].estPlace())
				{
					meilleurEmplacement = noeudVoisin->getEmplacement();
					if (meilleurEmplacement == nullptr)
					{
						meilleurEmplacement = getMeilleurEmplacement(_noeuds[noeudVoisin->getId()]);
					}
					else
					{
						meilleurEmplacement = &_emplacementsPossibles[meilleurEmplacement->getId()];
						if (!meilleurEmplacement->estDisponible())
						{
							std::cout << "Meilleur emplacement appelle\n";
							meilleurEmplacement = getMeilleurEmplacement(_noeuds[noeudVoisin->getId()]);
						}
					}
					_noeuds[noeudVoisin->getId()].setEmplacement(&_emplacementsPossibles[meilleurEmplacement->getId()]);
					Noeud* noeudGraphe1 = &graphe1._noeuds[noeudVoisin->getId()];
					Noeud* noeudGraphe2 = &graphe2._noeuds[noeudVoisin->getId()];
					if (noeudGraphe1->estPlace() && noeudGraphe2->estPlace())
					{
						if (!noeudGraphe1->compare(noeudGraphe2))
						{
							--nbNoeudATraiter;
						}
					}
					else
					{
						--nbNoeudATraiter;
					}
					graphe1._noeuds[noeudVoisin->getId()].ecraseNoeud(&graphe1._emplacementsPossibles[meilleurEmplacement->getId()]);
					graphe2._noeuds[noeudVoisin->getId()].ecraseNoeud(&graphe2._emplacementsPossibles[meilleurEmplacement->getId()]);
				}
			}

			//Liste des noeuds à placer
			std::vector<int> noeudsAVerifier;
			for (int i = 0; i < _noeuds.size(); ++i)
			{
				if (!graphe1._noeuds[i].estPlace() || !graphe2._noeuds[i].estPlace())
				{
					noeudsAVerifier.push_back(i);
				}
			}
			std::cout << "Glouton 1\n";
			graphe1.completeBasicGlouton();
			std::cout << "Glouton 2\n";
			graphe2.completeBasicGlouton();
			std::cout << "Fin Glouton\n";
			//Si les lieux coincident les noeuds ne sont plus à traiter
			for (int i = 0; i < noeudsAVerifier.size(); ++i)
			{
				if (graphe1._noeuds[noeudsAVerifier[i]].compare(&graphe2._noeuds[noeudsAVerifier[i]]))
				{
					--nbNoeudATraiter;
				}
			}


			//Change le parent choisis 
			if (currentGrapheNumber == 1)
			{
				currentGraphe = &graphe2;
				currentGrapheNumber = 0;
			}
			else
			{
				currentGraphe = &graphe1;
				currentGrapheNumber = 1;
			}
		}

		_noeuds.swap(graphe1._noeuds);
		_liens.swap(graphe1._liens);
		_emplacementsPossibles.swap(graphe1._emplacementsPossibles);
	}

	int nbNoeudEnCommun(const Graphe& graphe1, const Graphe& graphe2)
	{
		int total = 0;
		for (int i = 0; i < graphe1._noeuds.size(); ++i)
		{
			if (graphe1._noeuds[i].compare(&graphe2._noeuds[i]))
			{
				++total;
			}
		}
		return total;
	}

	void croisementAleatoireFrom(Graphe* graphe1, Graphe* graphe2)
	{
		std::vector<int> noeudNonTraite;
		for (int noeud = 0; noeud < _noeuds.size(); ++noeud)
		{
			int random = generateRand(2);
			Graphe* currentGraphe;
			if (random == 1)
			{
				currentGraphe = graphe1;
			}
			else
			{
				currentGraphe = graphe2;
			}
			int idEmplacement = currentGraphe->_noeuds[noeud].getEmplacement()->getId();
			if (_emplacementsPossibles[idEmplacement].estDisponible())
			{
				_noeuds[noeud].setEmplacement(&_emplacementsPossibles[noeud]);
			}
			else
			{
				noeudNonTraite.push_back(noeud);
			}
		}

		for (int noeud : noeudNonTraite)
		{
			placementNoeudAleatoire(noeud);
		}
	}
	int getScoreCroisementNode(int nodeIndex) {
		long score = 0;
		std::vector<int> indexPasse;
		for (int i = 0; i < _noeuds[nodeIndex]._aretes.size(); ++i) {
			int index = _noeuds[nodeIndex]._aretes[i];
			for (int j = 0; j < _liens.size(); ++j) {
				if ((_liens[index]._id != _liens[j]._id) && (!isInVector(indexPasse, j))) {
					if (!(_liens[index].contains(_liens[j].getNoeud1()) || _liens[index].contains(_liens[j].getNoeud2()))) {
						if (seCroisent(_liens[index], _liens[j])) {
							if (surSegment(_liens[index], *_liens[j].getNoeud1()) || surSegment(_liens[index], *_liens[j].getNoeud2())) {
								score += PENALITE_MAX;
							}
							else if (surSegment(_liens[j], *_liens[index].getNoeud1()) || surSegment(_liens[j], *_liens[index].getNoeud2())) {
								score += PENALITE_MAX;
							}
							else {
								score++;
							}
						}
					}
					else {
						Noeud* nodeNotInCommon = _liens[j].nodeNotInCommon(_liens[index]);
						if (surSegment(_liens[index], *nodeNotInCommon)) {
							score += PENALITE_MAX;
						}
						else {
							nodeNotInCommon = _liens[index].nodeNotInCommon(_liens[j]);
							if (surSegment(_liens[j], *nodeNotInCommon)) {
								score += PENALITE_MAX;
							}
						}
					}
				}
			}
			indexPasse.push_back(index);
		}
		return score;
	}

	int getScoreCroisementNode(int nodeIndex, int swapIndex) {
		long score = 0;
		std::vector<int> indexPasse;
		for (int i = 0; i < _noeuds[nodeIndex]._aretes.size(); ++i) {
			int index = _noeuds[nodeIndex]._aretes[i];
			if (!_liens[index].contains(swapIndex)) {
				for (int j = 0; j < _liens.size(); ++j) {
					if ((_liens[index]._id != _liens[j]._id) && (!isInVector(indexPasse, j))) {
						if (!_liens[j].contains(swapIndex)) {
							if (!(_liens[index].contains(_liens[j].getNoeud1()) || _liens[index].contains(_liens[j].getNoeud2()))) {
								if (seCroisent(_liens[index], _liens[j])) {
									if (surSegment(_liens[index], *_liens[j].getNoeud1()) || surSegment(_liens[index], *_liens[j].getNoeud2())) {
										score += PENALITE_MAX;
									}
									else if (surSegment(_liens[j], *_liens[index].getNoeud1()) || surSegment(_liens[j], *_liens[index].getNoeud2())) {
										score += PENALITE_MAX;
									}
									else {
										score++;
									}
								}
							}
							else {
								Noeud* nodeNotInCommon = _liens[j].nodeNotInCommon(_liens[index]);
								if (surSegment(_liens[index], *nodeNotInCommon)) {
									score += PENALITE_MAX;
								}
								else {
									nodeNotInCommon = _liens[index].nodeNotInCommon(_liens[j]);
									if (surSegment(_liens[j], *nodeNotInCommon)) {
										score += PENALITE_MAX;
									}
								}
							}
						}
					}
				}
			}
			indexPasse.push_back(index);
		}
		return score;
	}





};

#endif