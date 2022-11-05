#ifndef DISPOPENGL_H
#define DISPOPENGL_H

#include <Windows.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "jsonIO.hpp"
#include <random>

bool move_manual = false;
bool save_current = false;
bool make_copy = false;
bool apply_copy = false;
bool show_grid_size = true;
int currentZoom = 0;
bool showAllEdges = false;
std::vector<std::pair<int, int>> graphCopy;

// Incrément de déplacement du selected node
int dx, dy;

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

// Callback pour OpenGL
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		switch (key) {
			// Fermer l'application
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
		case GLFW_KEY_R:
			show_grid_size = !show_grid_size;
			break;
			// GAUCHE,DROITE,HAUT,BAS déplace le point sélectionné de 1 case dans la direction de la fleche
		case GLFW_KEY_LEFT:
			dx = -1;
			dy = 0;
			move_manual = true;
			break;
		case GLFW_KEY_RIGHT:
			dx = 1;
			dy = 0;
			move_manual = true;
			break;
		case GLFW_KEY_DOWN:
			dy = -1;
			dx = 0;
			move_manual = true;
			break;
		case GLFW_KEY_UP:
			dy = 1;
			dx = 0;
			move_manual = true;
			break;
		case GLFW_KEY_1:
			break;
		case GLFW_KEY_KP_1:
			break;
		case GLFW_KEY_KP_9:
			save_current = true;
			break;
		case GLFW_KEY_F1:
			make_copy = true;
			break;
		case GLFW_KEY_F2:
			apply_copy = true;
			break;
		case GLFW_KEY_KP_ADD:
			//if (currentZoom >= 30)
			currentZoom = currentZoom - 30;
			break;
		case GLFW_KEY_KP_SUBTRACT:
			currentZoom = currentZoom + 30;
			break;
		}
}

void dispOpenGL(Graphe& G, const int gridWidth, const int gridHeight, int maxX, int maxY, string nom_graphe) {

	// Chrono pour le temps d'exec, utilisé pour le stockage de donnée pour la création de graphiques, a supprimer lors de vrai tests
	auto start = std::chrono::system_clock::now();
	auto lastWritten = std::chrono::system_clock::now();
	// NB tour pour le stockage de donnée pour les graphiques, a supprimer lors de vrai executions
	unsigned long long totalTurn = 0;
	unsigned long long lastWrittenTurn = 0;

	//fin ogdf
	if (!glfwInit())
		exit(EXIT_FAILURE);
	GLFWwindow* window = glfwCreateWindow(1820, 980, "Fenetre OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	int width, height;
	glLineWidth(3);
	while (!glfwWindowShouldClose(window)) {
		float ratio;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (show_grid_size) {
			glOrtho(-1 - currentZoom, static_cast<float>(gridWidth) + 1 + currentZoom, -1 - currentZoom, static_cast<float>(gridHeight) + 1 + currentZoom, 1.f, -1.f);
		}
		else {
			//glOrtho(-1, static_cast<float>(maxX) + 1, -1, static_cast<float>(maxY) + 1, 1.f, -1.f);
			glOrtho(-300, static_cast<float>(maxX) + 60, -300, static_cast<float>(maxY) + 60, 1.f, -1.f);
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// Deplacer un noeud aléatoirement
		if (move_manual) {
			//move(vectorNodeBends[selectedNodeBendNum], GL, dx, dy, sommeLong, sommeLong2, variance);
			dx = 0;
			dy = 0;
			move_manual = false;
		}
		else if (save_current) {
			//writeToJson("currentSave.json", G, GL, gridWidth, gridHeight, maxBends);
			std::cout << "Finished saving" << std::endl;
			save_current = false;
		}
		else if (make_copy) {
			graphCopy.clear();
			//graphCopy = copyGraph(vectorNodeBends);
			make_copy = false;
		}
		else if (apply_copy) {
			//applyGraph(graphCopy);
			apply_copy = false;
		}
		// affichage de la grille avec une marge de 1
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		glVertex2d(-1, -1);
		glVertex2d(gridWidth + 1, -1);
		glVertex2d(gridWidth + 1, gridHeight + 1);
		glVertex2d(-1, gridHeight + 1);
		glVertex2d(-1, -1);
		glEnd();
		//afficher les edge
		glColor3f(1.0f, 1.0f, 1.0f);
		for (int i = 0; i < G.getLiens().size(); i++) {
			//glColor3f(1.0f, 1.0f, 1.0f);
			glBegin(GL_LINE_STRIP);
			glVertex2d(G.getLiens()[i].getNoeud1().getX(), G.getLiens()[i].getNoeud1().getY());
			glVertex2d(G.getLiens()[i].getNoeud2().getX(), G.getLiens()[i].getNoeud2().getY());
			glEnd();
		}
		//afficher les nodes
		glPointSize(7);
		glBegin(GL_POINTS);
		for (int i = 0; i < G.getNoeuds().size(); i++) {
			//glColor3f(1.0f, 1.0f, 1.0f);
			glVertex2d(G.getNoeuds()[i].getX(), G.getNoeuds()[i].getY());
			glEnd();
		}
		//afficher les slots
		glColor3f(0.0f, 0.0f, 1.0f);
		for (int i = 0; i < G.getEmplacements().size(); i++) {
			//glColor3f(0.0f, 0.0f, 1.0f);
			glVertex2d(G.getEmplacements()[i].getX(), G.getEmplacements()[i].getY());
			glEnd();
		}
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}

#endif