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
#define BASE_SPEED    0.4

#define is_dead(P) (P -> integer == DEAD)

struct interface *pacman;
static struct interface *killer = NULL;
struct sound *kill_sound;
static float offset_x, offset_y;
static float slow_down;
static float default_speed[21][2] = {
    {0.8, 0.9}, {0.9, 0.95}, {0.9, 0.95}, {0.9, 0.95}, {1.0, 1.0}, {1.0, 1.0},
    {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0},
    {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 1.0},
    {1.0, 1.0}, {1.0, 1.0}, {0.9, 0.9}};

int pacman_position_x, pacman_position_y;

void pacman_init(void){
    pacman_position_x = 14;
    pacman_position_y = 7;
    offset_x = 0.5;
    offset_y = 0.0;
    pacman = W.new_interface(6, 100, 100, PACMAN_SIZE,
                            PACMAN_SIZE, "pacman.gif");
    pacman -> animate = false;
    pacman -> current_frame = 2;
    pacman -> integer = RIGHT;
    slow_down = 1.0;
    kill_sound = W.new_sound("ghost.wav");
}

void pacman_turn_right(void){
    if(is_dead(pacman))
        return;
    if((offset_y < 0.5 &&
        !maze_walls[pacman_position_y][pacman_position_x + 1]) ||
       (offset_y > 0.5 && !maze_walls[pacman_position_y + 1][pacman_position_x + 1])){
        pacman -> integer = RIGHT;
        pacman -> animate = true;
    }
}

void pacman_turn_left(void){
    if(is_dead(pacman))
        return;
    if((offset_y < 0.5 &&
        !maze_walls[pacman_position_y][pacman_position_x - 1]) ||
       (offset_y > 0.5 && !maze_walls[pacman_position_y + 1][pacman_position_x - 1])){
        pacman -> integer = LEFT;
        pacman -> animate = true;
    }
}

void pacman_turn_up(void){
    if(is_dead(pacman))
        return;
    if((offset_x < 0.5 &&
        !maze_walls[pacman_position_y + 1][pacman_position_x]) ||
       (offset_x > 0.5 &&
        !maze_walls[pacman_position_y + 1][pacman_position_x + 1])){
        if(pacman -> integer == UP_FROM_LEFT || pacman -> integer == UP_FROM_RIGHT)
            return;
        else if(pacman -> integer == LEFT || pacman -> integer == DOWN_FROM_RIGHT)
            pacman -> integer = UP_FROM_LEFT;
        else
            pacman -> integer = UP_FROM_RIGHT;
        pacman -> animate = true;
    }
}

void pacman_turn_down(void){
    if(is_dead(pacman))
        return;
    if(pacman_position_x > 12 && pacman_position_x < 16 &&
       pacman_position_y == 19)
        return; // Não entre na área dos fantasmas
    if((offset_x < 0.5 &&
        !maze_walls[pacman_position_y - 1][pacman_position_x]) ||
       (offset_x > 0.5 &&
        !maze_walls[pacman_position_y - 1][pacman_position_x + 1])){
        if(pacman -> integer == DOWN_FROM_LEFT ||
           pacman -> integer == DOWN_FROM_RIGHT)
            return;
        else if(pacman -> integer == LEFT || pacman -> integer == UP_FROM_RIGHT)
            pacman -> integer = DOWN_FROM_LEFT;
        else
            pacman -> integer = DOWN_FROM_RIGHT;
        pacman -> animate = true;
    }
}

static void pacman_half_move(void){
    float begin_slow_down = slow_down;
    float movement;
    int level = W.game -> level - 1;
    float prev_x = offset_x, prev_y = offset_y;
    if(level > 20) level = 20;
    if(slow_down < default_speed[level][0])
        movement = slow_down * BASE_SPEED * 0.5;
    else
        movement = default_speed[level][0] * BASE_SPEED * 0.5;
    switch(pacman -> integer){
    case LEFT:
        if(offset_y == 0.0){
            if(offset_x == 0.0 &&
                maze_walls[pacman_position_y][pacman_position_x - 1])
                pacman -> animate = false;
            else if(maze_walls[pacman_position_y][pacman_position_x]){
                offset_x = 0.0;
                pacman_position_x ++;
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
                pacman_position_y ++;
                pellet_eat(pacman_position_x, pacman_position_y);
            }
        }
        if(offset_x < 0.0){
            pacman_position_x --;
            offset_x = 1.0 + offset_x;
            pellet_eat(pacman_position_x + 1, pacman_position_y);
        }
        break;
    case DOWN_FROM_LEFT:
    case DOWN_FROM_RIGHT:
        if(offset_x == 0.0){
            if(offset_y == 0.0 &&
               maze_walls[pacman_position_y - 1][pacman_position_x])
                pacman -> animate = false;
            else if(maze_walls[pacman_position_y][pacman_position_x]){
                offset_y = 0.0;
                pacman_position_y ++;
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
                    pacman_position_x ++;
                    pellet_eat(pacman_position_x, pacman_position_y);
                }
            }
            else{
                float square_offset = (1.0 - offset_x) * (1.0 - offset_x);
                offset_y -= sqrtf(movement * movement - square_offset);
                offset_x = 0.0;
                pacman_position_x ++;
                pellet_eat(pacman_position_x, pacman_position_y);
                pacman_position_y --;
            }
        }
        if(offset_y < 0.0){
            pacman_position_y --;
            offset_y = 1.0 + offset_y;
            pellet_eat(pacman_position_x, pacman_position_y + 1);
        }
        break;
    case RIGHT:
        if(offset_y == 0.0){
            if(!maze_walls[pacman_position_y][pacman_position_x + 1])
                offset_x += movement;
            else{
                offset_x = 0.0;
                pacman -> animate = false;
            }
        }
        else if(offset_y < 0.5){
            if(offset_y >= movement)
                offset_y -= movement;
            else{
                float square_offset = offset_y * offset_y;
                offset_x += sqrtf(movement * movement - square_offset);
                offset_y = 0.0;
                pellet_eat(pacman_position_x, pacman_position_y);
            }
        }
        else{
            if((1.0 - offset_y) >= movement){
                offset_y += movement;
                if(offset_y >= 1.0){
                    offset_y = offset_y - 1.0;
                    pacman_position_y ++;
                    pellet_eat(pacman_position_x, pacman_position_y);
                }
            }
            else{
                float square_offset = (1.0 - offset_y) * (1.0 - offset_y);
                offset_x += sqrtf(movement * movement - square_offset);
                offset_y = 0.0;
                pacman_position_y ++;
                pellet_eat(pacman_position_x, pacman_position_y);
            }
        }
        if(offset_x >= 1.0){
            pacman_position_x ++;
            offset_x -= 1.0;
            pellet_eat(pacman_position_x, pacman_position_y);
        }
        break;
    case UP_FROM_LEFT:
    case UP_FROM_RIGHT:
        if(offset_x == 0.0){
            if(!maze_walls[pacman_position_y + 1][pacman_position_x])
                offset_y += movement;
            else{
                offset_y = 0.0;
                pacman -> animate = false;
            }
        }
        else if(offset_x < 0.5){
            if(offset_x >= movement)
                offset_x -= movement;
            else{
                float square_offset = offset_x * offset_x;
                offset_y += sqrtf(movement * movement - square_offset);
                offset_x = 0.0;
                pellet_eat(pacman_position_x, pacman_position_y);
            }
        }
        else{
            if((1.0 - offset_x) >= movement){
                offset_x += movement;
                if(offset_x > 1.0){
                    offset_x = 1.0 - offset_x;
                    pacman_position_x ++;
                    pellet_eat(pacman_position_x, pacman_position_y);
                }
            }
            else{
                float square_offset = (1.0 - offset_x) * (1.0 - offset_x);
                offset_y += sqrtf(movement * movement - square_offset);
                offset_x = 0.0;
                pacman_position_x ++;
                pellet_eat(pacman_position_x, pacman_position_y);
            }
        }
        if(offset_y >= 1.0){
            pacman_position_y ++;
            offset_y -= 1.0;
            pellet_eat(pacman_position_x, pacman_position_y);
        }
        break;
    default:
        return;
    }
    if(pacman -> integer == LEFT &&
       (pacman_position_x < 0 || (pacman_position_x == 0 && offset_x == 0))){
        pacman_position_x = MAZE_WIDTH - 1;
        offset_x = 0;
    }
    else if(pacman -> integer == RIGHT && pacman_position_x >= MAZE_WIDTH - 1){
        pacman_position_x = 0;
        offset_x = 0;
    }
    if(pacman_position_y == 13 && ((pacman_position_x == 14 && offset_x > 0.5) ||
                            (pacman_position_x == 14 && offset_x < 0.5)) &&
       fruits_are_visible())
        fruits_get_eaten();
    if(begin_slow_down == slow_down)
        slow_down = 1.0;

    if(isnan(offset_x) || isnan(offset_y)){
        printf("%f %f\n", prev_x, prev_y);
        pacman_print_position();
    }
    if(pacman -> integer != DEAD){
        if(pacman_position_x == blinky_position_x &&
           pacman_position_y == blinky_position_y)
            ghost_eat_or_get_eaten(blinky);
        else if(pacman_position_x == pinky_position_x &&
                pacman_position_y == pinky_position_y)
            ghost_eat_or_get_eaten(pinky);
        else if(pacman_position_x == inky_position_x &&
                pacman_position_y == inky_position_y)
            ghost_eat_or_get_eaten(inky);
        else if(pacman_position_x == clyde_position_x &&
                pacman_position_y == clyde_position_y)
            ghost_eat_or_get_eaten(clyde);
    }
}

void pacman_move(void){
    if(pacman -> integer == DEAD){
        ghost_carry_pacman(killer, &pacman_position_x,
                           &pacman_position_y, &offset_x, &offset_y);
    }
    else if(pacman -> animate){
        pacman_half_move();
        pacman_half_move();
    }
}

void pacman_slow_down(float slow){
    slow_down = slow;
}

void pacman_print_position(void){
    printf("%d %d %f %f\n", pacman_position_x, pacman_position_y, offset_x, offset_y);
}

void pacman_transform(void){
    perspective_transform(pacman, pacman_position_x, pacman_position_y, offset_x, offset_y, PACMAN_SIZE);
}

void pacman_killed_by(struct interface *ghost){
    if(pacman -> integer != DEAD){
        pacman -> integer = DEAD;
        killer = ghost;
        pacman -> animate = false;
        W.stop_music("music1.mp3");
        W.play_sound(kill_sound);
        ghost_slow_down(ghost);
    }
}
