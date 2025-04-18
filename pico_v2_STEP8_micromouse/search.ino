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

#include "search.h"

SEARCH g_search;

void SEARCH::lefthand(void)
{
  g_run.accelerate(HALF_SECTION, g_run.search_speed);

  while (1) {
    if (g_sensor.sen_l.is_wall == false) {
      g_run.decelerate(HALF_SECTION, g_run.search_speed);
      g_run.rotate(left, 1);
      g_run.accelerate(HALF_SECTION, g_run.search_speed);
    } else if ((g_sensor.sen_fl.is_wall == false) && (g_sensor.sen_fr.is_wall == false)) {
      g_run.straight(SECTION, g_run.search_speed, g_run.search_speed, g_run.search_speed);
    } else if (g_sensor.sen_r.is_wall == false) {
      g_run.decelerate(HALF_SECTION, g_run.search_speed);
      g_run.rotate(right, 1);
      g_run.accelerate(HALF_SECTION, g_run.search_speed);
    } else {
      g_run.decelerate(HALF_SECTION, g_run.search_speed);
      g_run.rotate(right, 2);
      g_run.accelerate(HALF_SECTION, g_run.search_speed);
    }
  }
}

void SEARCH::adachi(char gx, char gy)
{
  t_global_direction glob_nextdir;
  t_local_direction temp_next_dir;

  switch (g_map.nextDirGet(gx, gy, &glob_nextdir)) {
    case front:
      break;
    case right:
      g_run.rotate(right, 1);
      break;
    case left:
      g_run.rotate(left, 1);
      break;
    case rear:
      g_run.rotate(right, 2);
      break;
  }

  g_run.accelerate(HALF_SECTION, g_run.search_speed);

  g_map.mypos.dir = glob_nextdir;
  g_map.axisUpdate();

  while ((g_map.mypos.x != gx) || (g_map.mypos.y != gy)) {
    g_map.wallSet(g_sensor.sen_fr.is_wall, g_sensor.sen_r.is_wall, g_sensor.sen_l.is_wall);

    switch (g_map.nextDirGet(gx, gy, &glob_nextdir)) {
      case front:
        g_run.oneStep(SECTION, g_run.search_speed);
        break;
      case right:
        g_run.decelerate(HALF_SECTION, g_run.search_speed);
        g_run.rotate(right, 1);
        g_run.accelerate(HALF_SECTION, g_run.search_speed);
        break;
      case left:
        g_run.decelerate(HALF_SECTION, g_run.search_speed);
        g_run.rotate(left, 1);
        g_run.accelerate(HALF_SECTION, g_run.search_speed);
        break;
      case rear:
        g_run.decelerate(HALF_SECTION, g_run.search_speed);
        g_run.rotate(right, 2);
        g_run.accelerate(HALF_SECTION, g_run.search_speed);
        break;
    }

    g_map.mypos.dir = glob_nextdir;  //方向を更新
    g_map.axisUpdate();
  }

  g_map.wallSet(g_sensor.sen_fr.is_wall, g_sensor.sen_r.is_wall, g_sensor.sen_l.is_wall);
  g_run.decelerate(HALF_SECTION, g_run.search_speed);
}