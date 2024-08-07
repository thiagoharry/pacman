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
#include <stdbool.h>
#include "pellet.h"

static const float normal_pellet_size = 64.0;
static bool initialized = false;
static struct interface *pellets[MAZE_HEIGHT][MAZE_WIDTH];
static struct sound *wakka, *super_wakka;
static int pellet_distribution[MAZE_HEIGHT][MAZE_WIDTH] =
  {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
   {0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0},
   {0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0},
   {0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0},
   {0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0},
   {0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0},
   {0,0,2,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,2,0,0},
   {0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0},
   {0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0},
   {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
   {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
   {0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0},
   {0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0},
   {0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0},
   {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
   {0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0},
   {0,0,2,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,2,0,0},
   {0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0},
   {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
static float slow_down[21][2] = {
    {0.71, 0.79}, {0.79, 0.83}, {0.79, 0.83}, {0.79, 0.83}, {0.87, 0.87},
    {0.87, 0.87}, {0.87, 0.87}, {0.87, 0.87}, {0.87, 0.87}, {0.87, 0.87},
    {0.87, 0.87}, {0.87, 0.87}, {0.87, 0.87}, {0.87, 0.87}, {0.87, 0.87},
    {0.87, 0.87}, {0.87, 0.87}, {0.87, 0.87}, {0.87, 0.87}, {0.87, 0.87},
    {0.79, 0.79}};

static int power_pellets_eaten;
static bool activate_pellet_achievement;

long when_was_last_pellet_eaten;


void pellet_init(void){
  int i, j;
  when_was_last_pellet_eaten = W.t;
  W.game -> pellets_eaten = 0;
  power_pellets_eaten = 0;
  activate_pellet_achievement = false;
  if(initialized){
      for(i = 0; i < MAZE_HEIGHT; i ++)
          for(j = 0; j < MAZE_WIDTH; j ++)
              if(pellet_distribution[i][j])
                  pellets[i][j] -> visible = true;
      return;
  }
  for(i = 0; i < MAZE_HEIGHT; i ++)
    for(j = 0; j < MAZE_WIDTH; j ++)
      if(pellet_distribution[i][j]){
        float size = normal_pellet_size * maze_space[i][j].size_multiplier;
        if(pellet_distribution[i][j] == 1)
          pellets[i][j] = W.new_interface(5, maze_space[i][j].x,
                                          maze_space[i][j].y, size, size,
                                          "pellet.png");
        else
          pellets[i][j] = W.new_interface(5, maze_space[i][j].x,
                                          maze_space[i][j].y, size, size,
                                          "power_pellet.png");
      }
  wakka = W.new_sound("wakka.wav");
  super_wakka = W.new_sound("wakka2.wav");
  initialized = true;
}

int pellet_eat(int x, int y){
    int value = pellet_distribution[y][x];
    if(x >= 0 && y >=0 && x < MAZE_WIDTH && y < MAZE_HEIGHT &&
       pellets[y][x] != NULL && pellets[y][x] -> visible){
        int level = W.game -> level - 1;
        if(level >= 21)
            level = 20;
        pellets[y][x] -> visible = false;
        W.game -> pellets_eaten ++;
        when_was_last_pellet_eaten = W.t;
        if(value == 2){
            if(!fast)
                pacman_slow_down(1.0 - 3.0 *(1.0 - slow_down[level][0]));
            else
                pacman_slow_down(1.0 - 3.0 *(1.0 - slow_down[level][1]));
            W.play_sound(super_wakka);
            score_increment(50);
            ghosts_fright();
            power_pellets_eaten ++;
        }
        else{
            W.play_sound(wakka);
            if(!fast)
                pacman_slow_down(slow_down[level][0]);
            else
                pacman_slow_down(slow_down[level][1]);
            score_increment(10);
            if(W.game -> pellets_eaten == 240 && power_pellets_eaten == 0)
                activate_pellet_achievement = true;
        }
        if(W.game -> pellets_eaten == 70 || W.game -> pellets_eaten == 170)
            fruits_appear();
        else if(W.game -> pellets_eaten >= 244){
            if(activate_pellet_achievement &&
               !achievement_has(ACHIEVEMENT_PELLET)){
                achievement_new(ACHIEVEMENT_PELLET);
                pacman_show_achievement();
            }
            level_up();
        }
    }
    return value;
}

void pellet_end(void){
    initialized = false;
}
