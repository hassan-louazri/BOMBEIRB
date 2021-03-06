/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <game.h>
#include <bombs.h>

struct map {
	int width;
	int height;
	unsigned char* grid;
};


#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);

  if(0 <= x && x < map->width && 0 <= y && y < map->height)
	{
          return 1;
  }
	else
	{
          return 0;
  }
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}
enum compose_type map_get_bonus_cell_type(struct map* map, int x, int y)
{
	//compose_type
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}
enum scenery_type map_get_cell_princess(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] == CELL_PRINCESS;
}

enum compose_type map_get_door_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}
void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;
		case BONUS_LIFE:
			window_display_image(sprite_get_banner_life(),x,y);
			break;
		default:
			window_display_image(sprite_get_bomb(0), x,y);
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	case SCENERY_PRINCESS:
		window_display_image(sprite_get_princess(), x, y);
	}
}

void display_door(struct map* map, int x, int y, unsigned char type)
{
	switch (type >> 7)
	{
		case CELL_DOOR:
			window_display_image(sprite_get_door_opened(), x, y);
			break;
		default:
			window_display_image(sprite_get_door_closed(), x, y);
			break;
	}
}
void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];

	    switch (type & 0xf0) {
			case CELL_SCENERY:
		  	display_scenery(map, x, y, type);
		  	break;
	    case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;
	    case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	    case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
			case CELL_DOOR:
	      window_display_image(sprite_get_door_opened(), x, y);
				break;
			case CELL_BOMB:
				window_display_image(sprite_get_bomb(0), x,y);
				break;

	    }
	  }
	}
}


struct map* get_map_from_file(char* filename)
{
	FILE* fichier;
	int width;
	int height;
	int cell;

	fichier = fopen(filename,"r");

	if (fichier == NULL)
	{
		printf("ERROR : EMPTY MAP FILE");
	}

	fscanf(fichier,"%d",&height);
	fseek(fichier, 1, SEEK_CUR);
	fscanf(fichier,"%d",&width);

	struct map* map = map_new(width,height);

	map->grid = malloc(width * height);
	for (int i = 0; i < width * height ; i++)
	{
			fscanf(fichier,"%d",&cell);

			map->grid[i] = cell;
	}
	return map;
}
