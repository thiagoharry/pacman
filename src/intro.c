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

#include "intro.h"

static struct interface *image, *sky, *play, *achievements, *credits;
#if W_TARGET == W_ELF
static struct interface *menu_exit;
#endif
static struct interface *cursor, *title, *credit_names;
static int option = 0;
static bool visible_achievement = false;
static bool visible_credits = false;

static int performance_measure_iterations;
static int performance;

static void measure_performance(void){
    if(performance_measure_iterations < 1000){
        performance += W.dt;
        performance_measure_iterations ++;
    }
}

static void check_performance(void){
    float result = (float) performance / (float) performance_measure_iterations;
    if(result >= 45000.0)
        W.game -> slow_computer  = true;
}

MAIN_LOOP intro(void){
 LOOP_INIT:
    image = W.new_interface(5, W.width / 2, 0.5 * W.height + 127,
                            720, 460, "intro.png");
    sky = W.new_interface(1, W.width / 2, 0.5 * W.height + 324,
                          702, W.height - 556, NULL);
    title = W.new_interface(5, W.width / 2, 0.5 * W.height + 278,
                            222, 36, "title.png");
    play = W.new_interface(5, W.width / 2, 0.3 * W.height,
                            71, 29, "menu_play.png");
    achievements = W.new_interface(5, W.width / 2, 0.3 * W.height - 40,
                           150, 32, "menu_achievements.png");
    credits = W.new_interface(5, W.width / 2, 0.3 * W.height - 80,
                                   102, 26, "menu_credits.png");
#if W_TARGET == W_ELF
    menu_exit = W.new_interface(5, W.width / 2, 0.3 * W.height - 120,
                           150, 26, "menu_sair.png");
#endif
    credit_names = W.new_interface(5, W.width / 2, -520,
                              744, 1053, "credits.png");
    credit_names -> visible = false;
    cursor = W.new_interface(W_INTERFACE_PERIMETER, W.width / 2, 0.3 * W.height,
                             152, 34, 1.0, 1.0, 1.0, 1.0);
    achievement_init();
    performance_measure_iterations = 0;
    performance = 0;
    W.game -> slow_computer = false;
LOOP_BODY:
    if(visible_achievement){
        if(W.keyboard[W_ENTER] == 1 ||
                (W.keyboard[W_ENTER] == 0 && W.keyboard[W_ANY] == 1)){
            achievement_hide();
            visible_achievement = false;
            sky -> visible = true;
            image -> visible = true;
            title -> visible = true;
        }
    }
    else if(visible_credits){
        if(W.keyboard[W_ENTER] == 1 ||
                (W.keyboard[W_ENTER] == 0 && W.keyboard[W_ANY] == 1) ||
            credit_names -> y >= W.height + 520){
            credit_names -> visible = false;
            visible_credits = false;
            sky -> visible = true;
            image -> visible = true;
            sky -> visible = true;
            play -> visible = true;
            achievements -> visible = true;
            credits -> visible = true;
            cursor -> visible = true;
            title -> visible = true;
            W.move_interface(credit_names, credit_names -> x, - 520);
        }
        else{
            W.move_interface(credit_names, credit_names -> x,
                             credit_names -> y + 5);
        }
    }
    else{
        if(W.keyboard[W_ENTER] == 1){
            if(option == 0){
                check_performance();
                Wloop(main_loop);
            }
            else if(option == 1){
                visible_achievement = true;
                sky -> visible = false;
                image -> visible = false;
                achievement_show();
            }
            else if(option == 2){
                visible_credits = true;
                credit_names -> visible = true;
                sky -> visible = false;
                image -> visible = false;
                sky -> visible = false;
                play -> visible = false;
                achievements -> visible = false;
                credits -> visible = false;
                cursor -> visible = false;
                title -> visible = false;
            }
            else if(option == 3){
                Wexit_loop();
            }
        }
        else if(W.keyboard[W_UP] == 1 && option > 0){
            option --;
            W.move_interface(cursor, cursor -> x, cursor -> y + 40);
        }
        else if(W.keyboard[W_DOWN] == 1 &&
#if W_TARGET == W_ELF
                option < 3){
#else
                option < 2){
#endif
            option ++;
            W.move_interface(cursor, cursor -> x, cursor -> y - 40);
        }
        measure_performance();
    }
LOOP_END:
  return;
}
