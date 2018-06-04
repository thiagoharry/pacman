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

#include "achievement.h"

static struct interface *book;
static struct interface *achievements[12];

static char list_of_achievements[12];

void achievement_init(void){
    int i;
    book = W.new_interface(10, W.width / 2, W.height / 2,
                           1000, 637, "achievement_book.png");
    book -> visible = false;
    achievements[0] = W.new_interface(11, W.width / 2 - 380,
                                      W.height / 2 + 220,
                                      100, 126, "achievement_cherry.png");
    achievements[1] = W.new_interface(11, W.width / 2 - 69,
                                      W.height / 2 + 120,
                                      100, 124, "achievement_strawberry.png");
    achievements[2] = W.new_interface(11, W.width / 2 - 380,
                                      W.height / 2 + 32,
                                      102, 102, "achievement_peach.png");
    achievements[3] = W.new_interface(11, W.width / 2 - 69,
                                      W.height / 2 - 64,
                                      119, 102, "achievement_apple.png");
    achievements[4] = W.new_interface(11, W.width / 2 - 381,
                                      W.height / 2 - 160,
                                      100, 119, "achievement_grape.png");
    achievements[5] = W.new_interface(11, W.width / 2 - 69,
                                      W.height / 2 - 248,
                                      100, 110, "achievement_galaxian.png");
    achievements[6] = W.new_interface(11, W.width / 2 + 75,
                                      W.height / 2 + 245,
                                      104, 104, "achievement_bell.png");
    achievements[7] = W.new_interface(11, W.width / 2 + 385,
                                      W.height / 2 + 145,
                                      104, 104, "achievement_key.png");
    achievements[8] = W.new_interface(11, W.width / 2 + 74,
                                      W.height / 2 + 60,
                                      100, 105, "achievement_ghost.png");
    achievements[9] = W.new_interface(11, W.width / 2 + 385,
                                      W.height / 2 - 42,
                                      102, 102, "achievement_picky.png");
    achievements[10] = W.new_interface(11, W.width / 2 + 74,
                                      W.height / 2 - 129,
                                      102, 102, "achievement_last_pellets.png");
    achievements[11] = W.new_interface(11, W.width / 2 + 385,
                                      W.height / 2 - 227,
                                      102, 102, "achievement_clock.png");
    for(i = 0; i < 12; i ++)
        achievements[i] -> visible = false;
    if(!W.read_string("achievements", list_of_achievements, 12))
        for(i = 0; i < 12; i ++)
            list_of_achievements[i] = 'n';
}

void achievement_show(void){
    int i;
    book -> visible = true;
    for(i = 0; i < 12; i ++)
        if(list_of_achievements[i] == 'y')
            achievements[i] -> visible = true;
}


void achievement_hide(void){
    int i;
    book -> visible = false;
    for(i = 0; i < 12; i ++)
        achievements[i] -> visible = false;
}

void achievement_save(void){
    W.write_string("achievements", list_of_achievements);
}

void achievement_new(int ach){
    if(ach >=0 && ach < 12){
        list_of_achievements[ach] = 'y';
    }
}

bool achievement_has(int ach){
    if(ach >=0 && ach < 12){
        return list_of_achievements[ach] == 'y';
    }
    return false;
}
