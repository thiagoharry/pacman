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

#include "fruits.h"

static struct interface *cherry;
// Credit: Felikin100 (https://pixabay.com/en/users/Felikin100-3374205), CC0
static struct interface *strawberry;
// Credit: https://pixabay.com/en/users/Clker-Free-Vector-Images-3736/, CC0
static struct interface *peach;
// Credit: https://pixabay.com/en/users/OpenClipart-Vectors-30363/, CC0
static struct interface *apple;
// Credit: https://commons.wikimedia.org/wiki/File:Grape_icon.png, OTRS
static struct interface *grape;
static struct interface *galaxian;
// https://commons.wikimedia.org/wiki/File:Nuvola_apps_bell.svg
static struct interface *instruments;
// Credit: https://pixabay.com/en/key-metal-plain-307753/
static struct interface *key;

// https://pixabay.com/en/book-open-book-an-empty-book-notes-1129923/

static bool initialized = false;
static struct sound *eat;

#define FRUIT_SIZE 33

void fruits_init(void){
    int x_pos = (maze_space[13][14].x + maze_space[13][15].x) / 2;
    int y_pos = maze_space[13][14].y;
    W.game -> fruits_eaten_this_stage = 0;
    if(initialized){
        fruits_disappear();
        W.cancel(fruits_disappear);
        return;
    }
    cherry = W.new_interface(5, x_pos, y_pos, FRUIT_SIZE, FRUIT_SIZE,
                             "cherry.png");
    strawberry = W.new_interface(5, x_pos, y_pos, FRUIT_SIZE, FRUIT_SIZE,
                                 "strawberry.png");
    peach = W.new_interface(5, x_pos, y_pos, FRUIT_SIZE, FRUIT_SIZE,
                            "peach.png");
    apple = W.new_interface(5, x_pos, y_pos, FRUIT_SIZE, FRUIT_SIZE,
                            "apple.png");
    grape = W.new_interface(5, x_pos, y_pos, FRUIT_SIZE, FRUIT_SIZE,
                            "grape.png");
    galaxian = W.new_interface(5, x_pos, y_pos, FRUIT_SIZE, FRUIT_SIZE,
                            "galaxian.png");
    instruments = W.new_interface(5, x_pos, y_pos, FRUIT_SIZE, FRUIT_SIZE,
                                  "instruments.png");
    key = W.new_interface(5, x_pos, y_pos, FRUIT_SIZE, FRUIT_SIZE,
                                  "key.png");
    fruits_disappear();
    // Sound under Creative Commons by Jason Shaw at audionautix.com
    eat = W.new_sound("bite1.wav");
    initialized = true;
}

void fruits_appear(void){
    switch(W.game -> level){
    case 1:
        cherry -> visible = true;
        break;
    case 2:
        strawberry -> visible = true;
        break;
    case 3:
    case 4:
        peach -> visible = true;
        break;
    case 5:
    case 6:
        apple -> visible = true;
        break;
    case 7:
    case 8:
        grape -> visible = true;
        break;
    case 9:
    case 10:
        galaxian -> visible = true;
        break;
    case 11:
    case 12:
        instruments -> visible = true;
        break;
    default:
        key -> visible = true;
        break;
    }
    W.run_futurelly(fruits_disappear, 9.5);
}

void fruits_disappear(void){
    cherry -> visible = false;
    strawberry -> visible = false;
    peach -> visible = false;
    apple -> visible = false;
    grape -> visible = false;
    galaxian -> visible = false;
    instruments -> visible = false;
    key -> visible = false;
}

bool fruits_are_visible(void){
    switch(W.game -> level){
    case 1:
        return cherry -> visible;
    case 2:
        return strawberry -> visible;
    case 3:
    case 4:
        return peach -> visible;
    case 5:
    case 6:
        return apple -> visible;
    case 7:
    case 8:
        return grape -> visible;
    case 9:
    case 10:
        return galaxian -> visible;
    case 11:
    case 12:
        return instruments -> visible;
    default:
        return key -> visible;
    }
}

void fruits_get_eaten(void){
    fruits_disappear();
    W.play_sound(eat);
    W.game -> fruits_eaten_this_stage ++;
    switch(W.game -> level){
    case 1:
        score_increment(100);
        if(!achievement_has(ACHIEVEMENT_CHERRY)){
            achievement_new(ACHIEVEMENT_CHERRY);
            pacman_show_achievement();
        }
        break;
    case 2:
        score_increment(300);
        if(!achievement_has(ACHIEVEMENT_STRAWBERRY)){
            achievement_new(ACHIEVEMENT_STRAWBERRY);
            pacman_show_achievement();
        }
        break;
    case 3:
    case 4:
        score_increment(500);
        if(!achievement_has(ACHIEVEMENT_PEACH)){
            achievement_new(ACHIEVEMENT_PEACH);
            pacman_show_achievement();
        }
        break;
    case 5:
    case 6:
        score_increment(700);
        if(!achievement_has(ACHIEVEMENT_APPLE)){
            achievement_new(ACHIEVEMENT_APPLE);
            pacman_show_achievement();
        }
        break;
    case 7:
    case 8:
        score_increment(1000);
        if(!achievement_has(ACHIEVEMENT_GRAPE)){
            achievement_new(ACHIEVEMENT_GRAPE);
            pacman_show_achievement();
        }
        break;
    case 9:
    case 10:
        score_increment(2000);
        if(!achievement_has(ACHIEVEMENT_SHIP)){
            achievement_new(ACHIEVEMENT_SHIP);
            pacman_show_achievement();
        }
        break;
    case 11:
    case 12:
        score_increment(3000);
        if(!achievement_has(ACHIEVEMENT_BELL)){
            achievement_new(ACHIEVEMENT_BELL);
            pacman_show_achievement();
        }
        break;
    default:
        score_increment(5000);
        if(!achievement_has(ACHIEVEMENT_KEY)){
            achievement_new(ACHIEVEMENT_KEY);
            pacman_show_achievement();
        }
        break;
    }

}
