#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"

static int G[9][9];
static int nbo;
static Case O[81];
static Cand** C;

void initJeu() {
	printf("Nom du fichier contenant la grille de sudoku :\n");
	char filename[24], *p;
	fgets(filename, sizeof(filename), stdin);
	if ((p = strchr(filename, '\n')) != NULL) {
		*p = '\0'; /* remove newline */
	}

	LireGrille(filename, G);

	printf("\nVoici la grille initiale :\n");
	EcrireGrille(G);
	nbo = GetOpenedSlots(G, O);
	C = GetCandidates(G);
	
	FermerGrille(G, O, nbo, C);
}

int main() {
	initJeu();
}