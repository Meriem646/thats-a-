#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "struct.h"
#include "customer.h"
#include "game.h"
#include "rental.h"

void insert_Game(TOF *tof, Game game) {
    BlocGame bloc;
    int read = read_BlocGame(tof, &bloc, tof->ent.nbbloc - 1);

    // Cas où le dernier bloc est plein ou inexistant
    if (read == -1 || bloc.nbr == MAX_RECORDS) {
        BlocGame newBloc;
        newBloc.tabGame[0] = game;
        newBloc.nbr = 1;
        write_BlocGame(tof, tof->ent.nbbloc, newBloc);
        tof->ent.nbbloc++;
        write_entete(tof->ent, tof);
    } else {
        bloc.tabGame[bloc.nbr] = game;
        bloc.nbr++;
        write_BlocGame(tof, tof->ent.nbbloc - 1, bloc);
    }
}

 





int read_BlocGame(TOF *tof, BlocGame *bloc, int index) {
    FILE *f = fopen(tof->filename, "r+b");
    if (f) {
        fseek(f, sizeof(Entete) + index * sizeof(BlocGame), SEEK_SET);
        size_t read = fread(bloc, sizeof(BlocGame), 1, f);
        fclose(f);
        return (read == 1) ? 1 : -1;
    }
    return -1;
}

int write_BlocGame(TOF *tof, int index, BlocGame bloc) {
    FILE *f = fopen(tof->filename, "r+b");
    if (f) {
        fseek(f, sizeof(Entete) + index * sizeof(BlocGame), SEEK_SET);
        size_t written = fwrite(&bloc, sizeof(BlocGame), 1, f);
        fclose(f);
        return (written == 1) ? 1 : -1;
    }
    return -1;
}

float calculate_total_price(TOF *tof, int *game_ids, int num_games) {
    float total_price = 0.0;
    Game game;

    for (int i = 0; i < num_games; i++) {
        if (search_game(tof, game_ids[i], &game)) {
            total_price += game.price;
        } else {
            printf("Warning: Game ID %d not found.\n", game_ids[i]);
        }
    }

    return total_price;
}


    int search_game(TOF *tof, int id, Game *game) {
    BlocGame bloc;

    // Parcours de tous les blocs de jeux
    for (int i = 0; i < tof->ent.nbbloc; i++) {
        // Lire un bloc de jeux
        if (read_BlocGame(tof, &bloc, i) == -1) {
            return 0;  // Erreur de lecture, retourner 0
        }

        // Chercher chaque jeu dans le bloc
        for (int j = 0; j < bloc.nbr; j++) {
            if (bloc.tabGame[j].gameID == id) {
                *game = bloc.tabGame[j];  // Copier le jeu trouvé
                return 1;  // Succès
            }
        }
    }
    return 0;  // Jeu non trouvé
}




void printGame(Game game) {
    printf("\n=== Game Information ===\n");
    printf("Game ID: %d\n", game.gameID);
    printf("Title: %s\n", game.title);
    printf("Price: %.2f\n", game.price);
    printf("======================\n");
}

