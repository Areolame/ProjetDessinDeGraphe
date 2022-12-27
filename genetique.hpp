#ifndef UTILITAIRE_HPP
#define UTILIRAIRE_HPP

#include <vector>
#include <iostream>


std::vector<Graphe> graphes;
std::vector<bool> isChild;


int findBestGraphe()
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

double calculMoyenneGraphes()
{
	double moyenne = 0;
	for (Graphe graphe : graphes)
	{
		moyenne += graphe.getNbCroisement();
	}
	return moyenne/graphes.size();
}

int perfectGraphe()
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



Graphe grapheGenetique(int population, int maxIteration, const std::string &nomGraphe, const std::string &nomSlot)
{
	isChild.resize(population);
	graphes.resize(population);
	for (int i = 0; i < population; ++i)
	{
		int a, b;
		readFromJsonGraph(graphes[i], nomGraphe);
		readFromJsonSlots(graphes[i], nomSlot, a, b);
		graphes[i].placementAleatoire();
	}
	int index;
	int currentIteration = 0;
	while (currentIteration < maxIteration && (index = perfectGraphe()) == -1)
	{
		double moyenne = calculMoyenneGraphes();
		std::cout << "Current Iteration: " << currentIteration << "\n";

		//Definit les nouveaux enfants
		for (int i = 0; i < graphes.size(); ++i)
		{
			if (graphes[i].getNbCroisement() < moyenne)
			{
				graphes[i].clearNodeEmplacement();
				isChild[i] = true;
			}
			else
			{
				isChild[i] = false;
			}
		}

		for (int i = 0; i < isChild.size(); ++i)
		{
			if (isChild[i])
			{
				int grapheID1, grapheID2;
				do
				{
					grapheID1 = generateRand(graphes.size()) - 1;
				}
				while (isChild[grapheID1]);
				do
				{
					grapheID2 = generateRand(graphes.size()) - 1;
				} 
				while (isChild[grapheID2]);
				graphes[i].croisementVoisinageFrom(graphes[grapheID1], graphes[grapheID2]);

			}
		}
		++currentIteration;

	}
	if (currentIteration >= maxIteration)
	{
		index = findBestGraphe();
	}

	return graphes[index];
}




#endif