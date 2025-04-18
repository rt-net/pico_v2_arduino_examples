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

#ifndef SRC_SENSOR_H_
#define SRC_SENSOR_H_

#include "run.h"

typedef struct
{
  short value;
  short p_value;
  short error;
  short ref;
  short th_wall;
  short th_control;
  bool is_wall;
  bool is_control;
} t_sensor;


class SENSOR{
public:
	volatile t_sensor sen_r, sen_l, sen_fr, sen_fl;
	volatile short battery_value;
	void interrupt(void);
};

extern SENSOR g_sensor;

#endif /* SRC_SENSOR_H_ */