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


#include "sensor.h"

SENSOR g_sensor;


SENSOR::SENSOR() {  //コンストラクタ

  sen_r.ref = REF_SEN_R;
  sen_l.ref = REF_SEN_L;

  sen_r.th_wall = TH_SEN_R;
  sen_l.th_wall = TH_SEN_L;

  sen_fr.th_wall = TH_SEN_FR;
  sen_fl.th_wall = TH_SEN_FL;

  sen_r.th_control = CONTH_SEN_R;
  sen_l.th_control = CONTH_SEN_L;


}

void sensorInterrupt(void) {
  g_sensor.interrupt();
}


void SENSOR::interrupt(void) 
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
      sen_fr.value = analogRead(AD4) - temp_r;
      sen_fl.value = analogRead(AD1) - temp_l;
      digitalWrite(SLED_F, LOW);
      if (sen_fr.value > sen_fr.th_wall) {
        sen_fr.is_wall = true;
      } else {
        sen_fr.is_wall = false;
      }
      if (sen_fl.value > sen_fl.th_wall) {
        sen_fl.is_wall = true;
      } else {
        sen_fl.is_wall = false;
      }
      break;
    case 1:
      temp_r = analogRead(AD3);
      temp_l = analogRead(AD2);

      digitalWrite(SLED_S, HIGH);
      for (int i = 0; i < 10; i++) {
        asm("nop \n");
      }
      sen_r.value = analogRead(AD3) - temp_r;
      sen_l.value = analogRead(AD2) - temp_l;
      digitalWrite(SLED_S, LOW);
      if (sen_r.value > sen_r.th_wall) {
        sen_r.is_wall = true;
      } else {
        sen_r.is_wall = false;
      }
      if (sen_r.value > sen_r.th_control) {
        sen_r.error = sen_r.value - sen_r.ref;
        sen_r.is_control = true;
      } else {
        sen_r.error = 0;
        sen_r.is_control = false;
      }
      if (sen_l.value > sen_l.th_wall) {
        sen_l.is_wall = true;
      } else {
        sen_l.is_wall = false;
      }
      if (sen_l.value > sen_l.th_control) {
        sen_l.error = sen_l.value - sen_l.ref;
        sen_l.is_control = true;
      } else {
        sen_l.error = 0;
        sen_l.is_control = false;
      }
      battery_value = (double)analogReadMilliVolts(AD0) / 1.0 * (1.0 + 10.0);
      break;
    default:
      Serial.printf("error\n\r");
      break;
  }
  cnt++;
  if (cnt >= 2) cnt = 0;
}
