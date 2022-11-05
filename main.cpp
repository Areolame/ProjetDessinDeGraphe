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
	string nom_fichier = "auto21-8_MaxFace2";
	string file = "D:/The World/Cours/M1S2/Graphe/Projet/DessinGraphe/embeddings/auto21-8/" + nom_fichier + ".json";

	bool useOpenGL = true;
	bool planarize = false;
	bool upscaleGrid = true;
	int margeGrid = 300;

	readFromJson(file, gridWidth, gridHeight);

	int maxX = gridWidth, maxY = gridHeight, minX = 0, minY = 0;

	if (upscaleGrid) {
		gridWidth = maxX + margeGrid;
		gridHeight = maxY + margeGrid;
	}
	minX = 0;
	minY = 0;
	std::cout << "Setup complete!" << std::endl;

	// OpenGL
	srand(static_cast<unsigned int>(time(NULL)));
	if (useOpenGL) {
		dispOpenGL(gridWidth, gridHeight, maxX, maxY, nom_fichier);
	}
	return 0;
}