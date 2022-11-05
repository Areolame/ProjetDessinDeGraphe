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
	string nomFichierGraph = "auto21-8_MaxFace2";
	string nomFichierSlots = "auto21-8_MaxFace2";
	string fileGraph = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Graphe/" + nomFichierGraph + ".json";
	string fileSlots = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Slots/" + nomFichierSlots + ".json";

	readFromJsonGraph(fileGraph);
	readFromJsonSlots(fileSlots, gridWidth, gridHeight);

	bool useOpenGL = true;
	bool planarize = false;

	int maxX = gridWidth, maxY = gridHeight;

	std::cout << "Setup complete!" << std::endl;

	// OpenGL
	srand(static_cast<unsigned int>(time(NULL)));
	if (useOpenGL) {
		dispOpenGL(gridWidth, gridHeight, maxX, maxY, nomFichierGraph);
	}
	return 0;
}