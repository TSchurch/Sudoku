#define _CRT_SECURE_NO_WARNINGS
#define true 1
#define false 0

#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>

/*
	LireGrille
	char filename: Le nom du fichier qui contient la grille
	int[9][9] G: Le tableau qui contiendra la grille

	Lit la grille d'un fichier texte et la transcode
	dans un tableau interpretable plus facilement.
*/
void LireGrille(const char *filename, int G[9][9]) {
	FILE *fp;

	// Ouvrir le fichier en mode read
	fp = fopen(filename, "r");
	
	// Gerer les eventuelles erreurs
	if (fp == NULL) {
		printf("Could not read file, err: %d\n", errno);
		exit(1);
	}

	// Initialization de la positions des chiffres
	int ch;
	int x = 0;
	int y = 0;

	// Tant que le fichier n'est pas vide
	while ((ch = fgetc(fp)) != EOF) {
		x++;
		// Si nouvelle ligne
		if (ch == (int)'\n') {
			x = 0;
			y++;
		}
		else {
			switch (y) {
			case 0: break;	// |
			case 4: break;  // |
			case 8: break;  // |
			case 12: break; // | Ignorer les lignes
			default:
				// Ignorer la colonne
				if (ch == (int)'|') {
					x--;
				}
				else {
					// Recuperer le chiffre et la stocker dans le tableau
					int yOffset = y <= 4 ? 1 : (y <= 8 ? 2 : 3);
					G[y-yOffset][x-1] = (char)ch - '0';
				}
			}
		}
	}

	// Fermer le fichier
	fclose(fp);
}

/*
	EcrireGrille
	int[9][9] G: Le tableau qui contient la grille

	Va ecrire sur le terminal la grille G
*/
void EcrireGrille(int G[9][9]) {
	printf("+---------+---------+---------+\n");
	for (int y = 0; y <= 8; y++) {
		if (y == 3 || y == 6) printf("|         |         |         |\n+---------+---------+---------+\n");
		printf("|         |         |         |\n|");
		for (int x = 0; x <= 8; x++) {
			if (x == 3 || x == 6) printf("|");
			G[y][x] == 0 ? printf(" _ ") : printf(" %d ", G[y][x]);
		}
		printf("|\n");
	}
	printf("|         |         |         |\n+---------+---------+---------+\n");
}

/*
	get
	Cand c: L'objet candidat contenant tout les candidats
	int i: La position du candidat que l'on veut.

	Recupere le candidat a la position i sinon retourne " "
*/
char get(Cand c, int i) {
	if (c.nbc > i) {
		return c.tab[i]+'0';
	} else {
		return ' ';
	}
}

/*
	EcrireCand
	Cand** C: Le tableau des candidats

	Ecrit les candidats de chaque cases dans le terminal
*/
void EcrireCand(Cand** C) {
	printf("\n");
	printf("*************************************************************************\n");
	for (int y = 0; y <= 8; y++) {
		if (y == 3 || y == 6) printf("*************************************************************************\n");
		if (y > 0 && y <= 2) printf("*-------+-------+-------*-------+-------+-------*-------+-------+-------*\n");
		if (y > 3 && y <= 5) printf("*-------+-------+-------*-------+-------+-------*-------+-------+-------*\n");
		if (y > 6 && y <= 8) printf("*-------+-------+-------*-------+-------+-------*-------+-------+-------*\n");
		for (int x = 0; x <= 8; x++) {
			if (x == 0 || x == 3 || x == 6) printf("*");
			Cand c = C[y][x];
			printf(" %c%c%c%c%c ", get(c, 4), get(c, 3), get(c, 2), get(c, 1), get(c, 0));
			if (x >= 0 && x <= 1) printf("|");
			if (x >= 3 && x <= 4) printf("|");
			if (x >= 6 && x <= 7) printf("|");
			if (x == 8) printf("*\n");
		}
	}
	printf("*************************************************************************\n");
}

/*
	GetOpenedSlots
	int[9][9] G: Le tableau qui contient la grille
	Case[81] slots: Le tableau qui contiendra les cases ouvertes

	return int: Retourne le nombre de cases ouvertes

	Recupere toute les cases ouvertes et les mets dans slots
*/
int GetOpenedSlots(int G[9][9], Case slots[81]) {
	int curPos = 0 ;
	for (int y = 0; y <= 8; y++) {
		for (int x = 0; x <= 8; x++) {
			if (G[y][x] == 0) {
				//printf("Found opened slot at x:%d y:%d writing it at pos %d \n", x, y, curPos);
				slots[curPos] = (Case) { x = x, y = y };
				curPos++;
			}
		}
	}
	return curPos;
}

/*
	EstCandidat
	int value: La valeur à tester
	Case slot: La case à tester
	int[9][9] G: Le tableau contenant la grille

	return bool: Si oui ou non la valeur peut etre candidate pour la case 
*/
int EstCandidat(int value, Case slot, int G[9][9]) {
	int isCandidate = true;

	// check x axis
	for (int x = 0; x <= 8; x++) {
		if (G[slot.y][x] == value) {
			isCandidate = false;
		}
	}

	// check y axis
	for (int y = 0; y <= 8; y++) {
		if (G[y][slot.x] == value) {
			isCandidate = false;
		}
	}

	// check quadrant
	int quadrantX = getQuadrantStart(slot.x);
	int quadrantY = getQuadrantStart(slot.y);

	for (int x = quadrantX; x <= quadrantX + 2; x++) {
		for (int y = quadrantY; y <= quadrantY + 2; y++) {
			if (G[y][x] == value) isCandidate = false;
		}
	}

	return isCandidate;
}

/*
	Appartient
	int value: La valeur a verifier si elle est candidate
	Case slot: La case a verifier
	Cand** slot: L'objet candidat a tester
	
	return bool: Si oui ou non la valeur est dans l'objet candidat 
*/
int Appartient(int value, Case slot, Cand** C)
{
	int appartient = false;
	for (int i = 0; i < C[slot.y][slot.x].nbc; i++) {
		if (C[slot.y][slot.x].tab[i] == value) appartient = true;
	}
	return appartient;
}


/*
	EstCandidatUnique
	Case slot: La case a tester
	Cand** C: La grille des candidats
	
	return bool: si oui ou non la case a qu'un seul candidat
*/
int EstCandUnique(Case slot, Cand ** C)
{
	return C[slot.y][slot.x].nbc == 1;
}

/*
	RechCaseUnique
	Case[81] O: Le tableau contenant les cases ouvertes
	int nbo: Le nombre de cases ouvertes
	Cand** C: La grille dans candidats

	return Case: Retourne une case avec un candidat unique
*/
Case RechCaseUnique(Case O[81], int nbo, Cand ** C)
{
	for (int i = 0; i <= nbo; i++) {
		if (EstCandUnique(O[i], C)) return O[i];
	}

	return (Case) { -1, -1 };
}

/*
	FermerCase
	Case slot: La case a fermer
	int[9][9] G: Le tableau contenant la grille
	Case[81] O: Le tableau contenant les cases ouvertes
	int nbo: Le nombre de cases ouvertes
	Cand** C: La grille des candidats

	Ferme la case slot
*/
void FermerCase(Case slot, int G[9][9], Case O[81], int *nbo, Cand** C)
{
	G[slot.y][slot.x] = C[slot.y][slot.x].tab[0];
	C[slot.y][slot.x].nbc = 0;
	C[slot.y][slot.x].tab = NULL;

	// find current index
	int index;
	for (int i = 0; i <= *nbo; i++) {
		if (O[i].x == slot.x && O[i].y == slot.y) {
			index = i;
		}
	}

	*nbo = *nbo-1;
}

/*
	FermerGrille
	int[9][9] G: Le tableau contenant la grille
	Case[81] O: Le tableau des cases ouvertes
	int nbo: Le nombre de cases ouvertes
	Cand** C: Le tableau des candidats

	Ferme la grille G
*/
void FermerGrille(int G[9][9], Case O[81], int *nbo, Cand ** C)
{
	while (*nbo >= 1) {
		printf("\nVoici les candidats des %d cases ouvertes de la grille : \n", *nbo);
		EcrireCand(C);
		printf("\nElimination des candidats unique...\n");
		Case slot = RechCaseUnique(O, nbo, C);
		int value = C[slot.y][slot.x].tab[0];
		FermerCase(slot, G, O, nbo, C);
		printf("\nla case (%d,%d) est fermee avec le chiffre %d\n\n", slot.x, slot.y, value);
		EcrireGrille(G);
		C = GetCandidates(G);
	}
}

/*
	getQuadrantStart
	int pos: La position de l'element

	return int: le debut de la grille de l'element
*/
int getQuadrantStart(int pos) {
	if (pos >= 0 && pos <= 2) {
		return 0;
	} else if (pos >= 3 && pos <= 5) {
		return 3;
	} else {
		return 6;
	}
}

/*
	GetCandidates
	int[9][9] G: Le tableau contenant la grille

	return Cand**: Le tableau contenant les candidats
*/
Cand** GetCandidates(int G[9][9]) {
	Cand **candidates = (Cand **)malloc(9 * sizeof(Cand *));
	for (int i = 0; i < 9; i++)
		candidates[i] = (Cand *)malloc(9 * sizeof(Cand));

	for (int y = 0; y <= 8; y++) {
		for (int x = 0; x <= 8; x++) {
			if (G[y][x] == 0) {
				int *tbl = (int *)malloc(9 * sizeof(int));
				int nbc = -1;
				for (int i = 1; i <= 9; i++) {
					if (EstCandidat(i, (Case) { x, y }, G)) {
						nbc++;
						tbl[nbc] = i;
					}
				}				
				candidates[y][x] = (Cand) { nbc+1, tbl };
			} else {
				candidates[y][x] = (Cand) { 0, NULL };
			}
		}
	}

	return candidates;
}

void debug_printCandidates(Cand candidate){
	printf("%d candidates: \n", candidate.nbc);
	for (int i=0; i < candidate.nbc; i++) {
		printf("	- %d\n", candidate.tab[i]);
	}
	printf("\n");
}