#ifndef NODISPRUN_HPP
#define NODISPRUN_HPP

#include <string>
#include "logCSV.hpp"

void readOldFiles(Graphe& G) {
	string nomFichierGraph = "auto21-6";
	string nomFichier = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/automatique/" + nomFichierGraph + ".json";
	//string nomFichier = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/2019/" + nomFichierGraph + ".json";
	std::vector<int> tmpvec = readFromJsonOldGraph(nomFichier, G);
	string nomFichierSlots = nomFichierGraph + "-slots.json";
	string nomFichierGrapheSave = "graph-" + nomFichierGraph + ".json";
	bool save = false;
	if (save) {
		bool savex2 = false;
		bool savex3 = false;
		if (savex2) {
			G.generateMoreEmplacement(2);
			nomFichierSlots = "2X-" + nomFichierSlots;
		}
		else if (savex3) {
			G.generateMoreEmplacement(3);
			nomFichierSlots = "3X-" + nomFichierSlots;
		}
		writeToJsonSlots(G, nomFichierSlots);
		//writeToJsonGraph(G, nomFichierGrapheSave);
	}
}

void multipleFilesRuns(Graphe& G) {
	std::ofstream out("data.txt");
	std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf()); //redirect std::cout
	std::unordered_map<string, std::vector<string>> mapGraphSlots;
	for (int i = 1; i <= 6; i++) {
		mapGraphSlots.insert({ "graph-" + std::to_string(i) + "-input",{std::to_string(i) + "-input-slots","2X-" + std::to_string(i) + "-input-slots","3X-" + std::to_string(i) + "-input-slots"} });
	}
	for (auto key : mapGraphSlots) {
		for (int i = 0; i < key.second.size(); i++) {
			std::cout << "-----------------------------------------" << std::endl;
			std::cout << "Graphe: " << key.first << " " << key.second[i] << std::endl;
			G.clearGraphe();
			string fileGraph = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Graphe/" + key.first + ".json";
			string fileSlots = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Slots/" + key.second[i] + ".json";
			readFromJsonGraph(G, fileGraph);
			readFromJsonSlots(G, fileSlots);
			G.placementAleatoire();
			G.recuitSimule();
		}
	}
	std::cout.rdbuf(coutbuf); //reset to standard output again
}

void allRunsSingleThread() {
	std::unordered_map<string, std::vector<string>> mapGraphSlots;
	//std::vector<string> methodesPlacement = { "Al�atoire", "Glouton", "Glouton Revisit�", "Glouton Gravit�", "Glouton Voisin", "OGDF" };
	std::vector<string> methodesPlacement = { "OGDF" };
	//std::vector<string> methodesAlgo = { "Recuit Simul�",  "Recuit Simul� Delay",  "Recuit Simul� Tournoi Binaire",  "Recuit Simul� Tournoi Multiple",  "Best Deplacement" };
	std::vector<string> methodesAlgo;
	for (int i = 1; i <= 6; i++) {
		mapGraphSlots.insert({ "graph-" + std::to_string(i) + "-input",{std::to_string(i) + "-input-slots","2X-" + std::to_string(i) + "-input-slots","3X-" + std::to_string(i) + "-input-slots"} });
	}
	int nbRuns = 10;
	std::cout << "Starting all run logs, nb run: " << nbRuns << std::endl;
	Graphe G;
	for (auto key : mapGraphSlots) {
		for (int i = 0; i < key.second.size(); i++) {
			std::cout << "-----------------------------------------" << std::endl;
			std::cout << "Graphe: " << key.first << " " << key.second[i] << std::endl;
			G.clearGraphe();
			string fileGraph = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Graphe/" + key.first + ".json";
			string fileSlots = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Slots/" + key.second[i] + ".json";
			readFromJsonGraph(G, fileGraph);
			readFromJsonSlots(G, fileSlots);
			string nomFichierLog = key.first;
			for (int j = 0; j < methodesPlacement.size(); j++) {
				std::cout << "--------------------------" << std::endl;
				std::cout << "Graphe: " << key.first << " Slots: " << key.second[i] << std::endl;
				std::cout << "Placement: " << methodesPlacement[j] << " Algo: Aucun" << std::endl;
				generateCSV(nbRuns, methodesPlacement[j], "Aucun", nomFichierLog, G);
				for (int k = 0; k < methodesAlgo.size(); k++) {
					std::cout << "Placement: " << methodesPlacement[j] << " Algo: " << methodesAlgo[k] << std::endl;
					generateCSV(nbRuns, methodesPlacement[j], methodesAlgo[k], nomFichierLog, G);
				}
			}
		}
	}
}

void allRunsLogged() {
	std::unordered_map<string, std::vector<string>> mapGraphSlots;
	std::vector<string> methodesPlacement = { "Al�atoire", "Glouton", "Glouton Revisit�", "Glouton Gravit�", "Glouton Voisin", "OGDF"};
	//std::vector<string> methodesPlacement = { "Glouton Revisit�" };
	std::vector<string> methodesAlgo = { "Recuit Simul�",  "Recuit Simul� Delay",  "Recuit Simul� Tournoi Binaire",  "Recuit Simul� Tournoi Multiple",  "Best Deplacement", "Recuit Simul� M1",  "Recuit Simul� Delay M1",  "Recuit Simul� Tournoi Binaire M1",  "Recuit Simul� Tournoi Multiple M1", "Recuit Simul� M2",  "Recuit Simul� Delay M2",  "Recuit Simul� Tournoi Binaire M2",  "Recuit Simul� Tournoi Multiple M2" };
	std::vector<int> runMethodesAlgo = { 10, 1, 10, 10, 10,10, 1, 10, 10,10, 1, 10, 10 };
	//std::vector<string> methodesAlgo;
	for (int i = 1; i <= 6; i++) {
		mapGraphSlots.insert({ "graph-" + std::to_string(i) + "-input",{std::to_string(i) + "-input-slots","2X-" + std::to_string(i) + "-input-slots","3X-" + std::to_string(i) + "-input-slots"} });
	}
	std::cout << "Starting all run logs." << std::endl;
	int nthreads, tid;
#pragma omp parallel private(tid)
	{
		Graphe G;
		int indexKey = 0;
		tid = ::omp_get_thread_num();
		nthreads = ::omp_get_num_threads();
		int chunk = std::ceil((double)mapGraphSlots.size() / (double)nthreads);
		if (tid == 0) {
			printf("Number of threads working on training data: %d\n", nthreads);
			printf("Chunk size: %d\n", chunk);
		}
		for (auto key : mapGraphSlots) {
			if (tid == indexKey) {
				for (int i = 0; i < key.second.size(); i++) {
					if (tid == 0) std::cout << "-----------------------------------------" << std::endl;
					if (tid == 0) std::cout << "Graphe: " << key.first << " " << key.second[i] << std::endl;
					G.clearGraphe();
					string fileGraph = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Graphe/" + key.first + ".json";
					string fileSlots = "D:/The World/Cours/M2S1/ProjetDessinGraphe/GitHub/ProjetDessinDeGraphe/exemple/Slots/" + key.second[i] + ".json";
					readFromJsonGraph(G, fileGraph);
					readFromJsonSlots(G, fileSlots);
					string nomFichierLog = key.first;
					for (int j = 0; j < methodesPlacement.size(); j++) {
						if (tid == 0) std::cout << "--------------------------" << std::endl;
						if (tid == 0) std::cout << "Graphe: " << key.first << " Slots: " << key.second[i] << std::endl;
						if (tid == 0) std::cout << "Placement: " << methodesPlacement[j] << " Algo: Aucun" << std::endl;
						generateCSV(1000, methodesPlacement[j], "Aucun", nomFichierLog, G);
						for (int k = 0; k < methodesAlgo.size(); k++) {
							if (tid == 0) std::cout << "Placement: " << methodesPlacement[j] << " Algo: " << methodesAlgo[k] << std::endl;
							generateCSV(runMethodesAlgo[k], methodesPlacement[j], methodesAlgo[k], nomFichierLog, G);
						}
					}
				}
			}
			indexKey++;
		}
		if (tid < mapGraphSlots.size())
			std::cout << "Thread: " << tid << " done." << std::endl;
	}
}

#endif