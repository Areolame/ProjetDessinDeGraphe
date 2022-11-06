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
	std::vector<Noeud> tmpVectorNoeud;
	std::vector<Noeud*> tmpVectorNoeud2;
	int nodeNumber = static_cast<int>(j["nodes"].size());
	std::cout << "Nombre de noeud dans le json: " << nodeNumber << std::endl;
	for (int i = 0; i < nodeNumber; i++) {
		Noeud* tmpNoeud = new Noeud(i);
		tmpVectorNoeud.push_back(*tmpNoeud);
		tmpVectorNoeud2.push_back(tmpNoeud);
	}
	G.setNoeuds(tmpVectorNoeud2);
	std::vector<Lien> vectorLien;
	int edgeNumber = static_cast<int>(j["edges"].size());
	int id1, id2;
	for (int i = 0; i < edgeNumber; i++) {
		id1 = j["edges"][i]["source"];
		id2 = j["edges"][i]["target"];
		vectorLien.push_back(Lien(tmpVectorNoeud2[id1], tmpVectorNoeud2[id2]));
	}
	G.setLiens(vectorLien);
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

	std::vector<Emplacement> vectorEmplacement;
	int slotsNumber = static_cast<int>(j["slots"].size());
	std::cout << "Nombre de slots dans le fichier json: " << slotsNumber << std::endl;
	int x, y;
	for (int i = 0; i < slotsNumber; i++) {
		x = j["slots"][i]["x"];
		y = j["slots"][i]["y"];
		vectorEmplacement.push_back(Emplacement(Point(x,y), i));
		if (x > gridWidth) { gridWidth = x; }
		if (y > gridHeight) { gridHeight = y; }
	}

	G.setEmplacementsPossibles(vectorEmplacement);
}

// ----- ECRITURE D'UN Graph DANS UN FICHIER JSON -----
void writeToJson(string output, int gridWidth, int gridHeight) {
	json j2;
	j2["width"] = gridWidth;
	j2["height"] = gridHeight;

	std::ofstream o(output);
	o << std::setw(4) << j2 << std::endl;
}

#endif