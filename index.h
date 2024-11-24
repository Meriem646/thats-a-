#ifndef INDEX_H
#define INDEX_H
#define MAX_SPARSE_INDEXES 1000  // Nombre maximum d'entrées dans l'index clairsemé

#include "struct.h"

// Gestion des index clairsemés
void addSparseIndex(SparseIndexBlock *sparseIndexBlock, int rentalID, int blocIndex, int rentalIndex);
int searchSparseIndex(SparseIndexBlock *sparseIndexBlock, int rentalID);

// Gestion des index denses
void addDenseIndex(DenseIndexBlock *denseIndexBlock, char *firstName, char *lastName, int customerID);
int searchDenseIndex(DenseIndexBlock *denseIndexBlock, char *fullName);

// Sauvegarde et chargement des index
void saveSparseIndex(SparseIndexBlock *sparseIndexBlock, const char *filename);
void loadSparseIndex(SparseIndexBlock *sparseIndexBlock, const char *filename);
void saveDenseIndex(DenseIndexBlock *denseIndexBlock, const char *filename);
void loadDenseIndex(DenseIndexBlock *denseIndexBlock, const char *filename);

void addDenseIndex(DenseIndexBlock *denseIndexBlock, char *firstName, char *lastName, int customerID);


#endif

