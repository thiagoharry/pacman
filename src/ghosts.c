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
#include "ghosts.h"

#define GHOST_SIZE    46.0
#define BASE_SPEED     0.4
#define LEFT             0
#define RIGHT            1
#define UP               2
#define DOWN             3
#define EPSILON        0.0
#define DECISION_POINTS 34


static int decision_points[DECISION_POINTS][2] =
    {
     {13, 1}, {MAZE_WIDTH - 14, 1},
     {4, 4}, {MAZE_WIDTH - 5, 4},
     {7, 7},  {10, 7}, {13, 7},
     {MAZE_WIDTH - 14, 7}, {MAZE_WIDTH - 11, 7}, {MAZE_WIDTH - 8, 7},
     {7, 10}, {10, 10}, {MAZE_WIDTH - 11, 10}, {MAZE_WIDTH - 8, 10},
     {10, 13}, {MAZE_WIDTH - 11, 13},
     {7, 16}, {10, 16}, {MAZE_WIDTH - 11, 16}, {MAZE_WIDTH - 8, 16},
     {13, 19}, {MAZE_WIDTH - 14, 19},
     {7, 22}, {MAZE_WIDTH - 8, 22},
     {2, 25}, {7, 25}, {10, 25}, {13, 25}, {16, 25}, {19, 25}, {22, 25}, {27, 25},
     {7, 29}, {MAZE_WIDTH - 8, 29}};
static float default_speed[21][3] = {// Normal, frightned, tunnel
    {0.75, 0.7, 0.4}, {0.85, 0.55, 0.45}, {0.85, 0.55, 0.45},
    {0.85, 0.55, 0.45}, {0.95, 0.6, 0.5}, {0.95, 0.6, 0.5},
    {0.95, 0.6, 0.5}, {0.95, 0.6, 0.5}, {0.95, 0.6, 0.5},
    {0.95, 0.6, 0.5}, {0.95, 0.6, 0.5}, {0.95, 0.6, 0.5},
    {0.95, 0.6, 0.5}, {0.95, 0.6, 0.5}, {0.95, 0.6, 0.5},
    {0.95, 0.6, 0.5}, {0.95, 0.6, 0.5}, {0.95, 0.6, 0.5},
    {0.95, 0.6, 0.5}, {0.95, 0.6, 0.5}, {0.95, 0.95, 0.5}};
static struct interface *blinky;
int blinky_position_x, blinky_position_y;
float blinky_offset_x, blinky_offset_y;

void ghosts_init(void){
    blinky = W.new_interface(7, 0, 0, GHOST_SIZE, GHOST_SIZE, "blinky.png");
    blinky -> integer = LEFT;
    blinky_position_x = 14;
    blinky_position_y = 19;
    blinky_offset_x = 0.5;
    blinky_offset_y = 0.0;
}

void ghosts_transform(void){
    perspective_transform(blinky, blinky_position_x, blinky_position_y,
                          blinky_offset_x, blinky_offset_y, GHOST_SIZE);
}

static void ghosts_half_move(struct interface *ghost, int *position_x,
                             int *position_y, float *offset_x, float *offset_y,
                             float *movement){
    switch(ghost -> integer){
    case LEFT:
        if(*offset_x <= EPSILON){
            *offset_x = 1.0 - *movement;
            (*position_x) --;
            *movement = 0.0;
        }
        else if(*movement >= *offset_x){
            *movement -= *offset_x;
            *offset_x = 0.0;
        }
        else{
            *offset_x -= *movement;
            *movement = 0.0;
        }
        break;
    case DOWN:
        if(*offset_y <= EPSILON){
            *offset_y = 1.0 - *movement;
            (*position_y) --;
            *movement = 0.0;
        }
        else if(*movement >= *offset_y){
            *movement -= *offset_y;
            *offset_y = 0.0;
        }
        else{
            *offset_y -= *movement;
            *movement = 0.0;
        }
        break;
    case RIGHT:
        if(*offset_x <= EPSILON){
            *offset_x += *movement;
            *movement = 0.0;
        }
        else if(*movement >= (1.0 - *offset_x)){
            *movement -= (1.0 - *offset_x);
            *offset_x = 0.0;
            (*position_x) ++;
        }
        else{
            *offset_x += *movement;
            *movement = 0.0;
        }
        break;
    case UP:
        if(*offset_y <= EPSILON){
            *offset_y += *movement;
            *movement = 0.0;
        }
        else if(*movement >= (1.0 - *offset_y)){
            *movement -= (1.0 - *offset_y);
            *offset_y = 0.0;
            (*position_y) ++;
        }
        else{
            *offset_y += *movement;
            *movement = 0.0;
        }
        break;
    }
    if(*offset_x >= 1.0){
        *offset_x -= 1.0;
        (*position_x) ++;
    }
    else if(*offset_x < 0.0){
        *offset_x += 1.0;
        (*position_x) --;
    }
    if(*offset_y >= 1.0){
        *offset_y -= 1.0;
        (*position_y) ++;
    }
    else if(*offset_y < 0.0){
        *offset_y += 1.0;
        (*position_y) --;
    }
}



static void choose_direction(struct interface *ghost, int position_x,
                             int position_y){
    float distance[4] = {5000.0, 5000.0, 5000.0, 5000.0};
    if(ghost == blinky){
        if(blinky -> integer != DOWN && !maze_walls[position_y + 1][position_x])
            distance[UP] = hypot(pacman_position_x - position_x,
                                 pacman_position_y - (position_y + 1));
        if(blinky -> integer != UP && !maze_walls[position_y - 1][position_x])
            distance[DOWN] = hypot(pacman_position_x - position_x,
                                   pacman_position_y - (position_y - 1));
        if(blinky -> integer != LEFT && !maze_walls[position_y][position_x + 1])
            distance[RIGHT] = hypot(pacman_position_x - (position_x + 1),
                                    pacman_position_y - position_y);
        if(blinky -> integer != RIGHT &&
           !maze_walls[position_y][position_x - 1])
            distance[LEFT] = hypot(pacman_position_x - (position_x - 1),
                                   pacman_position_y - position_y);
    }
    if(distance[UP] <= distance[DOWN] && distance[UP] <= distance[LEFT] &&
       distance[UP] <= distance[RIGHT])
        ghost -> integer = UP;
    else if(distance[LEFT] <= distance[DOWN] &&
            distance[LEFT] <= distance[RIGHT])
        ghost -> integer = LEFT;
    else if(distance[DOWN] <= distance[RIGHT])
        ghost -> integer = DOWN;
    else
        ghost -> integer = RIGHT;
}

static void turn_if_necessary(struct interface *ghost, int position_x,
                              int position_y){
    switch(ghost -> integer){
    case LEFT:
        if(maze_walls[position_y][position_x - 1]){
            if(maze_walls[position_y + 1][position_x])
                ghost -> integer = DOWN;
            else
                ghost -> integer = UP;
        }
        break;
    case RIGHT:
        if(maze_walls[position_y][position_x + 1]){
            if(maze_walls[position_y + 1][position_x])
                ghost -> integer = DOWN;
            else
                ghost -> integer = UP;
        }
        break;
    case UP:
        if(maze_walls[position_y + 1][position_x]){
            if(maze_walls[position_y][position_x + 1])
                ghost -> integer = LEFT;
            else
                ghost -> integer = RIGHT;
        }
        break;
    case DOWN:
        if(maze_walls[position_y - 1][position_x]){
            if(maze_walls[position_y][position_x + 1])
                ghost -> integer = LEFT;
            else
                ghost -> integer = RIGHT;
        }
        break;
    }
}

static void ghosts_full_move(struct interface *ghost, int *position_x,
                             int *position_y, float *offset_x, float *offset_y){
    float movement;
    bool is_in_decision_point = false;
    int level = W.game -> level - 1;
    int i;
    if(level > 20) level = 20;
    movement = default_speed[level][0] * BASE_SPEED;
    // Move to next tile
    if(*offset_x <= EPSILON && *offset_y <= EPSILON){
        // Check if in decision point
        for(i = 0; i < DECISION_POINTS; i ++){
            if(decision_points[i][0] == *position_x &&
               decision_points[i][1] == *position_y)
                is_in_decision_point = true;
        }
        if(is_in_decision_point)
            choose_direction(ghost, *position_x, *position_y); //Decide direção
        else
            turn_if_necessary(ghost, *position_x, *position_y);
    }
    is_in_decision_point = false;
    ghosts_half_move(ghost, position_x, position_y,
                     offset_x, offset_y, &movement);
    // Check if in decision point
    if(*offset_x <= EPSILON && *offset_y <= EPSILON){
        for(i = 0; i < DECISION_POINTS; i ++){
            if(decision_points[i][0] == *position_x &&
               decision_points[i][1] == *position_y){
                is_in_decision_point = true;
            }
        }
        if(is_in_decision_point){

            choose_direction(ghost, *position_x, *position_y); //Decide direção
        }
        else
            turn_if_necessary(ghost, *position_x, *position_y);
    }
    // Resume movement:
    ghosts_half_move(ghost, position_x, position_y,
                     offset_x, offset_y, &movement);
}

void ghosts_move(void){
    ghosts_full_move(blinky, &blinky_position_x, &blinky_position_y,
                     &blinky_offset_x, &blinky_offset_y);
}

void ghosts_debug(void){
    printf("BLINKY: %d %d %f %f\n", blinky_position_x, blinky_position_y,
           blinky_offset_x, blinky_offset_y);
}
