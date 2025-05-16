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

#ifndef SRC_RUN_H_
#define SRC_RUN_H_

typedef struct
{
  double control;
  double error;
  double p_error;
  double diff;
  double sum;
  double sum_max;
  double kp;
  double fkp;
  double kd;
  double ki;
  bool enable;
} t_control;

typedef enum {
  MOT_FORWARD = 1,  //TMC5240の方向に合わせた数字
  MOT_BACK = 2
} t_CW_CCW;

class RUN
{
private:
public:
  volatile double accel;
  volatile double speed;
  volatile double speed_target_r;
  volatile double speed_target_l;
  volatile double upper_speed_limit;
  volatile double lower_speed_limit;
  float search_accel;
  short search_speed;
  short max_speed;
  float turn_accel;
  t_control con_wall;

  float tire_diameter;
  float tread_width;
  float pulse;

  RUN();
  void interrupt(void);
  void counterClear(void);
  void straight(int len, int init_speed, int max_sp, int finish_speed);
  void accelerate(int len, int finish_speed);
  void oneStep(int len, int init_speed);
  void decelerate(int len, int init_speed);
  void rotate(t_local_direction dir, int times);

private:
  int step_lr_len, step_lr;
  void dirSet(t_CW_CCW dir_left, t_CW_CCW dir_right);
  void speedSet(double l_speed, double r_speed);
  void stepGet(void);
  void stay(float l_speed);
  void stop(void);
};

extern RUN g_run;

#endif /* SRC_RUN_H_ */