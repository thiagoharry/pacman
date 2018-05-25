/*
Copyright (c) Thiago "Harry" Leucz Astrizi, 2018

This file is part of pacman.

pacman is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

pacman is distributed in the hope that it will be useful,
but WITHOUT ANY  WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS  FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more  details.

You should have received a copy of the GNU Affero General Public License
along with pacman. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _ghosts_h_
#define _ghosts_h_

#include "weaver/weaver.h"
#include "includes.h"

extern int blinky_position_x, blinky_position_y;
extern int pinky_position_x, pinky_position_y;
extern int inky_position_x, inky_position_y;
extern int clyde_position_x, clyde_position_y;
extern bool ghosts_use_global_pellet_counter;
extern struct interface *blinky, *pinky, *inky, *clyde;

void ghosts_init(void);
void ghosts_transform(void);
void ghosts_move(void);
void ghost_carry_pacman(struct interface *, int *, int *, float *, float *);
void ghosts_debug(void);
void ghost_slow_down(struct interface *);
void ghosts_fright(void);
void ghosts_stop_frightned_mode(void);
void ghost_eat_or_get_eaten(struct interface *);


#endif
