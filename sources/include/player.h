/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct player;

int current_level;
int monster_reset_position;
// Creates a new player with a given number of available bombs
struct player* player_init(char* filename);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

int player_get_lives(struct player * player);
int player_get_bombs(struct player * player);
int player_get_bomb_range(struct player * player);
int player_get_keys(struct player * player);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);
void player_dec_nb_lives(struct player* player);
void player_inc_nb_keys(struct player* player);
void player_dec_nb_keys(struct player* player);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

void player_level_success(struct player* player);

#endif /* PLAYER_H_ */
