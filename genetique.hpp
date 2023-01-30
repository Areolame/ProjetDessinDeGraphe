#ifndef UTILITAIRE_HPP
#define UTILIRAIRE_HPP

#include <vector>
#include <iostream>

int findBestGraphe(std::vector<Graphe>& graphes)
{
	int meilleurID = 0;
	int nbRencontre = 0;
	int currentNbCroisement, meilleurNbCroisement = graphes[0].getNbCroisement();
	for (int i = 1; i < graphes.size(); ++i)
	{
		currentNbCroisement = graphes[i].getNbCroisement();
		if (currentNbCroisement < meilleurNbCroisement)
		{
			meilleurID = i;
			meilleurNbCroisement = currentNbCroisement;
			nbRencontre = 0;
		}
		else if (currentNbCroisement == meilleurNbCroisement)
		{
			++nbRencontre;
			int aleatoire = generateRand(nbRencontre);
			if (aleatoire == 1)
			{
				meilleurID = i;
				meilleurNbCroisement = currentNbCroisement;
			}
		}
	}
	return meilleurID;
}

double calculMoyenneGraphes(std::vector<Graphe>& graphes)
{
	double moyenne = 0;
	for (Graphe graphe : graphes)
	{
		moyenne += graphe.getNbCroisement();
	}
	return moyenne / graphes.size();
}

int perfectGraphe(std::vector<Graphe>& graphes)
{
	for (int i = 0; i < graphes.size(); ++i)
	{
		if (graphes[i].getNbCroisement() == 0)
		{
			return i;
		}
	}
	return -1;
}



Graphe grapheGenetique(int population, int maxIteration, const std::string& nomGraphe, const std::string& nomSlot, bool useRecuit, bool useRand=false)
{
	std::vector<Graphe> graphes;
	graphes.resize(population);
	for (int i = 0; i < population; ++i)
	{
		readFromJsonGraph(graphes[i], nomGraphe);
		readFromJsonSlots(graphes[i], nomSlot);
		graphes[i].placementAleatoire();
	}
	int index;
	int currentIteration = 0;
	while (currentIteration < maxIteration && (index = perfectGraphe(graphes)) == -1)
	{
		double moyenne = calculMoyenneGraphes(graphes);
		std::cout << "Current Iteration: " << currentIteration << "\n";

		sort(graphes.begin(), graphes.end());

		for (int i = population/2; i < population; ++i)
		{
			graphes[i].clearNodeEmplacement();
			//std::cout << "Enfant: " << i << std::endl;
			int grapheID1, grapheID2;
			grapheID1 = generateRand(population/2 - 1);
			grapheID2 = generateRand(population/2 - 1);
			//std::cout << "P1: " << grapheID1 << " P2: " << grapheID2 << std::endl;
			graphes[i].croisementVoisinageFrom(graphes[grapheID1], graphes[grapheID2], useRand);
			if (useRecuit) {
				graphes[i].recuitSimule(0.99, 100.0, 0);
			}
		}
		++currentIteration;

		std::cout << "Meilleur graphe: " << graphes[findBestGraphe(graphes)].getNbCroisement() << " \n";

	}
	if (currentIteration >= maxIteration)
	{
		index = findBestGraphe(graphes);
	}

	return graphes[index];
}




#endif