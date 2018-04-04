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

#include <math.h>
#include "pacman.h"

#define PACMAN_SIZE  46.0
#define BASE_SPEED    0.2
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
    image = W.new_interface(6, 100, 100, PACMAN_SIZE,
                            PACMAN_SIZE, "pacman.gif");
    image -> animate = false;
    image -> current_frame = 2;
    image -> integer = RIGHT;
}

void pacman_transform(void){
    float size;
    float rotation = 0.0;
    int screen_x, screen_y;
    float size_multiplier =
        maze_space[position_y][position_x].size_multiplier * (1.0 - offset_y) +
        maze_space[position_y + 1][position_x].size_multiplier * offset_y;
    size =  size_multiplier * PACMAN_SIZE;
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

void pacman_turn_right(void){
    if((offset_y < 0.5 && !maze_walls[position_y][position_x + 1]) ||
       (offset_y > 0.5 && !maze_walls[position_y + 1][position_x + 1])){
        image -> integer = RIGHT;
        image -> animate = true;
    }
}

void pacman_turn_left(void){
    if((offset_y < 0.5 && !maze_walls[position_y][position_x - 1]) ||
       (offset_y > 0.5 && !maze_walls[position_y + 1][position_x - 1])){
        image -> integer = LEFT;
        image -> animate = true;
    }
}

void pacman_turn_up(void){
    if((offset_x < 0.5 && !maze_walls[position_y + 1][position_x]) ||
       (offset_x > 0.5 && !maze_walls[position_y + 1][position_x + 1])){
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
    if(position_x > 12 && position_x < 16 && position_y == 19)
        return; // Não entre na área dos fantasmas
    if((offset_x < 0.5 && !maze_walls[position_y - 1][position_x]) ||
       (offset_x > 0.5 && !maze_walls[position_y - 1][position_x + 1])){
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

static void pacman_half_move(void){
    float movement = BASE_SPEED * W.game -> speed_multiplier;
    switch(image -> integer){
    case LEFT:
        if(offset_y == 0.0){
            if(offset_x == 0.0 &&
                maze_walls[position_y][position_x - 1])
                image -> animate = false;
            else if(maze_walls[position_y][position_x]){
                offset_x = 0.0;
                position_x ++;
            }
            else
                offset_x -= movement;
        }
        else if(offset_y < 0.5){
            if(offset_y > movement)
                offset_y -= movement;
            else{
                float square_offset = offset_y * offset_y;
                offset_x -= sqrtf(movement * movement - square_offset);
                offset_y = 0.0;
            }
        }
        else{
            if((1.0 - offset_y) > movement)
                offset_y += movement;
            else{
                float square_offset = (1.0 - offset_y) * (1.0 - offset_y);
                offset_x -= sqrtf(movement * movement - square_offset);
                offset_y = 0.0;
                position_y ++;
                pellet_eat(position_x, position_y);
            }
        }
        if(offset_x < 0.0){
            position_x --;
            offset_x = 1.0 - offset_x;
            pellet_eat(position_x + 1, position_y);
        }
        break;
    case DOWN_FROM_LEFT:
    case DOWN_FROM_RIGHT:
        if(offset_x == 0.0){
            if(offset_y == 0.0 &&
               maze_walls[position_y - 1][position_x])
                image -> animate = false;
            else if(maze_walls[position_y][position_x]){
                offset_y = 0.0;
                position_y ++;
            }
            else
                offset_y -= BASE_SPEED * W.game -> speed_multiplier;
        }
        else if(offset_x < 0.5){
            if(offset_x > movement)
                offset_x -= movement;
            else{
                float square_offset = offset_x * offset_x;
                offset_y -= sqrtf(movement * movement - square_offset);
                offset_x = 0.0;
            }
        }
        else{
            if((1.0 - offset_x) > movement){
                offset_x += movement;
                if(offset_x > 1.0){
                    offset_x = 1.0 - offset_x;
                    position_x ++;
                    pellet_eat(position_x, position_y);
                }
            }
            else{
                float square_offset = (1.0 - offset_x) * (1.0 - offset_x);
                offset_y -= sqrtf(movement * movement - square_offset);
                offset_x = 0.0;
                position_x ++;
                pellet_eat(position_x, position_y);
                position_y --;
            }
        }
        if(offset_y < 0.0){
            position_y --;
            offset_y = 1.0 - offset_y;
            pellet_eat(position_x, position_y + 1);
        }
        break;
    case RIGHT:
        if(offset_y == 0.0){
            if(!maze_walls[position_y][position_x + 1])
                offset_x += BASE_SPEED * W.game -> speed_multiplier;
            else{
                offset_x = 0.0;
                image -> animate = false;
            }
        }
        else if(offset_y < 0.5){
            if(offset_y > movement)
                offset_y -= movement;
            else{
                float square_offset = offset_y * offset_y;
                offset_x += sqrtf(movement * movement - square_offset);
                offset_y = 0.0;
                pellet_eat(position_x, position_y);
            }
        }
        else{
            if((1.0 - offset_y) > movement){
                offset_y += movement;
                if(offset_y >= 1.0){
                    offset_y = 1.0 - offset_y;
                    position_y ++;
                    pellet_eat(position_x, position_y);
                }
            }
            else{
                float square_offset = (1.0 - offset_y) * (1.0 - offset_y);
                offset_x += sqrtf(movement * movement - square_offset);
                offset_y = 0.0;
                position_y ++;
                pellet_eat(position_x, position_y);
            }
        }
        if(offset_x >= 1.0){
            position_x ++;
            offset_x -= 1.0;
            pellet_eat(position_x, position_y);
        }
        break;
    case UP_FROM_LEFT:
    case UP_FROM_RIGHT:
        if(offset_x == 0.0){
            if(!maze_walls[position_y + 1][position_x])
                offset_y += BASE_SPEED * W.game -> speed_multiplier;
            else{
                offset_y = 0.0;
                image -> animate = false;
            }
        }
        else if(offset_x < 0.5){
            if(offset_x > movement)
                offset_x -= movement;
            else{
                float square_offset = offset_x * offset_x;
                offset_y += sqrtf(movement * movement - square_offset);
                offset_x = 0.0;
                pellet_eat(position_x, position_y);
            }
        }
        else{
            if((1.0 - offset_x) > movement){
                offset_x += movement;
                if(offset_x > 1.0){
                    offset_x = 1.0 - offset_x;
                    position_x ++;
                    pellet_eat(position_x, position_y);
                }
            }
            else{
                float square_offset = (1.0 - offset_x) * (1.0 - offset_x);
                offset_y += sqrtf(movement * movement - square_offset);
                offset_x = 0.0;
                position_x ++;
                pellet_eat(position_x, position_y);
            }
        }
        if(offset_y >= 1.0){
            position_y ++;
            offset_y -= 1.0;
            pellet_eat(position_x, position_y);
        }
        break;
    default:
        return;
    }
    if(image -> integer == LEFT &&
       (position_x < 0 || (position_x == 0 && offset_x == 0))){
        position_x = MAZE_WIDTH - 1;
        offset_x = 0;
    }
    else if(image -> integer == RIGHT && position_x >= MAZE_WIDTH - 1){
        position_x = 0;
        offset_x = 0;
    }
}

void pacman_move(void){
    if(image -> animate){
        pacman_half_move();
        pacman_half_move();
    }
}
