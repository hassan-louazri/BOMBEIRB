/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <map.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <game.h>

struct player {
	int x, y;
	enum direction direction;
	int bombs;
	int range;
	int lives;
	int keys;
};

struct player* player_init(char* filename) {  // pour sauvegarder la partie
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	int bombs;
	int range;
	int lives;
	int keys;

	FILE* fichier = fopen(filename,"r");

	fscanf(fichier,"%d",&lives);
	fscanf(fichier,"%d",&bombs);
	fscanf(fichier,"%d",&range);
	fscanf(fichier,"%d",&keys);
	fscanf(fichier,"%d",&current_level); //game->level

	player->direction = EAST;
	player->bombs = bombs;
	player->range = range;
	player->lives = lives;
	player->keys = keys;


	return player;
}


void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}


void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

int player_get_lives(struct player* player) {
	assert(player);
	if (player->lives < 1)
	{
		printf("GAME OVER\n");
		exit(0);
	}

	return player->lives;

}
int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}

int player_get_bomb_range(struct player* player) {
	assert(player);
	return player->range;

}
void player_inc_nb_bomb(struct player* player) {
	assert(player);
	if (player->bombs > 0 && player -> bombs <= 9)
	{
		player->bombs += 1;
	}
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	if (player->bombs > 0 && player -> bombs <= 9)
	{
		player->bombs -= 1;
	}
}

void player_inc_nb_lives(struct player* player) {
	assert(player);
	if (player->lives > 0 && player -> lives <= 9)
	{
		player->lives += 1;
	}
}

void player_dec_nb_lives(struct player* player) {
	assert(player);
	if (player->lives > 0 && player -> lives <= 9)
	{
		player->lives -= 1;
	}
}

void player_inc_bomb_range(struct player*	player) {
	assert(player);
	if (player->range > 0 && player -> range <= 9)
	{
		player->range += 1;
	}
}

void player_dec_bomb_range(struct player* player) {
	assert(player);
	if (player->range > 0 && player -> range <= 9)
	{
		player->range -= 1;
  }
}
void player_inc_nb_keys(struct player* player) {
	assert(player);
	if (player -> keys <= 9)
	{
		player->keys++;
	}
}

void player_dec_nb_keys(struct player* player) {
	assert(player);
	if (player -> keys <= 9)
	{
		player->keys--;
	}
}

int player_get_keys(struct player* player) {
	assert(player);
	return player->keys;
}

void player_level_success(struct player* player)
{
	assert(player);
	current_level++;
	monster_reset_position = 1;
	player_set_position(player, 1, 1);
}

void Bonuseffects(struct map* map, struct player* player,int x,int y){
	if(map_get_bonus_cell_type(map,x,y)==CELL_BONUS_LIFE)
		player_inc_nb_lives(player);
	if(map_get_bonus_cell_type(map,x,y)==CELL_BONUS_BOMBDEC)
		player_dec_nb_bomb(player);
	if(map_get_bonus_cell_type(map,x,y)==CELL_BONUS_BOMBINC)
		player_inc_nb_bomb(player);
	if(map_get_bonus_cell_type(map,x,y)==CELL_BONUS_RANGEDEC)
		player_dec_bomb_range(player);
	if(map_get_bonus_cell_type(map,x,y)==CELL_BONUS_RANGEINC)
		player_inc_bomb_range(player);
}
static int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;


	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:

		return 0;
		break;

	case CELL_BOX:
		return 1;
		break;


	case CELL_DOOR:
		 	if (current_level < 7)
			{
				player_level_success(player);
			}
			break;

		return 1;
		break;

	case CELL_KEY:
		player_inc_nb_keys(player);
		return 1;
		break;

	case CELL_BONUS:
		Bonuseffects(map,player,x,y);
		break;

	case CELL_MONSTER:
		if(player_get_lives(player)>1)
		{
			player_dec_nb_lives(player);
		}
		else
		{
			printf("GAME OVER\n");
			exit(0);
		}
		break;


	case CELL_BOMB:
		player_dec_nb_lives(player);
		break;


	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;
if(pause==1){
	switch (player->direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			player->x++;
			move = 1;
		}
		break;
	}
}


	if (move)
	{
		if (map_get_cell_type(map, x, y)!=CELL_BOMB)
		{
			map_set_cell_type(map, x, y, CELL_EMPTY);
		}
		if (player->direction == NORTH && map_get_cell_type(map, x, y-1)==CELL_BOX)
		{
			if (map_is_inside(map,x,y-2)==1 && map_get_cell_type(map,x,y-2)==CELL_EMPTY)
			{
				map_set_cell_type(map,x,y-1,CELL_EMPTY);
				map_set_cell_type(map,x,y-2,CELL_BOX);
			}
			else
			{
				player_set_position(player, x, y);
			}
		}
		else if(player->direction == SOUTH && map_get_cell_type(map, x, y+1)==CELL_BOX)
		{
			if(map_is_inside(map,x,y+2) && map_get_cell_type(map, x, y+2)==CELL_EMPTY)
			{
				map_set_cell_type(map, x, y+1, CELL_EMPTY);
				map_set_cell_type(map, x, y+2,  CELL_BOX);
			}
			else
			{
				player_set_position(player, x, y);
			}
		}

		else if(player->direction == WEST && map_get_cell_type(map, x-1, y)==CELL_BOX)
		{
			if(map_is_inside(map,x-2,y) && map_get_cell_type(map, x-2, y)==CELL_EMPTY)
			{
				map_set_cell_type(map, x-2, y,  CELL_BOX);
				map_set_cell_type(map, x-1, y, CELL_EMPTY);
			}
			else
			{
				player_set_position(player, x, y);
			}
		}
		else if (player->direction == EAST && map_get_cell_type(map,x+1,y)==CELL_BOX)
		{
			if(map_is_inside(map,x+2,y)==1 && map_get_cell_type(map, x+2, y)==CELL_EMPTY)
			{
				map_set_cell_type(map,x+1,y,CELL_EMPTY);
				map_set_cell_type(map,x+2,y,CELL_BOX);
			}
			else
			{
				player_set_position(player, x, y);
			}
	}
}

	return move;
}


void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}
