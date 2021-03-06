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

#ifndef _maze_h_
#define _maze_h_

#include "weaver/weaver.h"
#include "includes.h"

#define MAZE_WIDTH  30
#define MAZE_HEIGHT 31

struct maze_tile{
  float x, y, size_multiplier;
} maze_space[MAZE_HEIGHT][MAZE_WIDTH];

extern bool maze_walls[MAZE_HEIGHT][MAZE_WIDTH];

void maze_init(void);

#endif
