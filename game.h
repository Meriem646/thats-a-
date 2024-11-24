#ifndef GAME_H
#define GAME_H

#include "struct.h"

void insert_Game(TOF *tof, Game game);
int search_game(TOF *tof, int id, Game *game);
int read_BlocGame(TOF *tof, BlocGame *bloc, int index);
int write_BlocGame(TOF *tof, int index, BlocGame bloc);
float calculate_total_price(TOF *tof, int *game_ids, int num_games);
void printGame(Game game);

#endif


