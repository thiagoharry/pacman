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
// Ghosts have 3 modes of behaviour mutually exclusive:
#define SCATTER          0
#define CHASE            1
#define FRIGHTNED        2

#define ghost_normal(G) (G -> r = 1.0, G -> b = 0.0, G -> g = 0.0)
#define ghost_dead(G)   (G -> r = 0.0, G -> b = 0.0, G -> g = 0.0)
#define ghost_blue(G)   (G -> r = 1.0, G -> b = 1.0, G -> g = 0.0)
#define ghost_blink(G)  (G -> r = 1.0, G -> b = 1.0, G -> g = 1.0)

#define is_alive(G)  (G -> r == 1.0)
#define is_scared(G) (G -> b == 1.0)

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
static float default_speed[21][6] = {// Normal, frightned, tunnel, fright time,
                                     // cruise elroy1, cruise elroy2
    {0.75, 0.7, 0.4, 6.0, 0.8, 0.85}, {0.85, 0.55, 0.45, 5.0, 0.9, 0.95},
    {0.85, 0.55, 0.45, 4.0, 0.9, 0.95}, {0.85, 0.55, 0.45, 3.0, 0.9, 0.95},
    {0.95, 0.6, 0.5, 2.0, 1.0, 1.05}, {0.95, 0.6, 0.5, 5.0, 1.0, 1.05},
    {0.95, 0.6, 0.5, 2.0, 1.0, 1.05}, {0.95, 0.6, 0.5, 2.0, 1.0, 1.05},
    {0.95, 0.6, 0.5, 1.0, 1.0, 1.05}, {0.95, 0.6, 0.5, 5.0, 1.0, 1.05},
    {0.95, 0.6, 0.5, 2.0, 1.0, 1.05}, {0.95, 0.6, 0.5, 1.0, 1.0, 1.05},
    {0.95, 0.6, 0.5, 1.0, 1.0, 1.05}, {0.95, 0.6, 0.5, 3.0, 1.0, 1.05},
    {0.95, 0.6, 0.5, 1.0, 1.0, 1.05}, {0.95, 0.6, 0.5, 1.0, 1.0, 1.05},
    {0.95, 0.6, 0.5, 0.0, 1.0, 1.05}, {0.95, 0.6, 0.5, 1.0, 1.0, 1.05},
    {0.95, 0.6, 0.5, 0.0, 1.0, 1.05}, {0.95, 0.6, 0.5, 0.0, 1.0, 1.05},
    {0.95, 0.95, 0.5, 0.0, 1.0, 1.05}};
static float mode_duration[5][8] =
    {
     {7.0, 20.0, 7.0, 20.0, 5.0, 20.0, 5.0, INFINITY},
     {7.0, 20.0, 7.0, 20.0, 5.0, 1033.0, 0.017, INFINITY},
     {7.0, 20.0, 7.0, 20.0, 5.0, 1033.0, 0.017, INFINITY},
     {7.0, 20.0, 7.0, 20.0, 5.0, 1033.0, 0.017, INFINITY},
     {5.0, 20.0, 5.0, 20.0, 5.0, 1037.0, 0.017, INFINITY}
    };
static int cruise_elroy_activation[21][2] = {//244
    {224, 234}, {214, 229}, {204, 224}, {204, 224}, {204, 224}, {194, 219},
    {194, 219}, {194, 219}, {184, 214}, {184, 214}, {184, 214}, {164, 204},
    {164, 204}, {164, 204}, {144, 194}, {144, 194}, {144, 194}, {144, 194},
    {124, 184}, {124, 184}, {124, 184}
};
static struct interface *stopped_ghost = NULL;
static int mode, mode_count, last_mode = CHASE;
static float remaining_time_to_change_mode;
static float blinky_offset_x, blinky_offset_y, pinky_offset_x, pinky_offset_y;
static int blinky_target_x, blinky_target_y, pinky_target_x, pinky_target_y;
static struct sound *eaten;
int blinky_position_x, blinky_position_y;
int pinky_position_x, pinky_position_y;
struct interface *blinky, *pinky;

static void ghosts_blink(void){
    if(is_scared(blinky))
        ghost_blink(blinky);
    if(is_scared(pinky))
        ghost_blink(pinky);
    W.run_futurelly(ghosts_stop_frightned_mode, 1.0);
}

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
        reverse_direction(pinky);
    }
    if(mode != FRIGHTNED)
        last_mode = mode;
    mode = new_mode;
    switch(mode){
    case SCATTER:
        if(W.game -> pellets_eaten < cruise_elroy_activation[level][0]){
            blinky_target_x = 27;
            blinky_target_y = MAZE_HEIGHT + 2;
        }
        pinky_target_x = 3;
        pinky_target_y = MAZE_HEIGHT + 2;
        break;
    case FRIGHTNED:
        W.cancel(ghosts_blink);
        W.cancel(ghosts_stop_frightned_mode);
        if(is_alive(blinky))
            ghost_blue(blinky);
        if(is_alive(pinky))
            ghost_blue(pinky);
        if(default_speed[level][3] >= 1.0)
            W.run_futurelly(ghosts_blink, default_speed[level][3] - 1.0);
        else{
            if(is_scared(blinky))
                ghost_blink(blinky);
            if(is_scared(pinky))
                ghost_blink(pinky);
            W.run_futurelly(ghosts_stop_frightned_mode, default_speed[level][3]);
        }
        break;
    }
}

static void mode_change(void){
    int level = W.game -> level - 1;
    if(level >= 5) level = 4;
    // Increment number of mode changes. Maximum: 7
    mode_count ++;
    if(mode == SCATTER)
        enter_mode(CHASE);
    else
        enter_mode(SCATTER);
    if(mode_count < 7){
            W.run_futurelly(mode_change, mode_duration[level][mode_count]);
    }
}

void ghosts_init(void){
    int level = W.game -> level - 1;
    if(level >= 5) level = 4;
    blinky = W.new_interface(7, 0, 0, GHOST_SIZE, GHOST_SIZE, "blinky.png");
    pinky = W.new_interface(7, 0, 0, GHOST_SIZE, GHOST_SIZE, "pinky.png");
    eaten = W.new_sound("bite2.wav");
    blinky -> integer = RIGHT;
    pinky -> integer = DOWN;
    ghost_normal(blinky);
    ghost_normal(pinky);
    blinky_position_x = 14;
    blinky_position_y = 19;
    pinky_position_x = 14;
    pinky_position_y = 16;
    pinky_offset_x = 0.5;
    pinky_offset_y = 0.0;
    blinky_offset_x = 0.5;
    blinky_offset_y = 0.0;
    mode_count = 0;
    enter_mode(SCATTER);
    W.run_futurelly(mode_change, mode_duration[level][mode_count]);
}

void ghosts_transform(void){
    perspective_transform(blinky, blinky_position_x, blinky_position_y,
                          blinky_offset_x, blinky_offset_y, GHOST_SIZE);
    perspective_transform(pinky, pinky_position_x, pinky_position_y,
                          pinky_offset_x, pinky_offset_y, GHOST_SIZE);
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
    int level = W.game -> level - 1;
    int target_x = 0, target_y = 0;
    if(level >= 21) level = 20;
    if(ghost == blinky){
        if(blinky -> r == 0.0){
            blinky_target_x = 14;
            blinky_target_y = 19;
        }
        else if(mode == CHASE ||
                W.game -> pellets_eaten >= cruise_elroy_activation[level][0]){
            blinky_target_x = pacman_position_x;
            blinky_target_y = pacman_position_y;
        }
        target_x = blinky_target_x;
        target_y = blinky_target_y;
    }
    else if(ghost == pinky){
        if(pinky -> r == 0.0){
            pinky_target_x = 14;
            pinky_target_y = 16;
        }
        else if(mode == CHASE){
            switch(pacman -> integer){
            case LEFT:
                pinky_target_x = pacman_position_x - 4;
                pinky_target_y = pacman_position_y;
                break;
            case RIGHT:
                pinky_target_x = pacman_position_x + 4;
                pinky_target_y = pacman_position_y;
                break;
            case UP_FROM_LEFT:
            case UP_FROM_RIGHT:
                pinky_target_x = pacman_position_x;
                pinky_target_y = pacman_position_y + 4;
                break;
            default:
                pinky_target_x = pacman_position_x;
                pinky_target_y = pacman_position_y - 4;
                break;
            }
        }
        target_x = pinky_target_x;
        target_y = pinky_target_y;
    }
    if(ghost -> integer != DOWN && !maze_walls[position_y + 1][position_x])
        distance[UP] = hypot(target_x - position_x,
                             target_y - (position_y + 1));
    if(ghost -> integer != UP && !maze_walls[position_y - 1][position_x])
        distance[DOWN] = hypot(target_x - position_x,
                               target_y - (position_y - 1));
    if(ghost -> integer != LEFT && !maze_walls[position_y][position_x + 1])
        distance[RIGHT] = hypot(target_x - (position_x + 1),
                                target_y - position_y);
    if(ghost -> integer != RIGHT &&
       !maze_walls[position_y][position_x - 1])
        distance[LEFT] = hypot(target_x - (position_x - 1),
                               target_y - position_y);
    if(position_y == 19 && ghost -> integer != DOWN)
        return;
    if(position_y == 7 && position_x > 12 && position_x < 18 &&
       ghost -> integer != DOWN)
        return;
    if(ghost -> b == 1.0 && ghost -> r == 1.0){
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
    // Set speed:
    if(*position_y == 16 && (*position_x < 7 || *position_x > 22))
        movement = default_speed[level][2] * BASE_SPEED; // Tunnel
    else if(ghost -> b == 1.0 || ghost -> r == 0.0)
        movement = default_speed[level][1] * BASE_SPEED; // Frightened
    else if(ghost == blinky &&
            W.game -> pellets_eaten >= cruise_elroy_activation[level][1])
        movement = default_speed[level][5] * BASE_SPEED; // Cruise elroy 2
    else if(ghost == blinky &&
            W.game -> pellets_eaten >= cruise_elroy_activation[level][0])
        movement = default_speed[level][4] * BASE_SPEED; // Cruise elroy 1
    else
        movement = default_speed[level][0] * BASE_SPEED; // Normal
    if(ghost == stopped_ghost)
        movement *= 0.25;
    // Try to kill pacman:
    if(*position_x == pacman_position_x && *position_y == pacman_position_y &&
        ghost -> r == 1.0)
        ghost_eat_or_get_eaten(ghost);
    // Special case 1: Need to enter in ghost pen
    if(!is_alive(ghost) && *position_y == 19 && *position_x == 14){
        if(*offset_x <= 0.5){
            *offset_x += movement;
            if(*offset_x >= 0.5){
                movement = *offset_x - 0.5;
                *offset_x = 0.5;
                ghost -> integer = DOWN;
            }
        }
        else{
            *offset_x -= movement;
            if(*offset_x <= 0.5){
                movement = 0.5 - *offset_x;
                *offset_x = 0.5;
                ghost -> integer = DOWN;
                ghost -> integer = DOWN;
            }
        }
    }
    // Move to next tile
    else if(*offset_x <= EPSILON && *offset_y <= EPSILON){
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
    if(*position_x == pacman_position_x && *position_y == pacman_position_y &&
        ghost -> r == 1.0){
        if(ghost -> b < 0.2)
            pacman_killed_by(ghost);
        else
            ghost_eat_or_get_eaten(ghost);
    }
    if(*position_x == 14 && *offset_x == 0.5 && *position_y == 19 &&
       ghost -> integer == UP && *offset_y == 0.0){ // After exit ghost room
            ghost -> integer = LEFT;
    }
    else if(*position_x == 14 && *position_y == 16 && *offset_y == 0.0 &&
            *offset_x == 0.5){
        if(ghost == pinky || ghost == blinky || is_alive(ghost)){
            // After entering the ghost pen
            ghost -> integer = UP;
            if(!is_alive(ghost))
                ghost_normal(ghost);
        }
    }
    // Resume movement:
    ghosts_half_move(ghost, position_x, position_y,
                     offset_x, offset_y, &movement);
}

void ghosts_move(void){
    ghosts_full_move(blinky, &blinky_position_x, &blinky_position_y,
                     &blinky_offset_x, &blinky_offset_y);
    ghosts_full_move(pinky, &pinky_position_x, &pinky_position_y,
                     &pinky_offset_x, &pinky_offset_y);
}

void ghost_carry_pacman(struct interface *ghost, int *position_x,
                        int *position_y, float *offset_x, float *offset_y){
    if(ghost == blinky){
        *position_x = blinky_position_x;
        *position_y = blinky_position_y;
        *offset_x = blinky_offset_x;
        *offset_y = blinky_offset_y;
    }
    else if(ghost == pinky){
        *position_x = pinky_position_x;
        *position_y = pinky_position_y;
        *offset_x = pinky_offset_x;
        *offset_y = pinky_offset_y;
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
    W.cancel(ghosts_stop_frightned_mode);
    W.cancel(ghosts_blink);
    if(mode != FRIGHTNED){
        remaining_time_to_change_mode = W.cancel(mode_change);
    }
    enter_mode(FRIGHTNED);
}

void ghosts_stop_frightned_mode(void){
    blinky -> b = 0.0;
    blinky -> g = 0.0;
    pinky -> b = 0.0;
    pinky -> g = 0.0;
    W.run_futurelly(mode_change, remaining_time_to_change_mode);
    enter_mode(last_mode);
}

void ghost_eat_or_get_eaten(struct interface *ghost){
    if(ghost -> r == 0.0)
        return;
    if(ghost -> b < 0.2)
        pacman_killed_by(ghost);
    else{
        ghost_dead(ghost);
        W.play_sound(eaten);
    }
}
