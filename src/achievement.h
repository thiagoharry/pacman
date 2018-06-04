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

#ifndef _achievement_h_
#define _achievement_h_

#include "weaver/weaver.h"
#include "includes.h"

#define ACHIEVEMENT_CHERRY     0
#define ACHIEVEMENT_STRAWBERRY 1
#define ACHIEVEMENT_PEACH      2
#define ACHIEVEMENT_APPLE      3
#define ACHIEVEMENT_GRAPE      4
#define ACHIEVEMENT_SHIP       5
#define ACHIEVEMENT_BELL       6
#define ACHIEVEMENT_KEY        7
#define ACHIEVEMENT_GHOST      8
#define ACHIEVEMENT_PICKY      9
#define ACHIEVEMENT_PELLET    10
#define ACHIEVEMENT_TIME      11

void achievement_init(void);
void achievement_show(void);
void achievement_hide(void);
void achievement_save(void);
void achievement_new(int);
bool achievement_has(int);

#endif
