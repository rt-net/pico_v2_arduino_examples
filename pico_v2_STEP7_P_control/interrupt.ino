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

void controlInterrupt(void)
{
  double spd_r, spd_l;

  g_speed += g_accel;

  if (g_speed > g_max_speed) {
    g_speed = g_max_speed;
  }
  if (g_speed < g_min_speed) {
    g_speed = g_min_speed;
  }

  if ((g_sen_r.is_control == true) && (g_sen_l.is_control == true)) {
    g_con_wall.error = g_sen_r.error - g_sen_l.error;
  } else {
    g_con_wall.error = 2.0 * (g_sen_r.error - g_sen_l.error);
  }

  g_con_wall.control = 0.001 * g_speed * g_con_wall.kp * g_con_wall.error;

  spd_r = g_speed + g_con_wall.control;
  spd_l = g_speed - g_con_wall.control;

  if (spd_r < g_min_speed) {
    spd_r = g_min_speed;
  }
  if (spd_l < g_min_speed) {
    spd_l = g_min_speed;
  }

  g_step_hz_r = (unsigned short)(spd_r / PULSE);
  g_step_hz_l = (unsigned short)(spd_l / PULSE);
}

void sensorInterrupt(void)
{
  static char cnt = 0;
  short temp_r, temp_l;

  switch (cnt) {
    case 0:
      temp_r = analogRead(AD4);
      temp_l = analogRead(AD1);

      digitalWrite(SLED_F, HIGH);
      for (int i = 0; i < 10; i++) {
        asm("nop \n");
      }
      g_sen_fr.value = analogRead(AD4) - temp_r;
      g_sen_fl.value = analogRead(AD1) - temp_l;
      digitalWrite(SLED_F, LOW);
      if (g_sen_fr.value > g_sen_fr.th_wall) {
        g_sen_fr.is_wall = true;
      } else {
        g_sen_fr.is_wall = false;
      }
      if (g_sen_fl.value > g_sen_fl.th_wall) {
        g_sen_fl.is_wall = true;
      } else {
        g_sen_fl.is_wall = false;
      }
      break;
    case 1:
      temp_r = analogRead(AD3);
      temp_l = analogRead(AD2);

      digitalWrite(SLED_S, HIGH);
      for (int i = 0; i < 10; i++) {
        asm("nop \n");
      }
      g_sen_r.value = analogRead(AD3) - temp_r;
      g_sen_l.value = analogRead(AD2) - temp_l;
      digitalWrite(SLED_S, LOW);
      if (g_sen_r.value > g_sen_r.th_wall) {
        g_sen_r.is_wall = true;
      } else {
        g_sen_r.is_wall = false;
      }
      if (g_sen_r.value > g_sen_r.th_control) {
        g_sen_r.error = g_sen_r.value - g_sen_r.ref;
        g_sen_r.is_control = true;
      } else {
        g_sen_r.error = 0;
        g_sen_r.is_control = false;
      }
      if (g_sen_l.value > g_sen_l.th_wall) {
        g_sen_l.is_wall = true;
      } else {
        g_sen_l.is_wall = false;
      }
      if (g_sen_l.value > g_sen_l.th_control) {
        g_sen_l.error = g_sen_l.value - g_sen_l.ref;
        g_sen_l.is_control = true;
      } else {
        g_sen_l.error = 0;
        g_sen_l.is_control = false;
      }
      g_battery_value = (double)analogReadMilliVolts(AD0) / 1.0 * (1.0 + 10.0);
      break;
    default:
      Serial.printf("error¥n¥r");
      break;
  }
  cnt++;
  if (cnt == 2) cnt = 0;
}