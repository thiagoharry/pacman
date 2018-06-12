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

#include "score.h"

static bool initialized = false;
static int score, high_score;
static struct interface *digits[6];
static struct interface *one_hundred, *two_hundred, *three_hundred,
    *four_hundred, *five_hundred, *seven_hundred, *eight_hundred,
    *one_thousand, *combo_ghost_point, *two_thousand, *three_thousand,
    *five_thousand;
static struct interface *record;
static struct interface *record_digits[6];

static void hidden_score(void){
    one_hundred -> visible = false;
    two_hundred -> visible = false;
    three_hundred -> visible = false;
    four_hundred -> visible = false;
    five_hundred -> visible = false;
    seven_hundred -> visible = false;
    eight_hundred -> visible = false;
    one_thousand -> visible = false;
    combo_ghost_point -> visible = false;
    two_thousand -> visible = false;
    three_thousand -> visible = false;
    five_thousand -> visible = false;
}

void score_init(void){
    int i;
    int division = 100000;
    bool visible = false;
    if(!initialized){
        high_score = W.game -> stored_high_score;
        for(i = 0; i < 6; i ++){
            digits[i] = W.new_interface(9, i * 15 + 7, W.height - 7, 15, 15,
                                        "digits.png");
            digits[i] -> integer = 0;
            record_digits[i] = W.new_interface(9, i * 15 + W.width / 2 - 38, W.height - 22, 15,
                                               15, "digits.png");
            record_digits[i] -> integer = (high_score / division) % 10;
            if(record_digits[i] -> integer != 0 || i == 5)
                visible = true;
            record_digits[i] -> visible = visible;
            division /= 10;
        }
        one_hundred = W.new_interface(5, 0, 0, 30, 30, "one_hundred.png");
        two_hundred = W.new_interface(5, 0, 0, 30, 30, "two_hundred.png");
        three_hundred = W.new_interface(5, 0, 0, 30, 30, "three_hundred.png");
        four_hundred = W.new_interface(5, 0, 0, 30, 30, "four_hundred.png");
        five_hundred = W.new_interface(5, 0, 0, 30, 30, "five_hundred.png");
        seven_hundred = W.new_interface(5, 0, 0, 30, 30, "seven_hundred.png");
        eight_hundred = W.new_interface(5, 0, 0, 30, 30, "eight_hundred.png");
        one_thousand = W.new_interface(5, 0, 0, 30, 30, "one_thousand.png");
        combo_ghost_point = W.new_interface(5, 0, 0, 30, 30, "combo_ghost_point.png");
        two_thousand = W.new_interface(5, 0, 0, 30, 30, "two_thousand.png");
        three_thousand = W.new_interface(5, 0, 0, 30, 30, "three_thousand.png");
        five_thousand = W.new_interface(5, 0, 0, 30, 30, "five_thousand.png");
        record = W.new_interface(5, W.width / 2, W.height - 15, 91, 30, "record.png");
    }
    hidden_score();
    for(i = 0; i < 5; i ++)
        if(digits[i] -> integer == 0)
            digits[i] -> visible = false;
        else
            break;
    score = 0;
    initialized = true;
}

// (x-100)/12+380
void score_increment(int increment){
    if(score < 10000 && score + increment >= 10000)
        pacman_increment_life(1);
    int i, division = 100000;
    bool visible = false;
    score += increment;
    if(score > high_score){
        high_score = score;
        for(i = 0; i < 6; i ++){
            record_digits[i] -> integer = (score / division) % 10;
            if(record_digits[i] -> integer != 0 || i == 5)
                visible = true;
            record_digits[i] -> visible = visible;
            division /= 10;
        }
    }
    visible = false;
    division = 100000;
    for(i = 0; i < 6; i ++){
        digits[i] -> integer = (score / division) % 10;
        if(digits[i] -> integer != 0 || i == 5)
            visible = true;
        digits[i] -> visible = visible;
        division /= 10;
    }
    if(increment >= 100)
        W.cancel(hidden_score);
    switch(increment){
    case 100:
        W.move_interface(one_hundred, pacman -> x, pacman -> y);
        one_hundred -> visible = true;
        break;
    case 200:
        W.move_interface(two_hundred, pacman -> x, pacman -> y);
        two_hundred -> visible = true;
        break;
    case 300:
        W.move_interface(three_hundred, pacman -> x, pacman -> y);
        three_hundred -> visible = true;
        break;
    case 400:
        W.move_interface(four_hundred, pacman -> x, pacman -> y);
        four_hundred -> visible = true;
        break;
    case 500:
        W.move_interface(five_hundred, pacman -> x, pacman -> y);
        five_hundred -> visible = true;
        break;
    case 700:
        W.move_interface(seven_hundred, pacman -> x, pacman -> y);
        seven_hundred -> visible = true;
        break;
    case 800:
        W.move_interface(eight_hundred, pacman -> x, pacman -> y);
        eight_hundred -> visible = true;
        break;
    case 1000:
        W.move_interface(one_thousand, pacman -> x, pacman -> y);
        one_thousand -> visible = true;
        break;
    case 1600:
        W.move_interface(combo_ghost_point, pacman -> x, pacman -> y);
        combo_ghost_point -> visible = true;
        break;
    case 2000:
        W.move_interface(two_thousand, pacman -> x, pacman -> y);
        two_thousand -> visible = true;
        break;
    case 3000:
        W.move_interface(three_thousand, pacman -> x, pacman -> y);
        three_thousand -> visible = true;
        break;
    case 5000:
        W.move_interface(five_thousand, pacman -> x, pacman -> y);
        five_thousand -> visible = true;
        break;
    default:
        break;
    }
    if(increment >= 100)
        W.run_futurelly(hidden_score, 1.0);
}

void score_save(void){
    if(high_score > W.game -> stored_high_score){
        W.write_integer("high_score", high_score);
        W.game -> stored_high_score = high_score;
    }
}

void score_end(void){
    score_save();
    initialized = false;
}
