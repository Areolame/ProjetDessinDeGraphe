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
const std::string NBNOEUDTABNAME = "Nombre de noeud, ";
const std::string NBARETETABNAME = "Nombre d'arete, ";
const std::string NBEMPLACEMENTTABNAME = "Nombre d'emplacement, ";
const std::string NBCROISEMENTMEILLEURTABNAME = "Nombre de croisement (Meilleur), ";
const std::string NBCROISEMENTMOYENNETABNAME = "Nombre de croisement (Moyenne), ";
const std::string NBCROISEMENTMEDIANTABNAME = "Nombre de croisement (Médian), ";
const std::string TEMPSEXECTABNAME = "Temps d'exécution (s) (Moyenne)";

void calculResultats(int nbEssay, const std::string& methodeName, Graphe& G)
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

int main() {

	int gridWidth = 10, gridHeight = 10;

	// ----- LECTURE D'UN FICHIER JSON DANS UN Graph -----

	Graphe G;

	int lecture = 0;

	if (lecture == 0) {
		string nomFichierGraph = "graph_exemple1";
		string nomFichierSlots = "slot_exemple1";
		string fileGraph = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Graphe/" + nomFichierGraph + ".json";
		string fileSlots = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Slots/" + nomFichierSlots + ".json";
		readFromJsonGraph(G, fileGraph);
		readFromJsonSlots(G, fileSlots, gridWidth, gridHeight);
	}
	// Fichiers 2022
	else if (lecture == 1) {
		string nomFichierGraph = "auto21-6";
		string nomFichier = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/automatique/" + nomFichierGraph + ".json";
		//string nomFichier = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/2019/" + nomFichierGraph + ".json";
		std::vector<int> tmpvec = readFromJsonOldGraph(nomFichier, G, gridWidth, gridHeight);
		string nomFichierSlots = nomFichierGraph + "-slots.json";
		string nomFichierGrapheSave = "graph-" + nomFichierGraph + ".json";
		bool save = false;
		if (save) {
			bool savex2 = false;
			bool savex3 = false;
			if (savex2) {
				G.generateMoreEmplacement(2, gridWidth, gridHeight);
				nomFichierSlots = "2X-" + nomFichierSlots;
			}
			else if (savex3) {
				G.generateMoreEmplacement(3, gridWidth, gridHeight);
				nomFichierSlots = "3X-" + nomFichierSlots;
			}
			writeToJsonSlots(G, nomFichierSlots, gridWidth, gridHeight);
			//writeToJsonGraph(G, nomFichierGrapheSave);
		}
	}
	else if (lecture == 2) {
		std::ofstream out("data.txt");
		std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf
		std::cout.rdbuf(out.rdbuf()); //redirect std::cout
		std::unordered_map<string, std::vector<string>> mapGraphSlots;
		for (int i = 1; i <= 6; i++) {
			mapGraphSlots.insert({ "graph-"+to_string(i) + "-input",{to_string(i) + "-input-slots","2X-" + to_string(i) + "-input-slots","3X-" + to_string(i) + "-input-slots"}});
		}
		for (auto key : mapGraphSlots) {
			for (int i = 0; i < key.second.size(); i++) {
				std::cout << "-----------------------------------------" << std::endl;
				std::cout << "Graphe: " << key.first << " " << key.second[i] << std::endl;
				G.clearGraphe();
				string fileGraph = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Graphe/" + key.first + ".json";
				string fileSlots = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Slots/" + key.second[i] + ".json";
				readFromJsonGraph(G, fileGraph);
				readFromJsonSlots(G, fileSlots, gridWidth, gridHeight);
				G.placementAleatoire();
				G.recuitSimule();
			}
		}
		std::cout.rdbuf(coutbuf); //reset to standard output again
		return 0;
	}

	G.placementAleatoire();
	//G.afficherNoeuds();
	//G.afficherEmplacement();
	//G.afficherLiens();
	std::cout << "Nombre intersection apres placement: " << G.getNbCroisement() << std::endl;
	//calculResultats(100000, "Aléatoire", G);

	bool useOpenGL = true;

	int maxX = gridWidth, maxY = gridHeight;

	std::cout << "Setup complete!" << std::endl;

	//G.bestDeplacement();

	// OpenGL
	srand(static_cast<unsigned int>(time(NULL)));
	if (useOpenGL) {
		std::cout << "Grid: " << gridWidth << " " << gridHeight << std::endl;
		if ((gridWidth <= 0) || (gridWidth > 100000))
			gridWidth = 10;
		if ((gridHeight <= 0) || (gridHeight > 100000))
			gridHeight = 10;
		dispOpenGL(G, gridWidth+1, gridHeight+1, maxX, maxY);
	}
	return 0;
}