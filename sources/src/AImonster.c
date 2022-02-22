#include <monster.h>
#include <player.h>
#include <map.h>

int xplayer,yplayer;
int xmonster,ymonster;

void monster_smart_moves(struct monster* monster, struct player* player,struct map * map)
{
  xplayer=player_get_x(player);
  yplayer=player_get_y(player);

  xmonster=monster_get_x(monster);
  ymonster=monster_get_y(monster);

  if(xmonster>xplayer&&map_is_inside(map,xmonster-1,ymonster)&&map_get_cell_type(map, xmonster-1, ymonster)==CELL_EMPTY)
    monster_set_current_way(monster,WEST);
  if(xmonster<xplayer&&map_is_inside(map,xmonster+1,ymonster)&&map_get_cell_type(map, xmonster+1, ymonster)==CELL_EMPTY)
    monster_set_current_way(monster,EAST);
  if(ymonster>yplayer&&map_is_inside(map,xmonster,ymonster-1)&&map_get_cell_type(map, xmonster, ymonster-1)==CELL_EMPTY)
    monster_set_current_way(monster,NORTH);
  if(ymonster<yplayer&&map_is_inside(map,xmonster,ymonster+1)&&map_get_cell_type(map, xmonster, ymonster+1)==CELL_EMPTY)
    monster_set_current_way(monster,SOUTH);
}
