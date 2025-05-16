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

#include "map_manager.h"

MapManager g_map;

//コンストラクタ
MapManager::MapManager()
{
  for (int i = 0; i < MAZESIZE_X; i++) {
    for (int j = 0; j < MAZESIZE_Y; j++) {
      wall[i][j].north = wall[i][j].east = wall[i][j].south = wall[i][j].west =
        _UNKNOWN;  //迷路情報を未知で初期化する
    }
  }

  for (int i = 0; i < MAZESIZE_X; i++) {
    wall[i][0].south = WALL;               //四方の壁を追加する(南)
    wall[i][MAZESIZE_Y - 1].north = WALL;  //四方の壁を追加する(北)
  }

  for (int i = 0; i < MAZESIZE_Y; i++) {
    wall[0][i].west = WALL;               //四方の壁を追加する(西)
    wall[MAZESIZE_X - 1][i].east = WALL;  //四方の壁を追加する(東)
  }
  //   x,y
  wall[0][0].east = wall[1][0].west = WALL;  //スタート地点の右の壁を追加する
  wall[0][0].north = wall[0][1].south = NOWALL;

  mypos.x = 0;
  mypos.y = 0;
  mypos.dir = north;

  goal_mx = 0x07;
  goal_my = 0x07;
}

void MapManager::positionInit(void)
{
  mypos.x = mypos.y = 0;
  mypos.dir = north;
}

char MapManager::wallDataGet(unsigned char x, unsigned char y, t_global_direction l_global_dir)
{
  switch (l_global_dir) {
    case north:
      return wall[x][y].north;
      break;
    case west:
      return wall[x][y].west;
      break;
    case south:
      return wall[x][y].south;
      break;
    case east:
      return wall[x][y].east;
      break;
  }
  return 99;
}

void MapManager::wallDataSet(
  unsigned char x, unsigned char y, t_global_direction l_global_dir, char data)
{
  switch (l_global_dir) {
    case north:
      wall[x][y].north = data;
      break;
    case west:
      wall[x][y].west = data;
      break;
    case south:
      wall[x][y].south = data;
      break;
    case east:
      wall[x][y].east = data;
      break;
  }
}

void MapManager::axisUpdate(void)
{
  switch (mypos.dir) {
    case north:
      mypos.y++;
      break;
    case east:
      mypos.x++;
      break;
    case south:
      mypos.y--;
      break;
    case west:
      mypos.x--;
      break;
  }
}

void MapManager::nextDir(t_local_direction l_local_dir)
{
  if (l_local_dir == right) {
    switch (mypos.dir) {
      case north:
        mypos.dir = east;
        break;
      case east:
        mypos.dir = south;
        break;
      case south:
        mypos.dir = west;
        break;
      case west:
        mypos.dir = north;
        break;
    }
  } else if (l_local_dir == left) {
    switch (mypos.dir) {
      case north:
        mypos.dir = west;
        break;
      case east:
        mypos.dir = north;
        break;
      case south:
        mypos.dir = east;
        break;
      case west:
        mypos.dir = south;
        break;
    }
  }
}

void MapManager::wallSet(bool IS_SEN_FR, bool IS_SEN_R, bool IS_SEN_L)  //壁情報を記録
{
  switch (mypos.dir) {
    case north:
      wall[mypos.x][mypos.y].north = IS_SEN_FR ? WALL : NOWALL;
      wall[mypos.x][mypos.y].east = IS_SEN_R ? WALL : NOWALL;
      wall[mypos.x][mypos.y].west = IS_SEN_L ? WALL : NOWALL;
      if (mypos.y < (MAZESIZE_X - 1)) wall[mypos.x][mypos.y + 1].south = IS_SEN_FR ? WALL : NOWALL;
      if (mypos.x < (MAZESIZE_Y - 1)) wall[mypos.x + 1][mypos.y].west = IS_SEN_R ? WALL : NOWALL;
      if (mypos.x > 0) wall[mypos.x - 1][mypos.y].east = IS_SEN_L ? WALL : NOWALL;
      break;
    case east:
      wall[mypos.x][mypos.y].east = IS_SEN_FR ? WALL : NOWALL;
      wall[mypos.x][mypos.y].south = IS_SEN_R ? WALL : NOWALL;
      wall[mypos.x][mypos.y].north = IS_SEN_L ? WALL : NOWALL;
      if (mypos.x < (MAZESIZE_X - 1)) wall[mypos.x + 1][mypos.y].west = IS_SEN_FR ? WALL : NOWALL;
      if (mypos.y > 0) wall[mypos.x][mypos.y - 1].north = IS_SEN_R ? WALL : NOWALL;
      if (mypos.y < (MAZESIZE_Y - 1)) wall[mypos.x][mypos.y + 1].south = IS_SEN_L ? WALL : NOWALL;
      break;
    case south:
      wall[mypos.x][mypos.y].south = IS_SEN_FR ? WALL : NOWALL;
      wall[mypos.x][mypos.y].west = IS_SEN_R ? WALL : NOWALL;
      wall[mypos.x][mypos.y].east = IS_SEN_L ? WALL : NOWALL;
      if (mypos.y > 0) wall[mypos.x][mypos.y - 1].north = IS_SEN_FR ? WALL : NOWALL;
      if (mypos.x > 0) wall[mypos.x - 1][mypos.y].east = IS_SEN_R ? WALL : NOWALL;
      if (mypos.x < (MAZESIZE_X - 1)) wall[mypos.x + 1][mypos.y].west = IS_SEN_L ? WALL : NOWALL;
      break;
    case west:
      wall[mypos.x][mypos.y].west = IS_SEN_FR ? WALL : NOWALL;
      wall[mypos.x][mypos.y].north = IS_SEN_R ? WALL : NOWALL;
      wall[mypos.x][mypos.y].south = IS_SEN_L ? WALL : NOWALL;
      if (mypos.x > 0) wall[mypos.x - 1][mypos.y].east = IS_SEN_FR ? WALL : NOWALL;
      if (mypos.y < (MAZESIZE_Y - 1)) wall[mypos.x][mypos.y + 1].south = IS_SEN_R ? WALL : NOWALL;
      if (mypos.y > 0) wall[mypos.x][mypos.y - 1].north = IS_SEN_L ? WALL : NOWALL;
      break;
  }
}

void MapManager::stepMapSet(
  unsigned char posX, unsigned char posY, t_global_direction l_global_dir, int * little,
  t_global_direction * now_dir, int * priority)
{
  int tmp_priority;
  tmp_priority = priorityGet(posX, posY, l_global_dir);
  if (steps_map[posX][posY] < *little) {
    *little = steps_map[posX][posY];
    *now_dir = l_global_dir;
    *priority = tmp_priority;
  } else if (steps_map[posX][posY] == *little) {
    if (*priority < tmp_priority) {
      *now_dir = l_global_dir;
      *priority = tmp_priority;
    }
  }
}

t_local_direction MapManager::nextGdir(t_global_direction * p_global_dir)
{
  switch (*p_global_dir) {
    case north:
      switch (mypos.dir) {
        case north:
          return front;
          break;
        case east:
          return left;
          break;
        case south:
          return rear;
          break;
        case west:
          return right;
          break;
        default:
          return loca_dir_error;
          break;
      }
      break;
    case east:
      switch (mypos.dir) {
        case east:
          return front;
          break;
        case south:
          return left;
          break;
        case west:
          return rear;
          break;
        case north:
          return right;
          break;
        default:
          return loca_dir_error;
          break;
      }
      break;
    case south:
      switch (mypos.dir) {
        case south:
          return front;
          break;
        case west:
          return left;
          break;
        case north:
          return rear;
          break;
        case east:
          return right;
          break;
        default:
          return loca_dir_error;
          break;
      }
      break;
    case west:
      switch (mypos.dir) {
        case west:
          return front;
          break;
        case north:
          return left;
          break;
        case east:
          return rear;
          break;
        case south:
          return right;
          break;
        default:
          return loca_dir_error;
          break;
      }
      break;
    default:
      return loca_dir_error;
      break;
  }
}

t_local_direction MapManager::nextDirGet(char x, char y, t_global_direction * p_global_dir)
{
  int little, priority;

  searchMapMake(x, y);
  little = 65535;
  priority = 0;

  if ((wall[mypos.x][mypos.y].north != WALL) && (mypos.y < (MAZESIZE_Y - 1))) {
    stepMapSet(mypos.x, mypos.y + 1, north, &little, p_global_dir, &priority);
  }

  if ((wall[mypos.x][mypos.y].east != WALL) && (mypos.x < (MAZESIZE_X - 1))) {
    stepMapSet(mypos.x + 1, mypos.y, east, &little, p_global_dir, &priority);
  }

  if ((wall[mypos.x][mypos.y].south != WALL) && (mypos.y > 0)) {
    stepMapSet(mypos.x, mypos.y - 1, south, &little, p_global_dir, &priority);
  }

  if ((wall[mypos.x][mypos.y].west != WALL) && (mypos.x > 0)) {
    stepMapSet(mypos.x - 1, mypos.y, west, &little, p_global_dir, &priority);
  }

  if (steps_map[mypos.x][mypos.y] == 65535) {
    while (1) {
      ledSet(0x0a);
      delay(500);
      ledSet(0x05);
      delay(500);
    }
  } else {
    return nextGdir(p_global_dir);
  }

  return front;
}

t_local_direction MapManager::nextDir2Get(short x, short y, t_global_direction * p_global_dir)
{
  int little, priority;

  map2Make(x, y);
  little = 65535;

  priority = 0;

  if ((wall[mypos.x][mypos.y].north == NOWALL) && ((mypos.y + 1) < MAZESIZE_Y)) {
    stepMapSet(mypos.x, mypos.y + 1, north, &little, p_global_dir, &priority);
  }

  if ((wall[mypos.x][mypos.y].east == NOWALL) && ((mypos.x + 1) < MAZESIZE_X)) {
    stepMapSet(mypos.x + 1, mypos.y, east, &little, p_global_dir, &priority);
  }

  if ((wall[mypos.x][mypos.y].south == NOWALL) && (mypos.y > 0)) {
    stepMapSet(mypos.x, mypos.y - 1, south, &little, p_global_dir, &priority);
  }

  if ((wall[mypos.x][mypos.y].west == NOWALL) && (mypos.x > 0)) {
    stepMapSet(mypos.x - 1, mypos.y, west, &little, p_global_dir, &priority);
  }

  if (steps_map[mypos.x][mypos.y] == 65535) {  //Goalにいけない
    while (1) {
      ledSet(0x0a);
      delay(500);
      ledSet(0x05);
      delay(500);
    }
  } else {
    return nextGdir(p_global_dir);
  }

  return front;
}

void MapManager::searchMapMake(int x, int y)
{
  bool change_flag;

  for (int i = 0; i < MAZESIZE_X; i++) {
    for (int j = 0; j < MAZESIZE_Y; j++) {
      steps_map[i][j] = 65535;
    }
  }
  steps_map[x][y] = 0;

  do {
    change_flag = false;
    for (int i = 0; i < MAZESIZE_X; i++) {
      for (int j = 0; j < MAZESIZE_Y; j++) {
        if (steps_map[i][j] == 65535) continue;
        if (
          (j < (MAZESIZE_Y - 1)) && (wall[i][j].north != WALL) && (steps_map[i][j + 1] == 65535)) {
          steps_map[i][j + 1] = steps_map[i][j] + 1;
          change_flag = true;
        }

        if ((i < (MAZESIZE_X - 1)) && (wall[i][j].east != WALL) && (steps_map[i + 1][j] == 65535)) {
          steps_map[i + 1][j] = steps_map[i][j] + 1;
          change_flag = true;
        }

        if ((j > 0) && (wall[i][j].south != WALL) && (steps_map[i][j - 1] == 65535)) {
          steps_map[i][j - 1] = steps_map[i][j] + 1;
          change_flag = true;
        }

        if ((i > 0) && (wall[i][j].west != WALL) && (steps_map[i - 1][j] == 65535)) {
          steps_map[i - 1][j] = steps_map[i][j] + 1;
          change_flag = true;
        }
      }
    }
  } while (change_flag == true);
}

void MapManager::map2Make(int x, int y)
{
  bool change_flag;

  for (int i = 0; i < MAZESIZE_X; i++) {
    for (int j = 0; j < MAZESIZE_Y; j++) {
      steps_map[i][j] = 65535;
    }
  }
  steps_map[x][y] = 0;

  do {
    change_flag = false;
    for (int i = 0; i < MAZESIZE_X; i++) {
      for (int j = 0; j < MAZESIZE_Y; j++) {
        if (steps_map[i][j] == 65535) continue;
        if (
          (j < (MAZESIZE_Y - 1)) && (wall[i][j].north == NOWALL) &&
          (steps_map[i][j + 1] == 65535)) {
          steps_map[i][j + 1] = steps_map[i][j] + 1;
          change_flag = true;
        }

        if (
          (i < (MAZESIZE_X - 1)) && (wall[i][j].east == NOWALL) && (steps_map[i + 1][j] == 65535)) {
          steps_map[i + 1][j] = steps_map[i][j] + 1;
          change_flag = true;
        }

        if ((j > 0) && (wall[i][j].south == NOWALL) && (steps_map[i][j - 1] == 65535)) {
          steps_map[i][j - 1] = steps_map[i][j] + 1;
          change_flag = true;
        }

        if ((i > 0) && (wall[i][j].west == NOWALL) && (steps_map[i - 1][j] == 65535)) {
          steps_map[i - 1][j] = steps_map[i][j] + 1;
          change_flag = true;
        }
      }
    }
  } while (change_flag == true);
}

int MapManager::priorityGet(unsigned char x, unsigned char y, t_global_direction dir)
{
  int priority;

  priority = 0;

  if (mypos.dir == dir) {
    priority = 2;
  } else if (
    ((mypos.dir == north) && (dir == south)) || ((mypos.dir == east) && (dir == west)) ||
    ((mypos.dir == south) && (dir == north)) || ((mypos.dir == west) && (dir == east))) {
    priority = 0;
  } else {
    priority = 1;
  }

  if (
    (wall[x][y].north == _UNKNOWN) || (wall[x][y].east == _UNKNOWN) ||
    (wall[x][y].south == _UNKNOWN) || (wall[x][y].west == _UNKNOWN)) {
    priority += 4;
  }

  return priority;
}
