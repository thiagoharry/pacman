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
static struct interface *cursor, *title;
static int option = 0;
static bool visible_achievement = false;

MAIN_LOOP intro(void){
 LOOP_INIT:
    image = W.new_interface(5, W.width / 2, 0.5 * W.height + 127,
                            720, 460, "intro.png");
    sky = W.new_interface(1, W.width / 2, 0.5 * W.height + 324,
                          702, W.height - 648, NULL);
    title = W.new_interface(5, W.width / 2, 0.5 * W.height + 278,
                            222, 36, "title.png");
    play = W.new_interface(5, W.width / 2, 0.3 * W.height,
                            71, 29, "menu_play.png");
    achievements = W.new_interface(5, W.width / 2, 0.3 * W.height - 40,
                           150, 32, "menu_achievements.png");
    credits = W.new_interface(5, W.width / 2, 0.3 * W.height - 80,
                                   102, 26, "menu_credits.png");
    cursor = W.new_interface(W_INTERFACE_PERIMETER, W.width / 2, 0.3 * W.height,
                             152, 34, 1.0, 1.0, 1.0, 1.0);
    achievement_init();
LOOP_BODY:
    if(visible_achievement){
        if(W.keyboard[W_ESC])
            Wexit_loop();
        else if(W.keyboard[W_ENTER] == 1 ||
                (W.keyboard[W_ENTER] == 0 && W.keyboard[W_ANY] == 1)){
            achievement_hide();
            visible_achievement = false;
            sky -> visible = true;
            image -> visible = true;
        }
    }
    else{
        if(W.keyboard[W_ESC])
            Wexit_loop();
        else if(W.keyboard[W_ENTER] == 1){
            if(option == 0)
                Wloop(main_loop);
            else if(option == 1){
                visible_achievement = true;
                sky -> visible = false;
                image -> visible = false;
                achievement_show();
            }
        }
        else if(W.keyboard[W_UP] == 1 && option > 0){
            option --;
            W.move_interface(cursor, cursor -> x, cursor -> y + 40);
        }
        else if(W.keyboard[W_DOWN] == 1 && option < 2 ){
            option ++;
            W.move_interface(cursor, cursor -> x, cursor -> y - 40);
        }
    }
LOOP_END:
  return;
}
