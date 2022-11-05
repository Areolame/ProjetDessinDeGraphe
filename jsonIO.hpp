#ifndef JSONIO
#define JSONIO

#include <string>
#include <nlohmann/json.hpp>

using std::string;
using nlohmann::json;

// ----- CREATION D'UN Graph A PARTIR D'UN FICHIER JSON -----
void readFromJson(string input, int& gridWidth, int& gridHeight) {
	std::ifstream inp(input);
	json j;
	inp >> j;
	gridWidth = j["width"];
	gridHeight = j["height"];
	std::cout << "gridWidth: " << gridWidth << " gridHeight: " << gridHeight << std::endl;

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
	//delete[] nodeTab;
	//delete[] edgeTab;
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