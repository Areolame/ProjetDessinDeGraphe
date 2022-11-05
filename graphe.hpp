#ifndef GRAPHE_HPP
#define GRAPHE_HPP

#include <vector>
#include "lien.hpp"
#include "point.hpp"
#include "emplacement.hpp"

class Graphe {
public:
	std::vector<Emplacement> _pointsPossibles;
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
	void setPoints(std::vector<Point> emplacementsPossibles) { _emplacementsPossibles = emplacementsPossibles; }

	std::vector<Noeud> getNoeuds() const { return _noeuds; }
	std::vector<Lien> getLiens()  const { return _liens; }
	std::vector<Emplacement> getEmplacementsPossibles()  const { return _pointsPossibles; }

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
					if (!(lien1.contains(lien2.getNoeud1()) || lien1.contains(lien2.getNoeud2())))
					{
						if (Intersection.seCroisent(lien1, lien2))
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