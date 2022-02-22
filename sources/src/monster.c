#include <SDL/SDL_image.h>
#include <assert.h>
#include <map.h>
#include <monster.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include "AImonster.h"

struct monster {
	int x, y;
	enum direction direction;
	int display;
};

struct monster* monster_init(int nombre) {
	struct monster* monster = malloc(sizeof(*monster));

	if (!monster)
		error("Memory error");

	monster->display=1;
	monster->direction = EAST;
	return monster;
}
void monster_set_position(struct monster *monster, int x, int y) {
	assert(monster);
	monster->x = x;
	monster->y = y;

}

void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}


static int monster_move_aux(struct monster* monster, struct map* map, int x, int y,struct player* player) {


	if(x==player_get_x(player)&&y==player_get_y(player)){
		player_dec_nb_lives(player);
		return 1;
	}
	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		monster_set_current_way(monster,WEST);
		return 1;
		break;

	case CELL_BOX:
		return 1;
		break;

	case CELL_DOOR:
		
		return 0;
		break;

	case CELL_BONUS:
		break;

	case CELL_MONSTER:
		break;


	case CELL_BOMB:
		monster->display=0;
		break;

	default:
		break;
	}
	return 1;
}
int monster_move(struct monster* monster, struct map* map,struct player* player) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;
	map_set_cell_type(map, x, y, CELL_EMPTY);
	switch (monster->direction) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1,player)) {
			monster->y--;
			y = monster->y;
			move = 1;
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1,player)) {
			monster->y++;
			y = monster->y;
			move = 1;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y,player)) {
			monster->x--;
			x = monster->x;
			move = 1;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y,player)) {
			monster->x++;
			x = monster->x;
			move = 1;
		}
		break;
	}

	map_set_cell_type(map, x, y, CELL_MONSTER);
	if (move){


		//if (map_get_cell_type(map, x, y)!=CELL_BOMB)
		//{
	//		map_set_cell_type(map, x, y, CELL_EMPTY);
		//}
		if (monster->direction==NORTH &&(!map_is_inside(map,x,y-1) || map_get_cell_type(map, x, y-1)!=CELL_EMPTY))
		{
			monster_set_current_way(monster,SOUTH);
		}
		if (monster->direction==SOUTH &&(!map_is_inside(map,x,y+1)|| map_get_cell_type(map, x, y+1)!=CELL_EMPTY))
		{
			monster_set_current_way(monster,NORTH);
		}
		if (monster->direction==EAST &&(!map_is_inside(map,x+1,y)|| map_get_cell_type(map, x+1, y)!=CELL_EMPTY))
		{
			monster_set_current_way(monster,WEST);
		}
		if (monster->direction==WEST &&(!map_is_inside(map,x-1,y)|| map_get_cell_type(map, x-1, y)!=CELL_EMPTY))
		{
			monster_set_current_way(monster,EAST);
		}

	}

	return move;
}
void monster_display(struct monster* monster) {
	assert(monster);
		if(monster->display==1){
	window_display_image(sprite_get_monster(monster->direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
		}
}
