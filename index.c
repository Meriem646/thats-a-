#include "index.h"
#include "game.h"
#include <stdio.h>
#include <string.h>
#include "customer.h"


// Ajouter une entrée dans l'index clairsemé
void addSparseIndex(SparseIndexBlock *sparseBlock, int rentalID, int blocIndex, int rentalIndex) {
    if (sparseBlock->sparseCount < MAX_SPARSE_INDEXES) {
        sparseBlock->sparseIndexes[sparseBlock->sparseCount].rentalID = rentalID;
        sparseBlock->sparseIndexes[sparseBlock->sparseCount].blocIndex = blocIndex;
        sparseBlock->sparseIndexes[sparseBlock->sparseCount].rentalIndex = rentalIndex;
        sparseBlock->sparseCount++;
    } else {
        printf("Erreur : Index clairsemé plein.\n");
    }
}


// Rechercher une location dans l'index clairsemé
int searchSparseIndex(SparseIndexBlock *sparseBlock, int rentalID) {
    for (int i = 0; i < sparseBlock->sparseCount; i++) {
        if (sparseBlock->sparseIndexes[i].rentalID == rentalID) {
            return i;  // Retourner l'index dans l'index clairsemé
        }
    }
    return -1;  // Location non trouvée
}

// Ajouter une entrée dans l'index dense
void addDenseIndex(DenseIndexBlock *denseIndexBlock, char *firstName, char *lastName, int customerID) {
    if (denseIndexBlock->denseCount >= MAX_CUSTOMERS) {
        printf("Dense index is full. Cannot add more entries.\n");
        return;
    }

    DenseIndex newIndex;
    snprintf(newIndex.fullName, sizeof(newIndex.fullName), "%s %s", firstName, lastName);
    newIndex.customerID = customerID;

    denseIndexBlock->denseIndexes[denseIndexBlock->denseCount++] = newIndex;
}

// Rechercher un client dans l'index dense
int searchDenseIndex(DenseIndexBlock *denseIndexBlock, char *fullName) {
    for (int i = 0; i < denseIndexBlock->denseCount; i++) {
        if (strcmp(denseIndexBlock->denseIndexes[i].fullName, fullName) == 0) {
            return i; // Retourne l'index trouvé
        }
    }
    return -1; // Non trouvé
}

// Sauvegarder l'index clairsemé dans un fichier
void saveSparseIndex(SparseIndexBlock *sparseIndexBlock, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error saving sparse index to file: %s\n", filename);
        return;
    }

    fwrite(sparseIndexBlock, sizeof(SparseIndexBlock), 1, file);
    fclose(file);
}

// Charger l'index clairsemé depuis un fichier
void loadSparseIndex(SparseIndexBlock *sparseIndexBlock, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error loading sparse index from file: %s\n", filename);
        return;
    }

    fread(sparseIndexBlock, sizeof(SparseIndexBlock), 1, file);
    fclose(file);
}

// Sauvegarder l'index dense dans un fichier
void saveDenseIndex(DenseIndexBlock *denseIndexBlock, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error saving dense index to file: %s\n", filename);
        return;
    }

    fwrite(denseIndexBlock, sizeof(DenseIndexBlock), 1, file);
    fclose(file);
}

// Charger l'index dense depuis un fichier
void loadDenseIndex(DenseIndexBlock *denseIndexBlock, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error loading dense index from file: %s\n", filename);
        return;
    }

    fread(denseIndexBlock, sizeof(DenseIndexBlock), 1, file);
    fclose(file);
}

