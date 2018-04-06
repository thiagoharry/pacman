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

static struct interface *image;
static int position_x, position_y;
static float offset_x, offset_y;
static float slow_down;
static float default_speed[21][2] = {
    {0.8, 0.9}, {0.9, 0.95}, {0.9, 0.95}, {0.9, 0.95}, {1.0, 1.0}, {1.0, 1.0},
    {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0},
    {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0},
    {1.0, 1.0}, {1.0, 1.0}, {0.9, 0.9}};

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
    slow_down = 1.0;
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
    float begin_slow_down = slow_down;
    float movement;

    float prev_x = offset_x, prev_y = offset_y;

    if(slow_down < default_speed[W.game -> level - 1][0])
        movement = slow_down * BASE_SPEED;
    else
        movement = default_speed[W.game -> level - 1][0] * BASE_SPEED;
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
            if(offset_y >= movement)
                offset_y -= movement;
            else{
                float square_offset = offset_y * offset_y;
                offset_x -= sqrtf(movement * movement - square_offset);
                offset_y = 0.0;
            }
        }
        else{
            if((1.0 - offset_y) >= movement)
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
            offset_x = 1.0 + offset_x;
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
                offset_y -= movement;
        }
        else if(offset_x < 0.5){
            if(offset_x >= movement)
                offset_x -= movement;
            else{
                float square_offset = offset_x * offset_x;
                offset_y -= sqrtf(movement * movement - square_offset);
                offset_x = 0.0;
            }
        }
        else{
            if((1.0 - offset_x) >= movement){
                offset_x += movement;
                if(offset_x > 1.0){
                    offset_x = offset_x - 1.0;
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
            offset_y = 1.0 + offset_y;
            pellet_eat(position_x, position_y + 1);
        }
        break;
    case RIGHT:
        if(offset_y == 0.0){
            if(!maze_walls[position_y][position_x + 1])
                offset_x += movement;
            else{
                offset_x = 0.0;
                image -> animate = false;
            }
        }
        else if(offset_y < 0.5){
            if(offset_y >= movement)
                offset_y -= movement;
            else{
                float square_offset = offset_y * offset_y;
                offset_x += sqrtf(movement * movement - square_offset);
                offset_y = 0.0;
                pellet_eat(position_x, position_y);
            }
        }
        else{
            if((1.0 - offset_y) >= movement){
                offset_y += movement;
                if(offset_y >= 1.0){
                    offset_y = offset_y - 1.0;
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
                offset_y += movement;
            else{
                offset_y = 0.0;
                image -> animate = false;
            }
        }
        else if(offset_x < 0.5){
            if(offset_x >= movement)
                offset_x -= movement;
            else{
                float square_offset = offset_x * offset_x;
                offset_y += sqrtf(movement * movement - square_offset);
                offset_x = 0.0;
                pellet_eat(position_x, position_y);
            }
        }
        else{
            if((1.0 - offset_x) >= movement){
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
    if(position_y == 13 && ((position_x == 14 && offset_x > 0.5) ||
                            (position_x == 14 && offset_x < 0.5)) &&
       fruits_are_visible())
        fruits_get_eaten();
    if(begin_slow_down == slow_down)
        slow_down = 1.0;

    if(isnan(offset_x) || isnan(offset_y)){
        printf("%f %f\n", prev_x, prev_y);
        pacman_print_position();
    }
}

void pacman_move(void){
    if(image -> animate){
        pacman_half_move();
        pacman_half_move();
    }
}

void pacman_slow_down(float slow){
    slow_down = slow;
}

void pacman_print_position(void){
    printf("%d %d %f %f\n", position_x, position_y, offset_x, offset_y);
}

void pacman_transform(void){
    perspective_transform(image, position_x, position_y, offset_x, offset_y, PACMAN_SIZE);
}
