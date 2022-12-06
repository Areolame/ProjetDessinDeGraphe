#include <string>
#include <stdio.h>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>

#include "jsonIO.hpp"
#include "dispOpenGL.hpp"

using namespace std;

const std::string METHODETABNAME = "Méthode Utilisé, ";
const std::string NBEXECUTIONTABNAME = "Nombre d'exécution, ";
const std::string NBNOEUDTABNAME= "Nombre de noeud, ";
const std::string NBARETETABNAME = "Nombre d'arete, ";
const std::string NBEMPLACEMENTTABNAME = "Nombre d'emplacement, ";
const std::string NBCROISEMENTMEILLEURTABNAME = "Nombre de croisement (Meilleur), ";
const std::string NBCROISEMENTMOYENNETABNAME = "Nombre de croisement (Moyenne), ";
const std::string NBCROISEMENTMEDIANTABNAME = "Nombre de croisement (Médian), ";
const std::string TEMPSEXECTABNAME = "Temps d'exécution (s) (Moyenne)";

void calculResultats(int nbEssay, const std::string &methodeName, Graphe &G)
{
	double moyenneCroisement = 0, medianCroisement;
	int meilleurCroisement = INT_MAX;
	double tempsExecMoyenne = 0;
	std::vector<int> croisementVector(nbEssay);
	std::vector<double> tempExecVector(nbEssay);
	for (int i = 0; i < nbEssay; ++i)
	{
		cout << "Current Execution: " << i << "\n";
		auto start = std::chrono::system_clock::now();
		if (methodeName == "Glouton") G.glouton();
		else if (methodeName == "Aléatoire") G.placementAleatoire();
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> secondsTotal = end - start;
		croisementVector[i] = G.getNbCroisement();
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
	remove("./resultats/resultats.csv") ;
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

int main() {

	int gridWidth, gridHeight;

	// ----- LECTURE D'UN FICHIER JSON DANS UN Graph -----

	Graphe G;

	int lecture = 0;

	if (lecture == 0) {
		string nomFichierGraph = "graph_exemple1";
		string nomFichierSlots = "slot_exemple1";
		string fileGraph = "C:/Users/Lenovo Yoga 700/source/repos/ConsoleApplicationOpenGL/ConsoleApplicationOpenGL/exemple/Graphe/" + nomFichierGraph + ".json";
		string fileSlots = "C:/Users/Lenovo Yoga 700/source/repos/ConsoleApplicationOpenGL/ConsoleApplicationOpenGL/exemple/Slots/" + nomFichierSlots + ".json";
		readFromJsonGraph(G, fileGraph);
		readFromJsonSlots(G, fileSlots, gridWidth, gridHeight);
	}
	// Fichiers 2022
	else if (lecture == 1) {
		string nomFichierGraph = "auto21-1";
		string nomFichier = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/automatique/" + nomFichierGraph + ".json";
		readFromJsonOldGraph(nomFichier, G, gridWidth, gridHeight);
	}

	std::cout << "Grid: " << gridWidth << " " << gridHeight << std::endl;
	
	gridWidth = 10;
	gridHeight = 10;

	//G.gloutonRevisite();
	//G.afficherNoeuds();
	//G.afficherEmplacement();
	//G.afficherLiens();
	calculResultats(100000, "Aléatoire", G);
	

	bool useOpenGL = false;
	bool planarize = false;
	
	int maxX = gridWidth, maxY = gridHeight;
	
	std::cout << "Setup complete!" << std::endl;
	
	// OpenGL
	srand(static_cast<unsigned int>(time(NULL)));
	if (useOpenGL) {
		dispOpenGL(G, gridWidth, gridHeight, maxX, maxY);
	}
	return 0;
}