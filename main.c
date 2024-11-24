#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "customer.h"
#include "rental.h"
#include "game.h"
#include "index.h"

void afficher_menu() {
    printf("\nMenu :\n");
    printf("1. Insérer une location\n");
    printf("2. Afficher toutes les locations\n");
    printf("3. Rechercher une location par ID client\n");
    printf("4. Rechercher une location par ID de location\n");
    printf("0. Quitter\n");
    printf("Choix : ");
}

int main() {
    TOF tof;
    char path[100], mode;
    int choix, id;
    float total_price = 0.0;

    SparseIndexBlock sparseBlock = { .sparseCount = 0 };
    DenseIndexBlock denseBlock = { .denseCount = 0 };

    printf("Entrez le chemin du fichier : ");
    scanf("%s", path);
    printf("Entrez le mode (N pour nouveau fichier, A pour ajout) : ");
    scanf(" %c", &mode);

    open_FILE(&tof, path, mode);

    // Charger les indexes
    loadSparseIndex(&sparseBlock, "sparseIndex.dat");
    loadDenseIndex(&denseBlock, "denseIndex.dat");

    do {
        afficher_menu();
        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n');
            choix = -1;
            continue;
        }
        switch (choix) {
  case 1: {
    RentalR rental;
    rental.rentalID = tof.ent.lastid + 1;

    printf("Entrez l'ID du client : ");
    scanf("%d", &rental.customerID);

    Customer customer;
    customer.customerID = rental.customerID;

    printf("Entrez l'ID du rental : ");
    scanf("%d", &rental.rentalID);

    printf("Entrez le prénom du client : ");
    scanf("%s", customer.firstName);

    printf("Entrez le nom du client : ");
    scanf("%s", customer.lastName);

    printf("Entrez les informations de contact du client : ");
    scanf("%s", customer.contactInfo);

    printf("Combien de jeux voulez-vous louer ? ");
    scanf("%d", &customer.numberOfGames);

    rental.numberOfGames = customer.numberOfGames;

    float total_price = 0.0;
    for (int i = 0; i < customer.numberOfGames; i++) {
        Game game;
        printf("Jeu %d - Titre : ", i + 1);
        scanf(" %[^\n]", game.title);

        printf("Jeu %d - Prix : ", i + 1);
        while (scanf("%f", &game.price) != 1) {
            printf("Entrée invalide. Veuillez entrer un nombre valide : ");
            while (getchar() != '\n');
        }

        total_price += game.price;
        insert_Game(&tof, game);

        // Ajouter chaque gameID à rental.gameIDs
        rental.gameIDs[i] = game.gameID;  // Stockage du gameID pour chaque jeu
    }

    customer.totalPrice = total_price;
    rental.totalPrice = total_price;

    printf("Entrez la date de location (YYYY-MM-DD) : ");
    scanf("%s", rental.rentalDate);

    printf("Entrez la date de retour (YYYY-MM-DD) : ");
    scanf("%s", rental.returnDate);

    // Insérer les informations dans les fichiers
    insert_Customer(&tof, customer, &denseBlock);
    insert_rental(&tof, rental, &sparseBlock);

    // Mettre à jour l'index clairsemé après insertion
    addSparseIndex(&sparseBlock, rental.rentalID, tof.ent.nbbloc - 1, rental.numberOfGames - 1);

    // Mettre à jour l'ID global
    tof.ent.lastid = rental.rentalID;
    write_entete(tof.ent, &tof);

    printf("Location ajoutée avec succès !\n");
    break;
}




            case 2: // Afficher toutes les locations
                printf("Liste des locations :\n");
                afficher_Toutes_Locations(&tof);
                break;

         
            case 3: {
    char fullName[100];
    printf("Entrez le prénom et nom complet du client : ");
    scanf(" %[^\n]", fullName);

    // Recherche dans l'index dense
    int index = searchDenseIndex(&denseBlock, fullName);
    if (index != -1) {
        printf("Client trouvé via l'index dense :\n");
        Customer customer;

        // Récupérer l'ID du client depuis l'index dense
        int customerID = denseBlock.denseIndexes[index].customerID;

        // Utiliser rechercher_Customer pour récupérer toutes les informations du client
        if (rechercher_Customer(&tof, customerID, &customer)) {
            printCustomer(customer); // Afficher les informations complètes du client
        } else {
            printf("Erreur : Impossible de récupérer les informations du client avec ID %d.\n", customerID);
        }
    } else {
        printf("Aucun client trouvé avec ce nom dans l'index dense.\n");
    }
    break;
}






         case 4: {
    printf("Entrez l'ID de la location à rechercher : ");
    scanf("%d", &id);

    // Recherche dans l'index clairsemé
    int sparseIndex = searchSparseIndex(&sparseBlock, id);
    if (sparseIndex != -1) {
        SparseIndex found = sparseBlock.sparseIndexes[sparseIndex];
        RentalR rental;

        // Lire le bloc contenant la location
        BlocRentalR bloc;
        if (read_BlocRentalR(&tof, &bloc, found.blocIndex) != -1) {
            rental = bloc.tabRental[found.rentalIndex];
            printf("Location trouvée via l'index sparse :\n");

            // Afficher les informations de la location
            printf("\n=== Rental Information ===\n");
            printf("Rental ID: %d\n", rental.rentalID);

            // Recherche des informations du client
            Customer customer;
            if (rechercher_Customer(&tof, rental.customerID, &customer)) {
                printf("Customer First Name: %s\n", customer.firstName);
                printf("Customer Last Name: %s\n", customer.lastName);
                printf("Contact Info: %s\n", customer.contactInfo);
            } else {
                printf("Erreur : Informations du client introuvables.\n");
            }

            // Afficher les jeux loués
            printf("Number of Games: %d\n", rental.numberOfGames);
            printf("Price: %.2f\n", rental.totalPrice);

            for (int k = 0; k < rental.numberOfGames; k++) {
                Game game;
                if (search_game(&tof, rental.gameIDs[k], &game)) {
                    printf("Game %d: %s (Price: %.2f)\n", k + 1, game.title, game.price);
                }
            }
            printf("========================\n");
        } else {
            printf("Erreur lors de la lecture du bloc contenant la location.\n");
        }
    } else {
        printf("Aucune location trouvée avec cet ID dans l'index sparse.\n");
    }
    break;
}




            

            case 0:
                saveSparseIndex(&sparseBlock, "sparseIndex.dat");
                saveDenseIndex(&denseBlock, "denseIndex.dat");
                printf("Au revoir !\n");
                break;

            default:
                printf("Choix invalide, veuillez réessayer.\n");
                while (getchar() != '\n');
        }
    } while (choix != 0);

    return 0;
}

