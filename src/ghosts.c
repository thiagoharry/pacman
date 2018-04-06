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

#include "ghosts.h"

#define GHOST_SIZE 46.0
#define BASE_SPEED    0.2

static struct interface *blinky;
int blinky_position_x, blinky_position_y;
float blinky_offset_x, blinky_offset_y;

void ghosts_init(void){
    blinky = W.new_interface(7, 0, 0, GHOST_SIZE, GHOST_SIZE, "blinky.png");
    blinky_position_x = 14;
    blinky_position_y = 19;
    blinky_offset_x = 0.5;
    blinky_offset_y = 0.0;
}

void ghosts_transform(void){
    perspective_transform(blinky, blinky_position_x, blinky_position_y,
                          blinky_offset_x, blinky_offset_y, GHOST_SIZE);
}
