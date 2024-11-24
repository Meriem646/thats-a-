#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "rental.h"
#include "game.h"
#include "customer.h"
#include <stdlib.h>
#include "index.h"

void insert_rental(TOF *tof, RentalR rental, SparseIndexBlock *sparseIndexBlock) {
    BlocRentalR bloc;
    int read = read_BlocRentalR(tof, &bloc, tof->ent.nbbloc - 1);

    if (read == -1 || bloc.nbr == MAX_RECORDS) {
        // Si le dernier bloc est plein ou inexistant, créer un nouveau bloc
        BlocRentalR newBloc;
        newBloc.tabRental[0] = rental;
        newBloc.nbr = 1;
        write_BlocRentalR(tof, tof->ent.nbbloc, newBloc);

        // Mise à jour de l'index clairsemé
        addSparseIndex(sparseIndexBlock, rental.rentalID, tof->ent.nbbloc, 0);

        // Mise à jour de l'en-tête
        tof->ent.nbbloc++;
        write_entete(tof->ent, tof);
    } else {
        // Ajouter la location au dernier bloc
        bloc.tabRental[bloc.nbr] = rental;
        int rentalIndex = bloc.nbr;
        bloc.nbr++;
        write_BlocRentalR(tof, tof->ent.nbbloc - 1, bloc);

        // Mise à jour de l'index clairsemé
        addSparseIndex(sparseIndexBlock, rental.rentalID, tof->ent.nbbloc - 1, rentalIndex);
    }
}



  void afficher_Toutes_Locations(TOF *tof) {
    BlocRentalR bloc;
    for (int i = 0; i < tof->ent.nbbloc; i++) {
        if (read_BlocRentalR(tof, &bloc, i) == -1) {
            printf("Error reading block %d\n", i);
            return;
        }

        for (int j = 0; j < bloc.nbr; j++) {
            RentalR rental = bloc.tabRental[j];
            Customer customer;
            Game game;

            printf("\n=== Location Details ===\n");
            printf("Rental ID: %d\n", rental.rentalID);
            printf("Customer ID: %d\n", rental.customerID);

            if (rechercher_Customer(tof, rental.customerID, &customer)) {
                printf("Customer Information:\n");
                printf("Name: %s %s\n", customer.firstName, customer.lastName);
                printf("Contact: %s\n", customer.contactInfo);
                printf("Number of Games: %d\n", customer.numberOfGames);
                printf("Total Price: %.2f\n", customer.totalPrice);
            }

            printf("Rented Games:\n");
            // Utiliser les gameIDs de rental pour récupérer les informations des jeux
            for (int k = 0; k < rental.numberOfGames; k++) {
                if (search_game(tof, rental.gameIDs[k], &game)) {
                    printf("Game %d: %s (Price: %.2f)\n", k + 1, game.title, game.price);
                } else {
                    printf("Error: Game ID %d not found.\n", rental.gameIDs[k]);
                }
            }

            printf("Rental Date: %s\n", rental.rentalDate);
            printf("Return Date: %s\n", rental.returnDate);
            printf("====================\n");
        }
    }
}



int search_rental(TOF *tof, int id, RentalR *rental, SparseIndexBlock *sparseIndexBlock) {
    // Rechercher dans l'index clairsemé
    int index = searchSparseIndex(sparseIndexBlock, id);
    if (index != -1) {
        SparseIndex sparse = sparseIndexBlock->sparseIndexes[index];
        
        // Lire le bloc correspondant dans le fichier TOF
        BlocRentalR bloc;
        if (read_BlocRentalR(tof, &bloc, sparse.blocIndex) == -1) {
            return 0; // Erreur de lecture
        }

        // Récupérer la location en utilisant l'index dans le bloc
        *rental = bloc.tabRental[sparse.rentalIndex];
        return 1; // Succès
    }
    return 0; // Location non trouvée
}





int read_BlocRentalR(TOF *tof, BlocRentalR *bloc, int index) {
    FILE *f = fopen(tof->filename, "r+");
    if (f) {
        fseek(f, sizeof(Entete) + index * sizeof(BlocRentalR), SEEK_SET);
        size_t read = fread(bloc, sizeof(BlocRentalR), 1, f);
        fclose(f);
        return (read == 1) ? 1 : -1;
    }
    return -1;
}

int write_BlocRentalR(TOF *tof, int index, BlocRentalR bloc) {
    FILE *f = fopen(tof->filename, "r+");
    if (f) {
        fseek(f, sizeof(Entete) + index * sizeof(BlocRentalR), SEEK_SET);
        size_t written = fwrite(&bloc, sizeof(BlocRentalR), 1, f);
        fclose(f);
        return (written == 1) ? 1 : -1;
    }
    return -1;
}


void printRentalR(RentalR rental) {
    printf("\n=== Rental Information ===\n");
    printf("Rental ID: %d\n", rental.rentalID);
    printf("Customer First Name: %s\n", rental.customer.firstName);
    printf("Customer Last Name: %s\n", rental.customer.lastName);
    printf("Contact Info: %s\n", rental.customer.contactInfo);
    printf("Number of Games: %d\n", rental.numberOfGames);
    printf("Price: %.2f\n", rental.totalPrice);
    printf("========================\n");
}

