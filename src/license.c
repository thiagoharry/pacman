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

#include "license.h"

static struct interface *image;
static bool end = false;

void end_license(void){
    end = true;
}

MAIN_LOOP license(void){
 LOOP_INIT:
    image  = W.new_interface(W_INTERFACE_IMAGE, W.width / 2, W.height / 2,
                             488, 191, "license.png");
    W.run_futurelly(end_license, 4.0);
 LOOP_BODY:
    if(end)
        Wloop(intro);
 LOOP_END:
  return;
}
