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

	//G.placementAleatoire();
	//G.glouton();
	G.gloutonRevisite();
	G.afficherNoeuds();
	G.afficherEmplacement();
	G.afficherLiens();

	bool useOpenGL = true;
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