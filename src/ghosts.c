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
#define SCATTER          0
#define CHASE            1
#define FRIGHTNED        2

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
     {2, 25}, {7, 25}, {10, 25}, {13, 25}, {16, 25}, {19, 25}, {22, 25},
     {27, 25},
     {7, 29}, {MAZE_WIDTH - 8, 29}};
static float default_speed[21][4] = {// Normal, frightned, tunnel, fright time
    {0.75, 0.7, 0.4, 6.0}, {0.85, 0.55, 0.45, 5.0}, {0.85, 0.55, 0.45, 4.0},
    {0.85, 0.55, 0.45, 3.0}, {0.95, 0.6, 0.5, 2.0}, {0.95, 0.6, 0.5, 5.0},
    {0.95, 0.6, 0.5, 2.0}, {0.95, 0.6, 0.5, 2.0}, {0.95, 0.6, 0.5, 1.0},
    {0.95, 0.6, 0.5, 5.0}, {0.95, 0.6, 0.5, 2.0}, {0.95, 0.6, 0.5, 1.0},
    {0.95, 0.6, 0.5, 1.0}, {0.95, 0.6, 0.5, 3.0}, {0.95, 0.6, 0.5, 1.0},
    {0.95, 0.6, 0.5, 1.0}, {0.95, 0.6, 0.5, 0.0}, {0.95, 0.6, 0.5, 1.0},
    {0.95, 0.6, 0.5, 0.0}, {0.95, 0.6, 0.5, 0.0}, {0.95, 0.95, 0.5, 0.0}};
static float mode_duration[5][8] =
    {
     {7.0, 20.0, 7.0, 20.0, 5.0, 20.0, 5.0, INFINITY},
     {7.0, 20.0, 7.0, 20.0, 5.0, 1033.0, 0.017, INFINITY},
     {7.0, 20.0, 7.0, 20.0, 5.0, 1033.0, 0.017, INFINITY},
     {7.0, 20.0, 7.0, 20.0, 5.0, 1033.0, 0.017, INFINITY},
     {5.0, 20.0, 5.0, 20.0, 5.0, 1037.0, 0.017, INFINITY}
    };
static struct interface *blinky, *stopped_ghost = NULL;
static int mode, mode_count, last_mode = CHASE;
static float remaining_time_to_change_mode;
static int blinky_position_x, blinky_position_y;
static float blinky_offset_x, blinky_offset_y;
static int blinky_target_x, blinky_target_y;

static void reverse_direction(struct interface *ghost){
    switch(ghost -> integer){
    case LEFT:
        ghost -> integer = RIGHT;
        break;
    case RIGHT:
        ghost -> integer = LEFT;
        break;
    case UP:
        ghost -> integer = DOWN;
        break;
    case DOWN:
        ghost -> integer = UP;
        break;
    }
}

static void enter_mode(int new_mode){
    int level = W.game -> level - 1;
    if(level >= 21) level = 20;
    if(mode == CHASE || mode == SCATTER){
        reverse_direction(blinky);
    }
    last_mode = mode;
    mode = new_mode;
    switch(mode){
    case SCATTER:
        blinky_target_x = 27;
        blinky_target_y = MAZE_HEIGHT + 2;
        break;
    case FRIGHTNED:
        blinky -> b = 1.0;
        W.run_futurelly(ghosts_stop_frightned_mode, default_speed[level][3]);
        break;
    }
}

static void mode_change(void){
    int level = W.game -> level - 1;
    if(level >= 5) level = 4;
    mode_count ++;
    if(mode_count >= 8) mode_count = 7;
        if(mode == SCATTER)
        enter_mode(CHASE);
    else
        enter_mode(SCATTER);
    if(mode_count != 7){
            W.run_futurelly(mode_change, mode_duration[level][mode_count]);
    }
}

void ghosts_init(void){
    int level = W.game -> level - 1;
    if(level >= 5) level = 4;
    blinky = W.new_interface(7, 0, 0, GHOST_SIZE, GHOST_SIZE, "blinky.png");
    blinky -> integer = RIGHT;
    blinky -> b = 0.0;
    blinky_position_x = 14;
    blinky_position_y = 19;
    blinky_offset_x = 0.5;
    blinky_offset_y = 0.0;
    mode_count = 0;
    enter_mode(SCATTER);
    W.run_futurelly(mode_change, mode_duration[level][mode_count]);
}

void ghosts_transform(void){
    perspective_transform(blinky, blinky_position_x, blinky_position_y,
                          blinky_offset_x, blinky_offset_y, GHOST_SIZE);
}

static void ghosts_half_move(struct interface *ghost, int *position_x,
                             int *position_y, float *offset_x, float *offset_y,
                             float *movement){
    float distance_to_center[4] = {*offset_x, 1.0 - *offset_x, 1.0 - *offset_y,
                                   *offset_y};
    float *offset_to_change[4] = {offset_x, offset_x, offset_y, offset_y};
    int *position_to_change[4] = {position_x, position_x, position_y, position_y};
    switch(ghost -> integer){
    case LEFT:
    case DOWN:
        if(*offset_to_change[ghost -> integer] <= EPSILON){
            *offset_to_change[ghost -> integer] = 1.0 - *movement;
            (*position_to_change[ghost -> integer]) --;
            *movement = 0.0;
        }
        else if(*movement >= distance_to_center[ghost -> integer]){
            *movement -= *offset_to_change[ghost -> integer];
            *offset_to_change[ghost -> integer] = 0.0;
        }
        else{
            *offset_to_change[ghost -> integer] -= *movement;
            *movement = 0.0;
        }
        break;
    case RIGHT:
    case UP:
        if(*offset_to_change[ghost -> integer] <= EPSILON){
            *offset_to_change[ghost -> integer] += *movement;
            *movement = 0.0;
        }
        else if(*movement >= distance_to_center[ghost -> integer]){
            *movement -= (1.0 - *offset_to_change[ghost -> integer]);
            *offset_to_change[ghost -> integer] = 0.0;
            (*position_to_change[ghost -> integer]) ++;
        }
        else{
            *offset_to_change[ghost -> integer] += *movement;
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
    if(ghost -> integer == LEFT &&
       (*position_x < 0 || (*position_x == 0 && *offset_x == 0))){
        *position_x = MAZE_WIDTH - 1;
        *offset_x = 0;
    }
    else if(ghost -> integer == RIGHT && *position_x >= MAZE_WIDTH - 1){
        *position_x = 0;
        *offset_x = 0;
    }
}

static void choose_direction(struct interface *ghost, int position_x,
                             int position_y){
    float distance[4] = {5000.0, 5000.0, 5000.0, 5000.0};
    if(ghost == blinky){
        if(mode == CHASE){
            blinky_target_x = pacman_position_x;
            blinky_target_y = pacman_position_y;
        }
        if(blinky -> integer != DOWN && !maze_walls[position_y + 1][position_x])
            distance[UP] = hypot(blinky_target_x - position_x,
                                 blinky_target_y - (position_y + 1));
        if(blinky -> integer != UP && !maze_walls[position_y - 1][position_x])
            distance[DOWN] = hypot(blinky_target_x - position_x,
                                   blinky_target_y - (position_y - 1));
        if(blinky -> integer != LEFT && !maze_walls[position_y][position_x + 1])
            distance[RIGHT] = hypot(blinky_target_x - (position_x + 1),
                                    blinky_target_y - position_y);
        if(blinky -> integer != RIGHT &&
           !maze_walls[position_y][position_x - 1])
            distance[LEFT] = hypot(blinky_target_x - (position_x - 1),
                                   blinky_target_y - position_y);
    }
    if(position_y == 19 && ghost -> integer != DOWN)
        return;
    if(position_y == 7 && position_x > 12 && position_x < 18 &&
       ghost -> integer != DOWN)
        return;
    if(ghost -> b == 1.0){
        int direction = W.random() % 4;
        if(distance[direction] != 5000.0)
            ghost -> integer = direction;
        else if(distance[UP] != 5000.0)
            ghost -> integer = UP;
        else if(distance[LEFT] != 5000.0)
            ghost -> integer = LEFT;
        else if(distance[DOWN] != 5000.0)
            ghost -> integer = DOWN;
        else
            ghost -> integer = RIGHT;
        return;
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

static bool is_in_decision_point(int x, int y){
    int i;
    for(i = 0; i < DECISION_POINTS; i ++)
        if(decision_points[i][0] == x && decision_points[i][1] == y)
            return true;
    return false;
}

static void ghosts_full_move(struct interface *ghost, int *position_x,
                             int *position_y, float *offset_x, float *offset_y){
    float movement;
    int level = W.game -> level - 1;
    if(level > 20) level = 20;
    if(*position_y == 16 && (*position_x < 7 || *position_x > 22))
        movement = default_speed[level][2] * BASE_SPEED;
    else
        movement = default_speed[level][0] * BASE_SPEED;
    if(ghost == stopped_ghost)
        movement *= 0.25;
    // Try to kill pacman:
    if(*position_x == pacman_position_x && *position_y == pacman_position_y)
        pacman_killed_by(ghost);
    // Move to next tile
    if(*offset_x <= EPSILON && *offset_y <= EPSILON){
        if(is_in_decision_point(*position_x, *position_y))
            choose_direction(ghost, *position_x, *position_y);
        else
            turn_if_necessary(ghost, *position_x, *position_y);
    }
    ghosts_half_move(ghost, position_x, position_y,
                     offset_x, offset_y, &movement);
    // Check if in decision point
    if(*offset_x <= EPSILON && *offset_y <= EPSILON){
        if(is_in_decision_point(*position_x, *position_y)){
            choose_direction(ghost, *position_x, *position_y); //Decide direção
        }
        else
            turn_if_necessary(ghost, *position_x, *position_y);
    }
    // Try to kill pacman again:
    if(*position_x == pacman_position_x && *position_y == pacman_position_y)
        pacman_killed_by(ghost);
    // Resume movement:
    ghosts_half_move(ghost, position_x, position_y,
                     offset_x, offset_y, &movement);
}

void ghosts_move(void){
    ghosts_full_move(blinky, &blinky_position_x, &blinky_position_y,
                     &blinky_offset_x, &blinky_offset_y);
}

void ghost_carry_pacman(struct interface *ghost, int *position_x,
                        int *position_y, float *offset_x, float *offset_y){
    if(ghost == blinky){
        *position_x = blinky_position_x;
        *position_y = blinky_position_y;
        *offset_x = blinky_offset_x;
        *offset_y = blinky_offset_y;
    }
}

void ghosts_debug(void){
    printf("BLINKY: %d %d %f %f\n", blinky_position_x, blinky_position_y,
           blinky_offset_x, blinky_offset_y);
}

void ghost_slow_down(struct interface *ghost){
    stopped_ghost = ghost;
}

void ghosts_fright(void){
    remaining_time_to_change_mode = W.cancel(mode_change);
    enter_mode(FRIGHTNED);
}

void ghosts_stop_frightned_mode(void){
    blinky -> b = 0.0;
    W.run_futurelly(mode_change, remaining_time_to_change_mode);
    enter_mode(last_mode);
}
