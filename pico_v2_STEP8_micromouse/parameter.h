#ifndef PARAMETER_H_
#define PARAMETER_H_

#define TIRE_DIAMETER (24.7)
#define TREAD_WIDTH (31.5)
#define TREAD_CIRCUIT (TREAD_WIDTH * PI / 4)
#define PULSE (TIRE_DIAMETER * PI / (35.0 / 10.0 * 20.0 * 8.0))
#define MIN_HZ 40

#define WAITLOOP_SLED 10

#define REF_SEN_R 394
#define REF_SEN_L 554

#define TH_SEN_R 150
#define TH_SEN_L 146
#define TH_SEN_FR 192
#define TH_SEN_FL 202

#define CONTROL_TH_SEN_R TH_SEN_R
#define CONTROL_TH_SEN_L TH_SEN_L

#define CON_WALL_KP 0.12
#define SEARCH_ACCEL 1.5
#define TURN_ACCEL 0.3

#define SEARCH_SPEED 200
#define MAX_SPEED 600
#define MIN_SPEED (MIN_HZ * PULSE)

#define GOAL_X 3
#define GOAL_Y 3

#define INC_FREQ 3000
#define DEC_FREQ 2000

#define BATT_MAX 4000
#define BATT_MIN 3800

#define HALF_SECTION 45
#define SECTION 90

#endif  // PARAMETER_H_