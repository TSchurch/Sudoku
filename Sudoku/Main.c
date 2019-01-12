#include "Utils.h"

static int G[9][9];
static int nbo;
static Case O[81];
static Cand** C;

void initJeu() {
	printf("Nom du fichier contenant la grille de sudoku :\n");
	char filename[24];
	gets(filename);

	LireGrille(filename, G);

	printf("\nVoici la grille initiale :\n");
	EcrireGrille(G);
	nbo = GetOpenedSlots(G, O);
	C = GetCandidates(G);
	
	FermerGrille(G, O, &nbo, C);
}

int main() {
	initJeu();
}