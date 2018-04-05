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

#include "fruits.h"

static struct interface *cherry;
static bool initialized = false;
static struct sound *eat;

#define FRUIT_SIZE 33.992715

void fruits_init(void){
    int x_pos = (maze_space[13][14].x + maze_space[13][15].x) / 2;
    int y_pos = maze_space[13][14].y;
    if(initialized)
        return;
    cherry = W.new_interface(5, x_pos, y_pos, FRUIT_SIZE, FRUIT_SIZE, "cherry.png");
    // Sound under Creative Commons by Jason Shaw at audionautix.com
    eat = W.new_sound("bite1.wav");
    cherry -> visible = false;
    initialized = true;
}

void fruits_appear(void){
    cherry -> visible = true;
    W.run_futurelly(fruits_disappear, 9.5);
}

void fruits_disappear(void){
    cherry -> visible = false;
}

bool fruits_are_visible(void){
    return cherry -> visible;
}

void fruits_get_eaten(void){
    cherry -> visible = false;
    W.play_sound(eat);
}
