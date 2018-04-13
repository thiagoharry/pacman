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

#ifndef _pacman_h_
#define _pacman_h_

#include "weaver/weaver.h"
#include "includes.h"

extern int pacman_position_x, pacman_position_y;

void pacman_init(void);
void pacman_transform(void);
void pacman_turn_left(void);
void pacman_turn_right(void);
void pacman_turn_up(void);
void pacman_turn_down(void);
void pacman_move(void);
void pacman_slow_down(float);
void pacman_print_position(void);

#endif
