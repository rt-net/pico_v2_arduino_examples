// Copyright 2023 RT Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "fast.h"

FAST g_fast;

void FAST::run(short gx, short gy)
{
  t_global_direction glob_nextdir;
  int straight_count = 0;

  switch (g_map.nextDir2Get(gx, gy, &glob_nextdir)) {
    case right:
      g_run.rotate(right, 1);  //右に曲がって
      break;
    case left:
      g_run.rotate(left, 1);  //左に曲がって
      break;
    case rear:
      g_run.rotate(right, 2);  //180度に旋回して
      break;
    default:
      break;
  }

  g_run.accelerate(HALF_SECTION, g_run.search_speed);
  straight_count = 0;
  g_map.mypos.dir = glob_nextdir;
  g_map.axisUpdate();

  while ((g_map.mypos.x != gx) || (g_map.mypos.y != gy)) {
    switch (g_map.nextDir2Get(gx, gy, &glob_nextdir)) {
      case front:
        straight_count++;
        break;
      case right:
        if (straight_count > 0) {
          g_run.straight(
            straight_count * SECTION, g_run.search_speed, g_run.max_speed, g_run.search_speed);
          straight_count = 0;
        }
        g_run.decelerate(HALF_SECTION, g_run.search_speed);
        g_run.rotate(right, 1);
        g_run.accelerate(HALF_SECTION, g_run.search_speed);
        break;
      case left:
        if (straight_count > 0) {
          g_run.straight(
            straight_count * SECTION, g_run.search_speed, g_run.max_speed, g_run.search_speed);
          straight_count = 0;
        }
        g_run.decelerate(HALF_SECTION, g_run.search_speed);
        g_run.rotate(left, 1);
        g_run.accelerate(HALF_SECTION, g_run.search_speed);
        break;
      default:
        break;
    }
    g_map.mypos.dir = glob_nextdir;
    g_map.axisUpdate();
  }
  if (straight_count > 0) {
    g_run.straight(
      straight_count * SECTION, g_run.search_speed, g_run.max_speed, g_run.search_speed);
  }
  g_run.decelerate(HALF_SECTION, g_run.search_speed);
}
