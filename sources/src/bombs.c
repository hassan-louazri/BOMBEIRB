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
#include <time.h>
struct bomb{
	int x,y;
	int state;
	int display;
	int time;
};
struct bomb* bomb_init() {
	struct bomb* bomb = malloc(sizeof(*bomb));

	if (!bomb)
		error("Memory error");

	bomb->state = 4;
	bomb->display=0;
	bomb->time=0;
	return bomb;
}
void bomb_set_position(struct bomb *bomb, int x, int y) {
	assert(bomb);
	bomb->x = x;
	bomb->y = y;
}

void bomb_free(struct bomb* bomb) {
	assert(bomb);
	free(bomb);
}
void shouldweseethebomb(struct bomb* bomb,int i){
	bomb->display=i;
}
void bomb_display(struct bomb* bomb) {
	bomb->time++;
	if(bomb->display==1){
		assert(bomb);
		window_display_image(sprite_get_bomb(bomb->state),
				bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
		if(bomb->time%32==0)
			bomb->state--;

	}else{
		
	}
}
void PutRandomBonus(struct game* game,int x, int y){
	srand(time(NULL));
	n=((y+x)+rand())%6+1;
	switch (n) {
	 case 1:
	 	 map_set_cell_type(game_get_current_map(game), x, y, CELL_BONUS_RANGEDEC);
		 break;
	 case 2:
		 map_set_cell_type(game_get_current_map(game), x, y, CELL_BONUS_RANGEINC);
		 break;
	 case 3:
		 map_set_cell_type(game_get_current_map(game), x, y, CELL_BONUS_BOMBINC);
  	 break;
   case 4:
		 map_set_cell_type(game_get_current_map(game), x, y, CELL_BONUS_BOMBDEC);
  	 break;
	 default:
	   map_set_cell_type(game_get_current_map(game), x, y, CELL_BONUS_LIFE);
  	 break;
		}
}
void destroythebomb(struct bomb* bomb, struct game* game)
{
	if(bomb->state==0)
	{
		for (int i = 0; i<= player_get_bomb_range(game_get_player(game)); i++)
		{
			if(map_is_inside(game_get_current_map(game),bomb->x,bomb->y-i) && map_get_cell_type(game_get_current_map(game),bomb->x,bomb->y-i)!=CELL_SCENERY)
			{
				if((map_get_cell_type(game_get_current_map(game),bomb->x,bomb->y-i)==CELL_BOX||map_get_cell_type(game_get_current_map(game),bomb->x,bomb->y-i)==CELL_BONUS)&&rand()%100>0)
				{
						map_set_cell_type(game_get_current_map(game),bomb->x,bomb->y-i,CELL_BONUS);
						break;
				}
				else
				{
						map_set_cell_type(game_get_current_map(game),bomb->x,bomb->y-i,CELL_BOMB);
				}
			}
			else
				break;
		}
		for (int i = 0; i<= player_get_bomb_range(game_get_player(game)); i++)
		{
			if(map_is_inside(game_get_current_map(game),bomb->x,bomb->y+i) && map_get_cell_type(game_get_current_map(game),bomb->x,bomb->y+i)!=CELL_SCENERY)
			{
				if((map_get_cell_type(game_get_current_map(game),bomb->x,bomb->y+i)==CELL_BOX||map_get_cell_type(game_get_current_map(game),bomb->x,bomb->y+i)==CELL_BONUS)&&rand()%100>0)
				{
					map_set_cell_type(game_get_current_map(game),bomb->x,bomb->y+i,CELL_BONUS);
					break;
				}
				else
				{
						map_set_cell_type(game_get_current_map(game),bomb->x,bomb->y+i,CELL_BOMB);
				}
			}
			else
				break;
		}
		for (int i = 0; i<= player_get_bomb_range(game_get_player(game)); i++)
		{
			if((map_is_inside(game_get_current_map(game), bomb->x-i, bomb->y) && map_get_cell_type(game_get_current_map(game), bomb->x-i, bomb->y)!=CELL_SCENERY) && rand()%100>0)
			{
				if(map_get_cell_type(game_get_current_map(game), bomb->x-i, bomb->y) == CELL_BOX||map_get_cell_type(game_get_current_map(game),bomb->x-i,bomb->y) == CELL_BONUS)
				{
					map_set_cell_type(game_get_current_map(game), bomb->x-i, bomb->y, CELL_BONUS);
					break;
				}
				else
				{
					map_set_cell_type(game_get_current_map(game), bomb->x-i, bomb->y, CELL_BOMB);
				}
			}
			else
				break;
		}
		for (int i = 0; i<= player_get_bomb_range(game_get_player(game)); i++)
		{
			if(map_is_inside(game_get_current_map(game), bomb->x+i, bomb->y) && map_get_cell_type(game_get_current_map(game), bomb->x+i, bomb->y) != CELL_SCENERY)
			{
				if((map_get_cell_type(game_get_current_map(game), bomb->x+i, bomb->y) == CELL_BOX || map_get_cell_type(game_get_current_map(game), bomb->x+i, bomb->y)==CELL_BONUS)&&rand()%100>0)
				{
					map_set_cell_type(game_get_current_map(game), bomb->x+i, bomb->y,CELL_BONUS);
					break;
				}
				else
					{
						map_set_cell_type(game_get_current_map(game),bomb->x+i,bomb->y,CELL_BOMB);
					}
			}
			else
				break;
		}


	}
	if(bomb->state<0)
	{
		map_set_cell_type( game_get_current_map(game),bomb->x,bomb->y,CELL_EMPTY);

		for (int i = 0; i<= player_get_bomb_range(game_get_player(game)); i++)
		{
			if(map_is_inside(game_get_current_map(game),bomb->x,bomb->y-i))
			{
				if(map_get_cell_type(game_get_current_map(game),bomb->x,bomb->y-i)==CELL_BOMB)
				{
					map_set_cell_type(game_get_current_map(game),bomb->x,bomb->y-i,CELL_EMPTY);
				}
				else if (map_get_cell_type(game_get_current_map(game),bomb->x,bomb->y-i)==CELL_BONUS)
				{
					PutRandomBonus(game,bomb->x,bomb->y-i);
				}
			}

			if(map_is_inside(game_get_current_map(game),bomb->x,bomb->y+i) )
			{
				if( map_get_cell_type(game_get_current_map(game),bomb->x,bomb->y+i)==CELL_BOMB)
				{
					map_set_cell_type(game_get_current_map(game),bomb->x,bomb->y+i,CELL_EMPTY);
				}
				else if(map_get_cell_type(game_get_current_map(game),bomb->x,bomb->y+i)==CELL_BONUS)
				{
					PutRandomBonus(game,bomb->x,bomb->y+i);
				}
			}
			if(map_is_inside(game_get_current_map(game),bomb->x-i,bomb->y))
			{
				if(map_get_cell_type(game_get_current_map(game),bomb->x-i,bomb->y)==CELL_BOMB)
				{
					map_set_cell_type(game_get_current_map(game),bomb->x-i,bomb->y,CELL_EMPTY);
				}
				else if (map_get_cell_type(game_get_current_map(game),bomb->x-i,bomb->y)==CELL_BONUS)
			 	{
					PutRandomBonus(game,bomb->x-i,bomb->y);
				}
			}
			if(map_is_inside(game_get_current_map(game),bomb->x+i,bomb->y) )
			{
				if(map_get_cell_type(game_get_current_map(game),bomb->x+i,bomb->y)==CELL_BOMB)
				{
					map_set_cell_type(game_get_current_map(game),bomb->x+i,bomb->y,CELL_EMPTY);
				}
				else if(map_get_cell_type(game_get_current_map(game),bomb->x+i,bomb->y)==CELL_BONUS)
				{
					PutRandomBonus(game,bomb->x+i,bomb->y);
				}
			}
		}
		shouldweseethebomb(bomb,0);
	}
}
