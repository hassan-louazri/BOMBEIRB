/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef BOMBS_H_
#define BOMBS_H_

#include <map.h>
#include <constant.h>


struct bomb;
struct bomb* bomb_init();
void bomb_set_position(struct bomb *bomb, int x, int y);
void bomb_free(struct bomb* bomb);
void bomb_display(struct bomb* bomb);
void shouldweseethebomb(struct bomb* bomb,int i );
void destroythebomb(struct bomb* bomb, struct game* game, int range);

//void bomb_display( struct map* map, int x, int y, unsigned char type,int state);
//void bomb_config(int x,int y);


#endif /*BOMBS_H_*/
