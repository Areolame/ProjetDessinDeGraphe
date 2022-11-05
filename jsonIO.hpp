#ifndef JSONIO
#define JSONIO

#include <string>
#include <nlohmann/json.hpp>

using std::string;
using nlohmann::json;

// ----- CREATION D'UN Graph A PARTIR D'UN FICHIER JSON -----
// Precondition: Les id des noeuds sont ordonnées et commencent par 0
void readFromJsonGraph(string input) {
	std::ifstream inp(input);
	json j;
	inp >> j;

	// Si le fichier ne contient pas de node
	if (j["nodes"] == nullptr) {
		exit(1);
	}

	int nodeNumber = static_cast<int>(j["nodes"].size());
	//node* nodeTab = new node[nodeNumber];
	int edgeNumber = static_cast<int>(j["edges"].size());
	//edge* edgeTab = new edge[edgeNumber];
	int id1, id2;
	for (int i = 0; i < edgeNumber; i++) {
		id1 = j["edges"][i]["source"];
		id2 = j["edges"][i]["target"];
	}
}

// Lecture des slots
// Precondition: Les id des slots sont ordonnés et commencent par 0
void readFromJsonSlots(string input, int& gridWidth, int& gridHeight) {
	std::ifstream inp(input);
	json j;
	inp >> j;

	// Si le fichier ne contient pas de slots
	if (j["slots"] == nullptr) {
		exit(1);
	}

	int slotsNumber = static_cast<int>(j["edges"].size());
	int x, y;
	for (int i = 0; i < slotsNumber; i++) {
		x = j["slots"][i]["x"];
		y = j["slots"][i]["y"];
		if (x > gridWidth) { gridWidth = x; }
		if (y > gridHeight) { gridHeight = y; }
	}

	std::cout << "gridWidth: " << gridWidth << " gridHeight: " << gridHeight << std::endl;
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