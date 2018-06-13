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

#include "game.h"

static struct interface *sky, *grass, *horizon, *ready, *digit;
static struct sound *start_sound;
static bool game_started = false;

void clock_achievement(void){
    if(!achievement_has(ACHIEVEMENT_TIME)){
        achievement_new(ACHIEVEMENT_TIME);
        pacman_show_achievement();
    }
}

void begin_game(void){
    digit -> integer --;
    digit -> visible = true;
    if(digit -> integer <= 0){
        game_started = true;
        ready -> visible = false;
        digit -> visible = false;
        if(W.game -> level > 1)
            W.play_music("music2.mp3", true);
        ghosts_init();
        W.run_futurelly(clock_achievement, 180.0);
    }
    else
        W.run_futurelly(begin_game, 1.0);
}

MAIN_LOOP main_loop(void){ // The game loop
LOOP_INIT: // Code executed during loop initialization
    sky = W.new_interface(1, W.width / 2, 0.5 * W.height + 324,
                          W.width, W.height - 648, NULL);
    grass = W.new_interface(2, W.width / 2, 324,
                            W.width, 648,
                            "grass.png");
    horizon = W.new_interface(3, W.width / 2, 653,
                              W.width, 10, "horizon.png");
    ready = W.new_interface(5, W.width / 2, W.height / 2 - 20,
                            91, 30, "ready.png");
    digit = W.new_interface(9, W.width / 2, W.height / 2 - 27,
                            15, 15, "digits.png");
    if(W.game -> slow_computer)
        sky -> visible = false;
    W.game -> level = 1;
    W.game -> game_over = false;
    digit -> visible = false;
    digit -> integer = 4;
    start_sound = W.new_sound("start.wav");
    maze_init();
    pellet_init();
    pacman_init();
    fruits_init();
    ghosts_init();
    score_init();
    W.play_music("music2.mp3", true);
    game_started = false;
    W.run_futurelly(begin_game, 1.0);
LOOP_BODY: // Code executed every loop iteration
    if(game_started){
        if(W.keyboard[W_UP])
            pacman_turn_up();
        else if(W.keyboard[W_DOWN])
            pacman_turn_down();
        else if(W.keyboard[W_LEFT])
            pacman_turn_left();
        else if(W.keyboard[W_RIGHT])
            pacman_turn_right();
        ghosts_move();
        pacman_move();
    }

    pacman_transform();
    ghosts_transform();
    if(W.game -> game_over)
        Wloop(intro);
LOOP_END: // Code executed at the end of the loop
    W.stop_music("music2.mp3");
    return;
}

void level_up(void){
    W.cancel(clock_achievement);
    W.game -> level ++;
    if(W.game -> ghosts_eaten_this_stage == 0 &&
       W.game -> fruits_eaten_this_stage == 2){
        if(!achievement_has(ACHIEVEMENT_PICKY)){
            achievement_new(ACHIEVEMENT_PICKY);
            pacman_show_achievement();
        }
    }
    pellet_init();
    pacman_init();
    fruits_init();
    ghosts_init();
    achievement_save();
    ghosts_use_global_pellet_counter = false;
    W.pause_music("music2.mp3");
    game_started = false;
    ready -> visible = true;
    digit -> visible = false;
    digit -> integer = 4;
    W.run_futurelly(begin_game, 1.0);
    W.play_sound(start_sound);
}

void lose_life(void){
    int new_life;
    W.cancel(clock_achievement);
    pacman_init();
    ghosts_init();
    achievement_save();
    new_life = pacman_increment_life(-1);
    ghosts_use_global_pellet_counter = true;
    if(new_life == 0){
        game_end();
    }
    else{
        W.play_music("music2.mp3", true);
        W.run_futurelly(clock_achievement, 180.0);
    }
}

int main(void){
  Winit(); // Initializes Weaver
  resolution_init();
  W.hide_cursor();
  game_init();
  Wloop(license);
  //copyleft();
  return 0;
}

void game_init(void){
    if(!W.read_integer("high_score", &(W.game -> stored_high_score))){
        W.game -> stored_high_score = 0;
    }
}

void game_end(void){
    W.game -> game_over = true;
    W.stop_music("music2.mp3");
    pellet_end();
    pacman_end();
    fruits_end();
    ghosts_end();
    score_end();
}
