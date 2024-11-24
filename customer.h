#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "game.h"

#include "struct.h"
void open_FILE(TOF *tof, const char *path, char mode);
void read_entete(TOF *tof);
void write_entete(Entete ent, TOF *tof);
void insert_Customer(TOF *tof, Customer c, DenseIndexBlock *denseIndexBlock);  // Mise à jour
int rechercher_Customer(TOF *tof, int id, Customer *customer);
int read_BlocCustomer(TOF *tof, BlocCustomer *bloc, int index);
int write_BlocCustomer(TOF *tof, int index, BlocCustomer bloc);
void printCustomer(Customer customer);


#endif




