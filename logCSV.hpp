#ifndef LOGCSV_HPP
#define LOGCSV_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "ogdfFunctions.hpp"
#include <omp.h>

void generateCSV(int nbEssay, const std::string& methodeName, const std::string& methodeAlgoName, const std::string& nomGraphe, Graphe& G) {
	double moyenneCroisement = 0, medianCroisement;
	int meilleurCroisement = INT_MAX;
	double tempsExecMoyenne = 0;
	std::vector<int> croisementVector(nbEssay);
	std::vector<double> tempExecVector(nbEssay);
	bool saveResult = true;
	for (int i = 0; i < nbEssay; ++i)
	{
		saveResult = true;
		std::cout << "Current Execution: " << i << "\n";
		auto start = std::chrono::system_clock::now();
		if (methodeName == "Glouton") G.glouton();
		else if (methodeName == "Glouton Revisit�") G.gloutonRevisite();
		else if (methodeName == "Glouton Gravit�") G.gloutonRevisiteGravite();
		else if (methodeName == "Glouton Voisin") G.gloutonRevisiteVoisin();
		else if (methodeName == "OGDF") ogdfPlacementAuPlusProche(G);
		else if (methodeName == "Al�atoire") G.placementAleatoire();
		else
		{
			std::cout << "ERROR Aucune methode " << methodeName << " trouve !";
			return;
		}

		if (G.estPlace()) {

			if (methodeAlgoName == "Recuit Simul�") G.recuitSimule();
			else if (methodeAlgoName == "Recuit Simul� Delay") G.recuitSimuleDelay();
			else if (methodeAlgoName == "Recuit Simul� Tournoi Binaire") G.recuitSimuleTournoiBinaire();
			else if (methodeAlgoName == "Recuit Simul� Tournoi Multiple") G.recuitSimuleTournoiMultiple();
			else if (methodeAlgoName == "Recuit Simul� M1") G.recuitSimule(0.99999, 100, 1);
			else if (methodeAlgoName == "Recuit Simul� M2") G.recuitSimule(0.99999, 100, 2);
			else if (methodeAlgoName == "Recuit Simul� Delay M1") G.recuitSimuleDelay(0.99999, 100, 10, 1);
			else if (methodeAlgoName == "Recuit Simul� Delay M2") G.recuitSimuleDelay(0.99999, 100, 10, 2);
			else if (methodeAlgoName == "Recuit Simul� Tournoi Binaire M1") G.recuitSimuleTournoiBinaire(0.99999, 100, 1);
			else if (methodeAlgoName == "Recuit Simul� Tournoi Binaire M2") G.recuitSimuleTournoiBinaire(0.99999, 100, 2);
			else if (methodeAlgoName == "Recuit Simul� Tournoi Multiple M1") G.recuitSimuleTournoiMultiple(0.99999, 100, 1);
			else if (methodeAlgoName == "Recuit Simul� Tournoi Multiple M2") G.recuitSimuleTournoiMultiple(0.99999, 100, 2);
			else if (methodeAlgoName == "Best Deplacement") G.bestDeplacement();
			else if (methodeAlgoName != "Aucun") {
				std::cout << "ERROR Aucun algo " << methodeAlgoName << " trouve !";
				return;
			}


			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> secondsTotal = end - start;
			croisementVector[i] = G.getNbCroisement();
			//cout << croisementVector[i] << "\n";
			tempExecVector[i] = secondsTotal.count();
			G.clearNodeEmplacement();
		}
		else {
			saveResult = false;
		}
	}
	if (saveResult) {
		for (int croisement : croisementVector)
		{
			moyenneCroisement += croisement;
			if (meilleurCroisement > croisement)
			{
				meilleurCroisement = croisement;
			}
		}
		moyenneCroisement /= nbEssay;

		for (double tempExec : tempExecVector)
		{
			tempsExecMoyenne += tempExec;
		}
		tempsExecMoyenne /= nbEssay;

		sort(croisementVector.begin(), croisementVector.end());
		if (croisementVector.size() % 2 == 1)
		{
			medianCroisement = croisementVector[croisementVector.size() / 2];
		}
		else
		{
			int nbCroisement1 = croisementVector[croisementVector.size() / 2];
			int nbCroisement2 = croisementVector[(croisementVector.size() / 2) - 1];

			medianCroisement = (nbCroisement1 + nbCroisement2) / 2.0;
		}

		string nomFichier = "./resultats/" + nomGraphe + ".csv";
		std::ofstream resultats(nomFichier, std::ios_base::app);

		resultats << methodeName << ","
			<< methodeAlgoName << ","
			<< nbEssay << ","
			<< G._noeuds.size() << ","
			<< G._liens.size() << ","
			<< G._emplacementsPossibles.size() << ","
			<< meilleurCroisement << ","
			<< moyenneCroisement << ","
			<< medianCroisement << ","
			<< tempsExecMoyenne << "\n";
		resultats.close();
	}
}


#endif
