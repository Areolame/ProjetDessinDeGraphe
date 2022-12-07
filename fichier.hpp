#ifndef FICHIER_HPP
#define FICHIER_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "jsonIO.hpp"
#include "dispOpenGL.hpp"

const std::string METHODETABNAME = "Méthode Utilisé, ";
const std::string NBEXECUTIONTABNAME = "Nombre d'exécution, ";
const std::string NBNOEUDTABNAME = "Nombre de noeud, ";
const std::string NBARETETABNAME = "Nombre d'arete, ";
const std::string NBEMPLACEMENTTABNAME = "Nombre d'emplacement, ";
const std::string NBCROISEMENTMEILLEURTABNAME = "Nombre de croisement (Meilleur), ";
const std::string NBCROISEMENTMOYENNETABNAME = "Nombre de croisement (Moyenne), ";
const std::string NBCROISEMENTMEDIANTABNAME = "Nombre de croisement (Médian), ";
const std::string TEMPSEXECTABNAME = "Temps d'exécution (s) (Moyenne)";

void generateSCV(int nbEssay, const std::string& methodeName, Graphe& G)
{
	double moyenneCroisement = 0, medianCroisement;
	int meilleurCroisement = INT_MAX;
	double tempsExecMoyenne = 0;
	std::vector<int> croisementVector(nbEssay);
	std::vector<double> tempExecVector(nbEssay);
	for (int i = 0; i < nbEssay; ++i)
	{
		std::cout << "Current Execution: " << i << "\n";
		auto start = std::chrono::system_clock::now();
		if (methodeName == "Glouton") G.gloutonRevisite();
		else if (methodeName == "Glouton Gravité") G.gloutonRevisiteGravite();
		else if (methodeName == "Glouton Voisin") G.gloutonRevisiteVoisin();
		else if (methodeName == "Aléatoire") G.placementAleatoire();
		else
		{
			std::cout << "ERROR Aucune methode " << methodeName << " trouve !";
			return;
		}
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> secondsTotal = end - start;
		croisementVector[i] = G.getNbCroisement();
		//cout << croisementVector[i] << "\n";
		tempExecVector[i] = secondsTotal.count();
		G.clearNodeEmplacement();
	}

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


	std::ofstream resultats;
	string nomFichier = "./resultats/resultats.csv";
	remove("./resultats/resultats.csv");
	resultats.open(nomFichier);

	resultats << METHODETABNAME << NBEXECUTIONTABNAME << NBNOEUDTABNAME << NBARETETABNAME << NBEMPLACEMENTTABNAME
		<< NBCROISEMENTMEILLEURTABNAME << NBCROISEMENTMOYENNETABNAME << NBCROISEMENTMEDIANTABNAME << TEMPSEXECTABNAME << "\n";

	resultats << methodeName << ", "
		<< nbEssay << ", "
		<< G._noeuds.size() << ", "
		<< G._liens.size() << ", "
		<< G._emplacementsPossibles.size() << ", "
		<< meilleurCroisement << ", "
		<< moyenneCroisement << ", "
		<< medianCroisement << ", "
		<< tempsExecMoyenne << "\n";

}


#endif
