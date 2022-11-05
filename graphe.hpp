#ifndef GRAPHE_HPP
#define GRAPHE_HPP

#include <vector>
#include "lien.hpp"
#include "point.hpp"
#include "intersection.hpp"
#include "emplacement.hpp"
#include "utilitaire.hpp"

class Graphe {
public:
	std::vector<Emplacement> _emplacementsPossibles;
	std::vector<Lien> _liens;
	std::vector<Noeud> _noeuds;
	Graphe(std::vector<Lien> liens, std::vector<Emplacement> emplacementsPossibles) {
		_emplacementsPossibles = emplacementsPossibles;
		_liens = liens;
	}

	Graphe()
	{}

	void setNoeuds(std::vector<Noeud> noeuds){_noeuds = noeuds;}
	void setLiens(std::vector<Lien> liens) { _liens = liens; }
	void setPoints(std::vector<Emplacement> emplacementsPossibles) { _emplacementsPossibles = emplacementsPossibles; }

	std::vector<Noeud> getNoeuds() const { return _noeuds; }
	std::vector<Lien> getLiens()  const { return _liens; }
	std::vector<Emplacement> getEmplacementsPossibles()  const { return _emplacementsPossibles; }

	void placementAleatoire()
	{
		for (int i = 0; i < _noeuds.size(); ++i)
		{
			Noeud noeud = _noeuds[i];
			int emplacementPossibleSize = _emplacementsPossibles.size();
			Emplacement emplacement = nullptr;
			int depth = 0;
			do
			{
				emplacement = _emplacementsPossibles[generateRand(emplacementPossibleSize) - 1];
				++depth;
			} while ( !emplacement.estDisponible() && depth < emplacementPossibleSize * 3);

			if (depth == emplacementPossibleSize * 3)
			{
				std::cout << "[LOG ERROR] Maximum emplacements atteints";
				return;
			}
			emplacement.setNoeud(&noeud);
		}
	}

	int getNbCroisement()
	{
		int total = 0;
		for (int i = 0; i < _liens.size(); ++i)
		{
			for (int j = 0; i < _liens.size(); ++j)
			{
				if (i != j)
				{
					Lien lien1 = _liens[i], lien2 = _liens[j];
					if (!(lien1.contains(&lien2.getNoeud1()) || lien1.contains(&lien2.getNoeud2())))
					{
						if (seCroisent(lien1, lien2))
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