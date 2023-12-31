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

void accelerate(int len, int tar_speed)
{
  int obj_step;
  g_max_speed = tar_speed;
  g_accel = 1.5;
  g_step_r = g_step_l = 0;
  g_speed = g_min_speed = MIN_SPEED;
  g_step_hz_r = g_step_hz_l = (unsigned short)(g_speed / PULSE);

  obj_step = (int)((float)len * 2.0 / PULSE);
  digitalWrite(CW_R, HIGH);
  digitalWrite(CW_L, LOW);
  g_motor_move = 1;

  while ((g_step_r + g_step_l) < obj_step) {
    continue;
  }
}

void oneStep(int len, int tar_speed)
{
  int obj_step;
  g_max_speed = tar_speed;
  g_accel = 0.0;
  g_step_r = g_step_l = 0;
  g_speed = g_min_speed = tar_speed;
  g_step_hz_r = g_step_hz_l = (unsigned short)(g_speed / PULSE);
  obj_step = (int)((float)len * 2.0 / PULSE);
  digitalWrite(CW_R, HIGH);
  digitalWrite(CW_L, LOW);

  while ((g_step_r + g_step_l) < obj_step) {
    continue;
  }
}

void decelerate(int len, int tar_speed)
{
  int obj_step;
  g_max_speed = tar_speed;
  g_accel = 0.0;
  g_step_r = g_step_l = 0;
  g_speed = g_min_speed = tar_speed;
  g_step_hz_r = g_step_hz_l = (unsigned short)(g_speed / PULSE);
  obj_step = (int)((float)len * 2.0 / PULSE);
  digitalWrite(CW_R, HIGH);
  digitalWrite(CW_L, LOW);

  while ((len - (g_step_r + g_step_l) / 2.0 * PULSE) >
         (((g_speed * g_speed) - (MIN_SPEED * MIN_SPEED)) / (2.0 * 1000.0 * 1.5))) {
    continue;
  }
  g_accel = -1.5;
  g_min_speed = MIN_SPEED;

  while ((g_step_r + g_step_l) < obj_step) {
    continue;
  }

  g_motor_move = 0;
}