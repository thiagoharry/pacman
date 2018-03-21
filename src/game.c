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

static struct interface *sky, *grass, *horizon;

MAIN_LOOP main_loop(void){ // The game loop
 LOOP_INIT: // Code executed during loop initialization
  sky = W.new_interface(1, W.width / 2, 0.5 * W.height + 324,
                        W.width, W.height - 648, NULL);
  grass = W.new_interface(2, W.width / 2, 324,
                          W.width, 648,
                          "grass.png");
  horizon = W.new_interface(3, W.width / 2, 653,
  W.width, 10, "horizon.png");
  maze_init();
  pellet_init();
  pacman_init();
 LOOP_BODY: // Code executed every loop iteration
    if(W.keyboard[W_ANY])
        Wexit_loop();
 LOOP_END: // Code executed at the end of the loop
    return;
}

int main(void){
  Winit(); // Initializes Weaver
  resolution_init();
  Wloop(main_loop); // Enter a new game loop
  return 0;
}
