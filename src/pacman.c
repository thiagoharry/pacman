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

#include "pacman.h"

#define PACMAN_SIZE  46.0
#define BASE_SPEED    0.4
#define LEFT            0
#define RIGHT           1
#define UP_FROM_LEFT    2
#define UP_FROM_RIGHT   3
#define DOWN_FROM_LEFT  4
#define DOWN_FROM_RIGHT 5

static struct interface *image;
static int position_x, position_y;
static float offset_x, offset_y;

void pacman_init(void){
    position_x = 14;
    position_y = 7;
    offset_x = 0.5;
    offset_y = 0.0;
    image = W.new_interface(5, 100, 100, PACMAN_SIZE,
                            PACMAN_SIZE, "pacman.gif");
    image -> animate = false;
    image -> current_frame = 2;
    image -> integer = RIGHT;
}

void pacman_transform(void){
    float size;
    int screen_x, screen_y;
    float size_multiplier =
        maze_space[position_y][position_x].size_multiplier;
    size =  size_multiplier * PACMAN_SIZE;
    screen_x = maze_space[position_y][position_x].x + offset_x *
        TILE_SIZE * size_multiplier;
    screen_y = maze_space[position_y][position_x].y;
    W.resize_interface(image, size, size);
    W.move_interface(image, screen_x, screen_y);
}

void pacman_turn_right(void){
    if((offset_y == 0.0 && !maze_walls[position_y][position_x + 1]) ||
       (offset_y > 0.0 && !maze_walls[position_y + 1][position_x + 1])){
        image -> integer = RIGHT;
        image -> animate = true;
    }
}

void pacman_turn_left(void){
    if((offset_y == 0.0 && !maze_walls[position_y][position_x - 1]) ||
       (offset_y > 0.0 && !maze_walls[position_y + 1][position_x - 1])){
        image -> integer = LEFT;
        image -> animate = true;
    }
}

void pacman_turn_up(void){
    if((offset_x == 0.0 && !maze_walls[position_y + 1][position_x]) ||
       (offset_x > 0.0 && !maze_walls[position_y + 1][position_x + 1])){
        if(image -> integer == UP_FROM_LEFT || image -> integer == UP_FROM_RIGHT)
            return;
        else if(image -> integer == LEFT || image -> integer == DOWN_FROM_RIGHT)
            image -> integer = UP_FROM_LEFT;
        else
            image -> integer = UP_FROM_RIGHT;
        image -> animate = true;
    }
}

void pacman_turn_down(void){
    if((offset_x == 0.0 && !maze_walls[position_y - 1][position_x]) ||
       (offset_x > 0.0 && !maze_walls[position_y - 1][position_x + 1])){
        if(image -> integer == DOWN_FROM_LEFT ||
           image -> integer == DOWN_FROM_RIGHT)
            return;
        else if(image -> integer == LEFT || image -> integer == UP_FROM_RIGHT)
            image -> integer = DOWN_FROM_LEFT;
        else
            image -> integer = DOWN_FROM_RIGHT;
        image -> animate = true;
    }
}
