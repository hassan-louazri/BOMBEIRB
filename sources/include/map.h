/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000 decimal = 32
	CELL_DOOR=0x30,      	//  0011 0000 decimal >= 48
	CELL_KEY=0x40,       	//  0100 0000 decimal = 64
	CELL_BONUS=0x50, 		// 	0101 0000 decimal = 80
	CELL_MONSTER=0x60, 		// 	0110 0000 decimal = 96
	CELL_BOMB=0x70 	   		// 	0111 0000 deciaml = 112

};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_MONSTER,
	BONUS_LIFE
};

enum door_type {
	DOOR_CLOSED = 0,
	DOOR_OPENED = 1
};

enum door_to_map {
	CELL_DOOR_1 = 0,
	CELL_DOOR_2 = 2,
	CELL_DOOR_3 = 4,
	CELL_DOOR_4 = 6,
	CELL_DOOR_5 = 10,
	CELL_DOOR_6 = 12,
	CELL_DOOR_7 = 14
};

enum scenery_type {
	SCENERY_STONE = 1,    // 0001 decimal = 17
	SCENERY_TREE  = 2,    // 0010 decimal = 18
	SCENERY_PRINCESS = 4  // 0100 decimal = 20
};

enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

		CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
		CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_INC,
		CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_DEC,
		CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_INC,
		CELL_BOX_LIFE     = CELL_BOX | BONUS_MONSTER,
		CELL_BOX_MONSTER  = CELL_BOX | BONUS_LIFE,


		CELL_BONUS_RANGEINC = CELL_BONUS | BONUS_BOMB_RANGE_INC,
		CELL_BONUS_RANGEDEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC,
		CELL_BONUS_BOMBINC  = CELL_BONUS | BONUS_BOMB_NB_INC,
		CELL_BONUS_BOMBDEC  = CELL_BONUS | BONUS_BOMB_NB_DEC,
		CELL_BONUS_LIFE     = CELL_BONUS | BONUS_LIFE,
		CELL_BONUS_MONSTER  = CELL_BONUS | BONUS_MONSTER,

		CELL_DOOR_CLOSED = CELL_DOOR | DOOR_CLOSED,
		CELL_DOOR_OPENED = CELL_DOOR | DOOR_OPENED
};

struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);
enum compose_type map_get_bonus_cell_type(struct map* map, int x, int y);
enum compose_type map_get_door_type(struct map* map, int x, int y);
// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);


// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_static();

// Display the map on the screen
void map_display(struct map* map);


struct map* get_map_from_file(char* filename);

#endif /* MAP_H_ */
