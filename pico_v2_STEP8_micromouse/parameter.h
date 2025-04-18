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

#ifndef PARAMETER_H_
#define PARAMETER_H_

#define TIRE_DIAMETER_INIT (24.7)
#define TREAD_WIDTH_INIT (31.5)

#define REF_SEN_R_INIT 394
#define REF_SEN_L_INIT 554

#define TH_SEN_R_INIT 150
#define TH_SEN_L_INIT 146
#define TH_SEN_FR_INIT 192
#define TH_SEN_FL_INIT 202

#define GOAL_X_INIT 3
#define GOAL_Y_INIT 3

#define WAITLOOP_SLED 10

#define CON_WALL_KP_INIT 0.12
#define SEARCH_ACCEL_INIT 1.5
#define TURN_ACCEL_INIT 0.3

#define SEARCH_SPEED_INIT 200
#define MAX_SPEED_INIT 600
#define MIN_SPEED 10

#define INC_FREQ 3000
#define DEC_FREQ 2000

#define BATT_MAX 4000
#define BATT_MIN 3250

#define HALF_SECTION 45
#define SECTION 90

#endif  // PARAMETER_H_