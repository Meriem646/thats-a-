#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "customer.h"
#include "rental.h"
#include "index.h"


void open_FILE(TOF *tof, const char *path, char mode) {
    FILE *f;

    switch (mode) {
        case 'N':
            f = fopen(path, "w+b");  // Changed to binary mode
            if (!f) {
                printf("Error opening file for writing.\n");
                return;
            }
            strcpy(tof->filename, path);
            tof->ent.nbbloc = 0;
            tof->ent.lastid = 0;
            write_entete(tof->ent, tof);
            tof->f = f;
            break;

        case 'A':
            f = fopen(path, "r+b");  // Changed to binary mode
            if (!f) {
                printf("Error opening file for appending.\n");
                return;
            }
            strcpy(tof->filename, path);
            read_entete(tof);
            tof->f = f;
            break;

        default:
            printf("Invalid mode.\n");
            return;
    }
}

void read_entete(TOF *tof) {
    FILE *f = fopen(tof->filename, "r+b");
    if (!f) {
        printf("Error opening file for reading entete.\n");
        return;
    }
    fread(&(tof->ent), sizeof(Entete), 1, f);
    fclose(f);
}

void write_entete(Entete ent, TOF *tof) {
    FILE *f = fopen(tof->filename, "r+b");
    if (!f) {
        printf("Error opening file for writing entete.\n");
        return;
    }
    fwrite(&ent, sizeof(Entete), 1, f);
    fclose(f);
}

void insert_Customer(TOF *tof, Customer c, DenseIndexBlock *denseIndexBlock) {
    BlocCustomer bloc;
    int read = read_BlocCustomer(tof, &bloc, tof->ent.nbbloc - 1);

    // Cas où le dernier bloc est plein ou inexistant
    if (read == -1 || bloc.nbr == MAX_RECORDS) {
        BlocCustomer blocc;
        blocc.tabCust[0] = c;
        blocc.nbr = 1;
        write_BlocCustomer(tof, tof->ent.nbbloc, blocc);
        tof->ent.nbbloc++;
        write_entete(tof->ent, tof);
    } else {
        // Ajout dans un bloc existant
        bloc.tabCust[bloc.nbr] = c;
        bloc.nbr++;
        write_BlocCustomer(tof, tof->ent.nbbloc - 1, bloc);
    }

    // Mise à jour de l'index dense
    addDenseIndex(denseIndexBlock, c.firstName, c.lastName, c.customerID);
}




int rechercher_Customer(TOF *tof, int id, Customer *customer) {
    BlocCustomer bloc;
    for (int i = 0; i < tof->ent.nbbloc; i++) {
        if (read_BlocCustomer(tof, &bloc, i) == -1) {
            printf("Erreur : Lecture du bloc %d échouée.\n", i);
            return 0;
        }

        for (int j = 0; j < bloc.nbr; j++) {
            if (bloc.tabCust[j].customerID == id) {
                *customer = bloc.tabCust[j];
                return 1; // Client trouvé
            }
        }
    }
    return 0; // Client non trouvé
}



   int read_BlocCustomer(TOF *tof, BlocCustomer *bloc, int index) {
    FILE *f = fopen(tof->filename, "r+b");
    if (f) {
        fseek(f, sizeof(Entete) + index * sizeof(BlocCustomer), SEEK_SET);
        size_t read = fread(bloc, sizeof(BlocCustomer), 1, f);
        fclose(f);
        return (read == 1) ? 1 : -1;
    }
    return -1;
}
 



int write_BlocCustomer(TOF *tof, int index, BlocCustomer bloc) {
    FILE *f = fopen(tof->filename, "r+b");
    if (f) {
        fseek(f, sizeof(Entete) + index * sizeof(BlocCustomer), SEEK_SET);
        size_t written = fwrite(&bloc, sizeof(BlocCustomer), 1, f);
        fclose(f);
        return (written == 1) ? 1 : -1;
    }
    return -1;
}

void printCustomer(Customer customer) {
    printf("\n=== Customer Information ===\n");
    printf("Customer ID: %d\n", customer.customerID);
    printf("Name: %s %s\n", customer.firstName, customer.lastName);
    printf("Contact Info: %s\n", customer.contactInfo);
    printf("Number of Games: %d\n", customer.numberOfGames);
    printf("Total Price: %.2f\n", customer.totalPrice);
    printf("==========================\n");
}

