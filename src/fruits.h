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

#ifndef _fruits_h_
#define _fruits_h_

#include "weaver/weaver.h"
#include "includes.h"

void fruits_init(void);
void fruits_end(void);
void fruits_appear(void);
void fruits_disappear(void);
void fruits_get_eaten(void);
bool fruits_are_visible(void);

#endif
