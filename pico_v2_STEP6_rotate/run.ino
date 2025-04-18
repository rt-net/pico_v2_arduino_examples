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

RUN g_run;

RUN::RUN()
{
  speed = 0.0;
  accel = 0.0;
}

//割り込み
void controlInterrupt(void) { g_run.interrupt(); }

void RUN::interrupt(void)
{  //割り込み内からコール
  speed += accel;

  if (speed > max_speed) {
    speed = max_speed;
  }
  if (speed < min_speed) {
    speed = min_speed;
  }

  speedSet(speed, speed);
}

void RUN::dirSet(t_CW_CCW dir_left, t_CW_CCW dir_right)
{
  if (dir_left == MOT_FORWARD) {
    digitalWrite(CW_L, LOW);
  } else {
    digitalWrite(CW_L, HIGH);
  }
  if (dir_right == MOT_FORWARD) {
    digitalWrite(CW_R, HIGH);
  } else {
    digitalWrite(CW_R, LOW);
  }
}

void RUN::counterClear(void) { g_step_r = g_step_l = 0; }

void RUN::speedSet(double l_speed, double r_speed)
{
  g_step_hz_l = (int)(l_speed / PULSE);
  g_step_hz_r = (int)(r_speed / PULSE);
}

void RUN::stepGet(void)
{
  step_lr = g_step_r + g_step_l;
  step_lr_len = (int)((float)step_lr / 2.0 * PULSE);
}

void RUN::stop(void) { g_motor_move = 0; }

void RUN::accelerate(int len, int finish_speed)
{
  int obj_step;

  accel = 1.5;
  speed = min_speed = MIN_SPEED;
  max_speed = finish_speed;
  counterClear();
  speedSet(speed, speed);
  dirSet(MOT_FORWARD, MOT_FORWARD);
  obj_step = (int)((float)len * 2.0 / PULSE);
  g_motor_move = 1;

  while (1) {
    stepGet();
    if (step_lr > obj_step) {
      break;
    }
  }
}

void RUN::oneStep(int len, int init_speed)
{
  int obj_step;

  accel = 0.0;
  max_speed = init_speed;
  speed = min_speed = init_speed;
  counterClear();
  speedSet(init_speed, init_speed);
  dirSet(MOT_FORWARD, MOT_FORWARD);
  obj_step = (int)((float)len * 2.0 / PULSE);

  while (1) {
    stepGet();
    if (step_lr > obj_step) {
      break;
    }
  }
}

void RUN::decelerate(int len, int init_speed)
{
  int obj_step;

  accel = 1.5;
  max_speed = init_speed;
  speed = min_speed = init_speed;
  counterClear();
  speedSet(init_speed, init_speed);
  dirSet(MOT_FORWARD, MOT_FORWARD);
  obj_step = (int)((float)len * 2.0 / PULSE);

  while (1) {
    stepGet();
    if (
      (len - step_lr_len) <
      (int)(((speed * speed) - (MIN_SPEED * MIN_SPEED)) / (2.0 * 1000.0 * accel))) {
      break;
    }
  }
  accel = -1.5;
  min_speed = MIN_SPEED;

  while (1) {
    stepGet();
    if (step_lr > obj_step) {
      break;
    }
  }

  stop();
}

void RUN::rotate(t_local_direction dir, int times)
{
  int obj_step;

  accel = 0.3;
  max_speed = 200.0;
  speed = min_speed = MIN_SPEED;
  counterClear();
  speedSet(MIN_SPEED, MIN_SPEED);
  obj_step = (int)(TREAD_WIDTH * PI / 4.0 * (float)times * 2.0 / PULSE);

  switch (dir) {
    case right:
      dirSet(MOT_FORWARD, MOT_BACK);
      g_motor_move = 1;
      break;
    case left:
      dirSet(MOT_BACK, MOT_FORWARD);
      g_motor_move = 1;
      break;
    default:
      dirSet(MOT_FORWARD, MOT_FORWARD);
      g_motor_move = 0;
      break;
  }

  while (1) {
    stepGet();
    if (
      (int)((float)obj_step / 2.0f * PULSE - step_lr_len) <
      (int)(((speed * speed) - (MIN_SPEED * MIN_SPEED)) / (2.0 * 1000.0 * accel))) {
      break;
    }
  }
  accel = -0.3;
  min_speed = MIN_SPEED;

  while (1) {
    stepGet();
    if (step_lr > obj_step) {
      break;
    }
  }
  stop();
}
