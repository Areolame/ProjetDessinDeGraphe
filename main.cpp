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
	string nomFichierGraph = "graph_exemple1";
	string nomFichierSlots = "slot_exemple1";
	string fileGraph = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Graphe/" + nomFichierGraph + ".json";
	string fileSlots = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Slots/" + nomFichierSlots + ".json";

	Graphe G;

	readFromJsonGraph(G, fileGraph);
	readFromJsonSlots(G, fileSlots, gridWidth, gridHeight);
	std::cout << "Grid: " << gridWidth << " " << gridHeight << std::endl;
	gridWidth = 10;
	gridHeight = 10;

	//G.placementAleatoire();
	G.glouton();
	//G.afficherNoeuds();
	//G.afficherLiens();

	bool useOpenGL = true;
	bool planarize = false;

	int maxX = gridWidth, maxY = gridHeight;

	std::cout << "Setup complete!" << std::endl;

	// OpenGL
	srand(static_cast<unsigned int>(time(NULL)));
	if (useOpenGL) {
		dispOpenGL(G, gridWidth, gridHeight, maxX, maxY, nomFichierGraph);
	}
	return 0;
}