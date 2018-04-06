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

#include "perspective.h"

void perspective_transform(struct interface *image, int position_x,
                           int position_y, float offset_x, float offset_y,
                           float natural_size){
    float size;
    float rotation = 0.0;
    int screen_x, screen_y;
    float size_multiplier =
        maze_space[position_y][position_x].size_multiplier * (1.0 - offset_y) +
        maze_space[position_y + 1][position_x].size_multiplier * offset_y;
    size =  size_multiplier * natural_size;
    screen_x = maze_space[position_y][position_x].x * (1.0 - offset_x) +
        maze_space[position_y][position_x + 1].x * offset_x;
    screen_y = maze_space[position_y][position_x].y * (1.0 - offset_y) +
        maze_space[position_y + 1][position_x].y * offset_y;
    W.resize_interface(image, size, size);
    W.move_interface(image, screen_x, screen_y);
    if(position_x == 0)
        image -> a = (1.0 - offset_x);
    else if(position_x == MAZE_WIDTH - 2)
        image -> a = offset_x;
    else
        image -> a = 1.0;
    if(image -> a < 0.2)
        image -> a = 0.2;
    // Rotação
    if(image -> integer != LEFT && image -> integer != RIGHT)
        rotation = (M_PI * (position_x - 15)) / 180.0;
    W.rotate_interface(image, rotation);
}
