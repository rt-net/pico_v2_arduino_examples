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

#include "adjust.h"

ADJUST g_adjust;

void ADJUST::mapView(void)
{
  Serial.printf("\x1b[2j");
  Serial.printf("\x1b[0;0H");
  Serial.printf("\n\r+");
  for (int i = 0; i < MAZESIZE_X; i++) {
    switch (g_map.wallDataGet(i, MAZESIZE_Y - 1, north)) {  //黒色は"[30m"
      case NOWALL:
        Serial.printf("\x1b[37m  +");  //NOWALL
        break;
      case WALL:
        Serial.printf("\x1b[37m--+");  //WALL
        break;
      case _UNKNOWN:
        Serial.printf("\x1b[31m--+");  //UNNOWN
        break;
      default:
        Serial.printf("\x1b[33m--+");  //VWALL
        break;
    }
  }
  Serial.printf("\n\r");
  for (int j = (MAZESIZE_Y - 1); j > -1; j--) {
    switch (g_map.wallDataGet(0, j, west)) {
      case NOWALL:
        Serial.printf("\x1b[37m ");  //NOWALL
        break;
      case WALL:
        Serial.printf("\x1b[37m|");  //WALL
        break;
      case _UNKNOWN:
        Serial.printf("\x1b[31m|");  //UNNOWN
        break;
      default:
        Serial.printf("\x1b[33m|");  //VWALL
        break;
    }
    for (int i = 0; i < MAZESIZE_X; i++) {
      switch (g_map.wallDataGet(i, j, east)) {
        case NOWALL:
          Serial.printf("\x1b[37m   ");  //NOWALL
          break;
        case WALL:
          Serial.printf("\x1b[37m  |");  //WALL
          break;
        case _UNKNOWN:
          Serial.printf("\x1b[31m  |");  //UNNOWN
          break;
        default:
          Serial.printf("\x1b[33m  |");  //VWALL
          break;
      }
    }
    Serial.printf("\n\r+");
    for (int i = 0; i < MAZESIZE_X; i++) {
      switch (g_map.wallDataGet(i, j, south)) {
        case NOWALL:
          Serial.printf("\x1b[37m  +");  //NOWALL
          break;
        case WALL:
          Serial.printf("\x1b[37m--+");  //WALL
          break;
        case _UNKNOWN:
          Serial.printf("\x1b[31m--+");  //UNNOWN
          break;
        default:
          Serial.printf("\x1b[33m--+");  //VWALL
          break;
      }
    }
    Serial.printf("\n\r");
  }
}

void ADJUST::adcView(void)
{
  motorDisable();

  while (1) {
    Serial.printf("r_sen        is\t%d   \r\n", g_sensor.sen_r.value);
    Serial.printf("fr_sen       is\t%d   \r\n", g_sensor.sen_fr.value);
    Serial.printf("fl_sen       is\t%d  \r\n", g_sensor.sen_fl.value);
    Serial.printf("l_sen        is\t%d   \r\n", g_sensor.sen_l.value);
    Serial.printf("VDD          is\t%d mV\r\n", g_sensor.battery_value);
    Serial.printf("\n\r");  //改行
    delay(100);
    Serial.printf("\x1b[2j");
    Serial.printf("\x1b[0;0H");
  }
}

void ADJUST::straightCheck(int section_check)
{
  motorEnable();
  delay(1000);
  g_run.accelerate(HALF_SECTION, g_run.search_speed);
  if (section_check > 1) {
    g_run.straight(
      SECTION * (section_check - 1), g_run.search_speed, g_run.max_speed, g_run.search_speed);
  }
  g_run.decelerate(HALF_SECTION, g_run.search_speed);

  motorDisable();
}

void ADJUST::rotationCheck(void)
{
  motorEnable();
  delay(1000);
  for (int i = 0; i < 8; i++) {
    g_run.rotate(right, 1);
  }
  motorDisable();
}

void ADJUST::menu(void)
{
  unsigned char l_mode = 1;
  char LED3_data;
  char sw;

  while (1) {
    ledSet(l_mode);
    while (1) {
      sw = switchGet();
      if (sw != 0) break;
      delay(33);
      LED3_data ^= 1;
      ledSet((l_mode & 0x7) + ((LED3_data << 3) & 0x08));
    }
    LED3_data = 0;
    switch (sw) {
      case SW_RM:
        l_mode = g_misc.buttonInc(l_mode, 7, 1);
        break;
      case SW_LM:
        g_misc.buttonOk();
        if (modeExec(l_mode) == 1) {
          return;
        }
        break;
      default:
        break;
    }
  }
}

unsigned char ADJUST::modeExec(unsigned char l_mode)
{
  motorDisable();
  switch (l_mode) {
    case 1:
      buzzerEnable(INC_FREQ);
      delay(30);
      buzzerDisable();
      delay(500);
      buzzerEnable(INC_FREQ);
      delay(30);
      buzzerDisable();
      webServerSetup();
      //webserverでセンサの値のばらつきが大きすぎて判断できない時は、webServerSetup()をコメントアウトしadcView()のコメントをはずし、シリアルモニタで確認してください。
      //      adcView();
      break;
    case 2:
      straightCheck(9);
      break;

    case 3:
      rotationCheck();
      break;
    case 4:
      mapCopy();
      mapView();
      break;

    case 5:
      break;

    case 6:
      break;

    default:
      return 1;
      break;
  }

  return 0;
}
