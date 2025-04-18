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

hw_timer_t * g_timer0 = NULL;
hw_timer_t * g_timer1 = NULL;
hw_timer_t * g_timer2 = NULL;
hw_timer_t * g_timer3 = NULL;

volatile bool g_motor_move = 0;
volatile unsigned int g_step_r, g_step_l;
unsigned short g_step_hz_r = 30;
unsigned short g_step_hz_l = 30;

portMUX_TYPE g_timer_mux = portMUX_INITIALIZER_UNLOCKED;

void stepHzSetR(short data) { g_step_hz_r = data; }
void stepHzSetL(short data) { g_step_hz_l = data; }
void stepClearR(void) { g_step_r = 0; }
void stepClearL(void) { g_step_l = 0; }
unsigned int stepGetR(void) { return g_step_r; }
unsigned int stepGetL(void) { return g_step_l; }
void motorMoveSet(bool data) { g_motor_move = data; }

void IRAM_ATTR onTimer0(void)
{
  portENTER_CRITICAL_ISR(&g_timer_mux);
  controlInterrupt();
  portEXIT_CRITICAL_ISR(&g_timer_mux);
}

void IRAM_ATTR onTimer1(void)
{
  portENTER_CRITICAL_ISR(&g_timer_mux);
  sensorInterrupt();
  portEXIT_CRITICAL_ISR(&g_timer_mux);
}

void IRAM_ATTR isrR(void)
{
  portENTER_CRITICAL_ISR(&g_timer_mux);
  if (g_motor_move) {
    if (g_step_hz_r < 30) g_step_hz_r = 30;
    timerAlarm(g_timer2, 2000000 / g_step_hz_r, true, 0);
    digitalWrite(PWM_R, HIGH);
    for (int i = 0; i < 100; i++) {
      asm("nop \n");
    }
    digitalWrite(PWM_R, LOW);
    g_step_r++;
  }
  portEXIT_CRITICAL_ISR(&g_timer_mux);
}

void IRAM_ATTR isrL(void)
{
  portENTER_CRITICAL_ISR(&g_timer_mux);
  if (g_motor_move) {
    if (g_step_hz_l < 30) g_step_hz_l = 30;
    timerAlarm(g_timer3, 2000000 / g_step_hz_l, true, 0);
    digitalWrite(PWM_L, HIGH);
    for (int i = 0; i < 100; i++) {
      asm("nop \n");
    };
    digitalWrite(PWM_L, LOW);
    g_step_l++;
  }
  portEXIT_CRITICAL_ISR(&g_timer_mux);
}

void controlInterruptStart(void) { timerStart(g_timer0); }
void controlInterruptStop(void) { timerStop(g_timer0); }

void sensorInterruptStart(void) { timerStart(g_timer1); }
void sensorInterruptStop(void) { timerStop(g_timer1); }

void PWMInterruptStart(void)
{
  timerStart(g_timer2);
  timerStart(g_timer3);
}
void PWMInterruptStop(void)
{
  timerStop(g_timer2);
  timerStop(g_timer3);
}

void deviceInit(void)
{
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  pinMode(BLED0, OUTPUT);
  pinMode(BLED1, OUTPUT);

  pinMode(SW_L, INPUT_PULLUP);
  pinMode(SW_R, INPUT_PULLUP);

  ledcAttach(BUZZER, 440, 10);
  ledcWrite(BUZZER, 0);

  pinMode(SLED_F, OUTPUT);
  pinMode(SLED_S, OUTPUT);
  digitalWrite(SLED_F, LOW);
  digitalWrite(SLED_S, LOW);

  pinMode(MOTOR_EN, OUTPUT);
  pinMode(CW_R, OUTPUT);
  pinMode(CW_L, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  pinMode(PWM_L, OUTPUT);
  digitalWrite(MOTOR_EN, LOW);
  digitalWrite(CW_R, LOW);
  digitalWrite(CW_L, LOW);
  digitalWrite(PWM_R, LOW);
  digitalWrite(PWM_L, LOW);

  g_timer0 = timerBegin(1000000);
  timerAttachInterrupt(g_timer0, &onTimer0);
  timerAlarm(g_timer0, 1000, true, 0);
  timerStart(g_timer0);

  g_timer1 = timerBegin(1000000);
  timerAttachInterrupt(g_timer1, &onTimer1);
  timerAlarm(g_timer1, 500, true, 0);
  timerStart(g_timer1);

  g_timer2 = timerBegin(2000000);
  timerAttachInterrupt(g_timer2, &isrR);
  timerAlarm(g_timer2, 13333, true, 0);
  timerStart(g_timer2);

  g_timer3 = timerBegin(2000000);
  timerAttachInterrupt(g_timer3, &isrL);
  timerAlarm(g_timer3, 13333, true, 0);
  timerStart(g_timer3);

  Serial.begin(115200);
}

//LED
void ledSet(unsigned char data)
{
  digitalWrite(LED0, data & 0x01);
  digitalWrite(LED1, (data >> 1) & 0x01);
  digitalWrite(LED2, (data >> 2) & 0x01);
  digitalWrite(LED3, (data >> 3) & 0x01);
}
void bledSet(char data)
{
  if (data & 0x01) {
    digitalWrite(BLED0, HIGH);
  } else {
    digitalWrite(BLED0, LOW);
  }
  if (data & 0x02) {
    digitalWrite(BLED1, HIGH);
  } else {
    digitalWrite(BLED1, LOW);
  }
}

//Buzzer
void buzzerEnable(short f) { ledcWriteTone(BUZZER, f); }
void buzzerDisable(void) { ledcWrite(BUZZER, 0); }  //duty 0% Buzzer OFF

//motor
void motorEnable(void) { digitalWrite(MOTOR_EN, HIGH); }  //Power ON
void motorDisable(void) { digitalWrite(MOTOR_EN, LOW); }  //Power OFF

void motorDirectionSet(t_CW_CCW left_CW, t_CW_CCW right_CW)
{  //左右のモータの回転方向を指示する
  if (right_CW == MOT_FORWARD) {
    digitalWrite(CW_R, HIGH);
  } else {
    digitalWrite(CW_R, LOW);
  }

  if (left_CW == MOT_FORWARD) {
    digitalWrite(CW_L, LOW);
  } else {
    digitalWrite(CW_L, HIGH);
  }
}

//SWITCH
unsigned char switchGet(void)
{
  unsigned char ret = 0;
  if (digitalRead(SW_R) == LOW) {
    do {
      delay(20);
    } while (digitalRead(SW_R) == LOW);
    ret |= SW_RM;
  }
  if (digitalRead(SW_L) == LOW) {
    do {
      delay(20);
    } while (digitalRead(SW_L) == LOW);
    ret |= SW_LM;
  }
  return ret;
}

//sensor
void sensorGetS(volatile short * r_value, volatile short * l_value)
{
  short temp_r, temp_l;
  temp_r = analogRead(AD3);
  temp_l = analogRead(AD2);
  digitalWrite(SLED_S, HIGH);
  for (int i = 0; i < WAITLOOP_SLED; i++) {
    asm("nop \n");
  }
  *r_value = analogRead(AD3) - temp_r;
  *l_value = analogRead(AD2) - temp_l;
  digitalWrite(SLED_S, LOW);
}

void sensorGetF(volatile short * fr_value, volatile short * fl_value)
{
  short temp_r, temp_l;
  temp_r = analogRead(AD4);
  temp_l = analogRead(AD1);
  digitalWrite(SLED_F, HIGH);  //LED点灯
  for (int i = 0; i < WAITLOOP_SLED; i++) {
    asm("nop \n");
  }
  *fr_value = analogRead(AD4) - temp_r;
  *fl_value = analogRead(AD1) - temp_l;
  digitalWrite(SLED_F, LOW);  //LED消灯
}

short batteryVoltGet(void)
{
  return (short)((double)analogReadMilliVolts(AD0) / 1.0 * (1.0 + 10.0));
}
