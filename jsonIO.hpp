#ifndef JSONIO
#define JSONIO

#include <string>
#include <nlohmann/json.hpp>
#include "graphe.hpp"

using std::string;
using nlohmann::json;

// ----- CREATION D'UN Graph A PARTIR D'UN FICHIER JSON -----
// Precondition: Les id des noeuds sont ordonnées et commencent par 0
void readFromJsonGraph(Graphe& G, string input) {
	std::cout << input << std::endl;
	std::ifstream inp(input);
	json j;
	inp >> j;

	// Si le fichier ne contient pas de node
	if (j["nodes"] == nullptr) {
		exit(1);
	}
	int nodeNumber = static_cast<int>(j["nodes"].size());
	std::cout << "Nombre de noeud dans le json: " << nodeNumber << std::endl;
	for (int i = 0; i < nodeNumber; i++) {
		G._noeuds.push_back(Noeud(i));
	}
	int edgeNumber = static_cast<int>(j["edges"].size());
	int id1, id2;
	for (int i = 0; i < edgeNumber; i++) {
		id1 = j["edges"][i]["source"];
		id2 = j["edges"][i]["target"];
		G._liens.push_back(Aretes(&G._noeuds[id1], &G._noeuds[id2]));
	}
}

// Lecture des slots
// Precondition: Les id des slots sont ordonnés et commencent par 0
void readFromJsonSlots(Graphe& G, string input, int& gridWidth, int& gridHeight) {
	std::ifstream inp(input);
	json j;
	inp >> j;

	// Si le fichier ne contient pas de slots
	if (j["slots"] == nullptr) {
		exit(1);
	}

	int slotsNumber = static_cast<int>(j["slots"].size());
	std::cout << "Nombre de slots dans le fichier json: " << slotsNumber << std::endl;
	int x, y;
	for (int i = 0; i < slotsNumber; i++) {
		x = j["slots"][i]["x"];
		y = j["slots"][i]["y"];
		G._emplacementsPossibles.push_back(Emplacement(Point(x,y), i));
		if (x > gridWidth) { gridWidth = x; }
		if (y > gridHeight) { gridHeight = y; }
	}
}

// Sauvegarde des slots dans le fichier output
void writeToJsonSlots(const Graphe& G, string output, int gridWidth, int gridHeight) {
	json j;
	j["width"] = gridWidth;
	j["height"] = gridHeight;

	int slotsNumber = G._emplacementsPossibles.size();
	for (int i = 0; i < slotsNumber; i++) {
		j["slots"][i]["id"] = i;
		j["slots"][i]["x"] = G._emplacementsPossibles[i].getX();
		j["slots"][i]["y"] = G._emplacementsPossibles[i].getY();
	}

	std::ofstream o(output);
	o << std::setw(4) << j << std::endl;
}

// ----- CREATION D'UN Graph A PARTIR D'UN FICHIER JSON -----
// Creer les emplacements a partir des anciennes coord
// Les noeuds doivent avoir des coordonnées
void readFromJsonOldGraph(string input, Graphe& G, int& gridWidth, int& gridHeight) {
	std::ifstream inp(input);
	json j;
	inp >> j;
	gridWidth = j["width"];
	gridHeight = j["height"];
	std::cout << "gridWidth: " << gridWidth << " gridHeight: " << gridHeight << std::endl;

	if (j["nodes"] == nullptr) {
		exit(1);
	}

	int nodeNumber = static_cast<int>(j["nodes"].size());
	int x, y;
	for (int i = 0; i < nodeNumber; i++) {
		G._noeuds.push_back(Noeud(i));
		x = j["nodes"][i]["x"];
		y = j["nodes"][i]["y"];
		G._emplacementsPossibles.push_back(Emplacement(Point(x, y), i));
	}

	int edgeNumber = static_cast<int>(j["edges"].size());
	int id1, id2;
	for (int i = 0, index = nodeNumber; i < edgeNumber; i++) {
		id1 = j["edges"][i]["source"];

		if (j["edges"][i]["bends"] != nullptr) {
			int bendsNumber = static_cast<int>(j["edges"][i]["bends"].size());
			for (int k=0; k < bendsNumber; k++, index++) {
				id2 = index;
				G._noeuds.push_back(Noeud(index));
				x = j["edges"][i]["bends"][k]["x"];
				y = j["edges"][i]["bends"][k]["y"];
				G._emplacementsPossibles.push_back(Emplacement(Point(x, y), index));
				G._liens.push_back(Aretes(&G._noeuds[id1], &G._noeuds[id2]));
				id1 = id2;
			}
		}
		id2 = j["edges"][i]["target"];
		G._liens.push_back(Aretes(&G._noeuds[id1], &G._noeuds[id2]));
	}
}

#endif