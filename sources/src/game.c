/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>
#include <map.h>
#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bombs.h>
#include <monster.h>
#include <AImonster.h>

struct game {
	struct map** maps;       // the game's maps
	short levels;        // nb maps of the game
	short level;					// the game's current level
	struct player* player;
	struct monster** monster;
	int numbreofmonsters;
	struct bomb** bomb;
};

int xbomb=0;
int ybomb=0;
int monster_speed=0;
int pause;
int n=100;

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory
	pause=1;
	monster_reset_position = 0;
	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct game));
	game->maps[0] = get_map_from_file("map/map_0");
	game->maps[1] = get_map_from_file("map/map_1");
	game->maps[2] = get_map_from_file("map/map_2");
	game->maps[3] = get_map_from_file("map/map_3");
	game->maps[4] = get_map_from_file("map/map_4");
	game->maps[5] = get_map_from_file("map/map_5");
	game->maps[6] = get_map_from_file("map/map_6");
	game->maps[7] = get_map_from_file("map/map_7");
	game->levels = 8;
	game->level = current_level; //from player.c
	game->numbreofmonsters=7;

	game->player = player_init("data/player");

	//monsters init

	game->monster=malloc(game->numbreofmonsters*(sizeof(game->monster)));
	for (int  i = 0; i < game->numbreofmonsters; i++)
	{
	game->monster[i]=monster_init();
	}
	//set the default direction of the monsters
	monster_set_current_way(game->monster[0],SOUTH);
	monster_set_current_way(game->monster[1],EAST);
	monster_set_current_way(game->monster[2],NORTH);
	monster_set_current_way(game->monster[3],WEST);
	monster_set_current_way(game->monster[4],SOUTH);
	monster_set_current_way(game->monster[5],NORTH);
	monster_set_current_way(game->monster[6],EAST);



	//bombs init
	game->bomb=malloc(player_get_nb_bomb(game->player)*sizeof(game->bomb));
	for (int i = 0; i < player_get_nb_bomb(game->player); i++) {
		game->bomb[i]=bomb_init();
	}

	// Set default location of the player
	player_set_position(game->player, 1, 0);

	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

int game_get_current_level(struct game* game)
{
	assert(game);
	return game->level;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 5 * SIZE_BLOC) / 12;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_lives(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_bomb_range(game_get_player(game))), x, y);

	x = 4 * white_bloc + 6 * SIZE_BLOC;
	window_display_image(sprite_get_key(), x, y);

	x = 4 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_keys(game_get_player(game))), x, y);

	x = 5 * white_bloc + 8 * SIZE_BLOC;
	window_display_image(sprite_get_number(game_get_current_level(game)), x, y);
}


void make_cell_empty(struct game* game,int x, int y)
{
	map_set_cell_type(game_get_current_map(game),x ,y ,CELL_EMPTY);
}


void game_display(struct game* game) {
	assert(game);
	window_clear();
	game_banner_display(game);
	game->level = current_level;
	if (monster_reset_position == 1)
	{
		monster_set_position(game->monster[0], 9, 0);
		monster_set_position(game->monster[1], 10, 9);
		monster_set_position(game->monster[2], 4, 6);
		monster_set_position(game->monster[3], 8, 2);
		monster_set_position(game->monster[4], 6, 5);
		monster_set_position(game->monster[5], 3, 1);
		monster_set_position(game->monster[6], 0, 11);
		monster_reset_position = 0;
	}
	int range = player_get_bomb_range(game_get_player(game));
	monster_speed++;
	if(pause==1){

			if(monster_speed % 20 == 0)
			{
			for (int  i = 0; i < game->numbreofmonsters && i < game->level; i++) {
				if (game->level != 7)
				{
				monster_move(game->monster[i],game_get_current_map(game),game_get_player(game));
			}
			}
				monster_smart_moves(game->monster[1],game->player,game_get_current_map(game));
				monster_smart_moves(game->monster[4],game->player,game_get_current_map(game));
				monster_smart_moves(game->monster[6],game->player,game_get_current_map(game));
			}
			for (int  i = 0; i < game->numbreofmonsters && i < game->level; i++) {
				if (game->level != 7)
				{
					monster_display(game->monster[i]);
				}
			}


	map_display(game_get_current_map(game));
	player_display(game->player);


	for (int i = 0; i <= 2; i++) {
		if(i!=3){
			bomb_display(game->bomb[i]);
			destroythebomb(game->bomb[i],game,range);
		}
	}
}
	window_refresh();
}

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);


	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
				case SDLK_SPACE:
					if(player_get_nb_bomb(player) > 0)
					{
						player_dec_nb_bomb(player);
						ybomb=player_get_y(player);
						xbomb=player_get_x(player);

						bomb_set_position(game->bomb[player_get_nb_bomb(player)], xbomb, ybomb);
					//	map_set_cell_type(map,xbomb,ybomb,CELL_BOMB);
						shouldweseethebomb(game->bomb[player_get_nb_bomb(player)],1);

					}
					break;
				case SDLK_p:
					 pause=-pause;

					break;

			default:
				break;
			}

			break;
		}
	}
	return 0;
}


int game_update(struct game* game) {
	if (input_keyboard(game))
		return 1; // exit game
	return 0;
}
