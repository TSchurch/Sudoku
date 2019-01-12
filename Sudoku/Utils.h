#pragma once

typedef struct {
	int x;
	int y;
} Case;

typedef struct {
	int nbc;
	int * tab;
} Cand;

void LireGrille(const char *filename, int G[9][9]);
void EcrireGrille(int G[9][9]);
int EstCandidat(int value, Case slot, int G[9][9]);
int Appartient(int value, Case slot, Cand** C);
int EstCandUnique(Case slot, Cand** C);
Case RechCaseUnique(Case O[81], int nbo, Cand** C);
void FermerCase(Case slot, int G[9][9], Case O[81], int *nbo, Cand** C);
void FermerGrille(int G[9][9], Case O[81], int *nbo, Cand** C);
void EcrireCand(Cand** C);

int GetOpenedSlots(int G[9][9], Case slots[81]);
Cand** GetCandidates(int G[9][9]);

void debug_printCandidates(Cand candidate);