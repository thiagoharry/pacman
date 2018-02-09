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

#include "maze.h"

static struct interface *maze_image;

void maze_init(void){
  int i, j;
  int y_pos[MAZE_HEIGHT] = {63, 90, 115, 140, 165, 188, 210, 233, 254, 275, 296,
                            316, 336, 355, 375, 393, 411, 429, 446, 463, 480,
                            497, 514, 530, 546, 562, 578, 594, 609, 624, 639};
  for(i = 0; i < MAZE_HEIGHT; i ++)
    for(j = 0; j < MAZE_WIDTH; j ++){
      float proportion = ((740.0 - y_pos[i]) / 1480.0) + 0.5;
      maze_space[i][j].y = y_pos[i];
      maze_space[i][j].size_multiplier = 1.0 - 0.00073529 * maze_space[i][j].y;
      maze_space[i][j].x = W.width / 2 - 445 * proportion -
        i * 1.4 +
        (29.3 * j) * proportion +
        j * 1.17 / (proportion * proportion);
    }
  // Size should be multiple of 640x740
  maze_image = W.new_interface(4, W.width / 2, 550, 1024, 1184,
                               "maze.png");

  // X 28.7 X X 27.2 X X 25.96 X X 24.9 X X X X X X X X X X 21.76 X X 21.17 X X X
  // 20.5
  /*for(i = 0; i < 30; i ++){
    maze_space[0][i].x = maze_space[0][i].y = maze_space[0][i].size_multiplier = -1.0;
  }
  for(i = 0; i < 30; i ++){
    maze_space[i][1].y = 90;
    maze_space[i][1].size_multiplier = 0.8906;
    maze_space[i][1].x = W.width / 2 - 417.4 + 28.7 * i;
  }
  for(i = 0; i < 30; i ++){
    maze_space[i][2].y = 115;
    maze_space[i][2].size_multiplier = 0.875;
    maze_space[i][2].x = W.width / 2 - 410 + 28.0 * i;
  }
  for(i = 0; i < 30; i ++){
    maze_space[i][3].y = 140;
    maze_space[i][3].size_multiplier = 0.84375;
    maze_space[i][3].x = W.width / 2 - 403.52 + 27.76 * i;
  }
  for(i = 0; i < 30; i ++){
    maze_space[i][4].y = 165;
    maze_space[i][4].size_multiplier = 0.828125;
    maze_space[i][4].x = W.width / 2 - 395.4 + 27.2 * i;
  }
  for(i = 0; i < 30; i ++){
    maze_space[i][5].y = 188;
    maze_space[i][5].size_multiplier = 0.8125;
    maze_space[i][5].x = W.width / 2 - 389.0476 + 26.7619 * i;
    }*/
}
