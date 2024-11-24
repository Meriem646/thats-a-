#ifndef RENTAL_H
#define RENTAL_H

#include "struct.h"

// Nouvelle déclaration dans rental.h (avec SparseIndexBlock)
void insert_rental(TOF *tof, RentalR rental, SparseIndexBlock *sparseIndexBlock);
int search_rental(TOF *tof, int id, RentalR *rental, SparseIndexBlock *sparseIndexBlock);
int read_BlocRentalR(TOF *tof, BlocRentalR *bloc, int index);
int write_BlocRentalR(TOF *tof, int index, BlocRentalR bloc);
void printRentalR(RentalR rental);
void afficher_Toutes_Locations(TOF *tof);


#endif




