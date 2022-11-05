#ifndef GRAPHE_HPP
#define GRAPHE_HPP

#include <vector>
#include "lien.hpp"
#include "point.hpp"

class Graphe {
public:
	std::vector<Point> _pointsPossibles;
	std::vector<Lien> _liens;
	Graphe(std::vector<Lien> liens, std::vector<Point> pointsPossibles) {
		_pointsPossibles = pointsPossibles;
		_liens = liens;
	}

	std::vector<Lien> getLiens()  const { return _liens; }
	std::vector<Point> getPointsPossibles()  const { return _pointsPossibles; }

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