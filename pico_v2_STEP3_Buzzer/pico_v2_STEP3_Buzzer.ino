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

#define LED0 42
#define LED1 41
#define LED2 15
#define LED3 16

#define SW_L 13
#define SW_R 14

#define BUZZER 40

#define INC_FREQ 2000
#define DEC_FREQ 1000

#define FREQ_C 523  //ド
#define FREQ_D 587  //レ
#define FREQ_E 659  //ミ

char g_mode;

void setLED(char data)
{
  if (data & 0x01) {
    digitalWrite(LED0, HIGH);
  } else {
    digitalWrite(LED0, LOW);
  }
  if (data & 0x02) {
    digitalWrite(LED1, HIGH);
  } else {
    digitalWrite(LED1, LOW);
  }
  if (data & 0x04) {
    digitalWrite(LED2, HIGH);
  } else {
    digitalWrite(LED2, LOW);
  }
  if (data & 0x08) {
    digitalWrite(LED3, HIGH);
  } else {
    digitalWrite(LED3, LOW);
  }
}

void execByMode(char mode)
{
  switch (mode) {
    case 1:
      ledcWriteTone(0, FREQ_C);
      delay(1000);
      ledcWrite(0, 1024);
      break;
    case 2:
      ledcWriteTone(0, FREQ_D);
      delay(1000);
      ledcWrite(0, 1024);
      break;
    case 3:
      ledcWriteTone(0, FREQ_E);
      delay(1000);
      ledcWrite(0, 1024);
      break;
    default:
      ledcWrite(0, 1024);
      break;
  }
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  pinMode(SW_L, INPUT);
  pinMode(SW_R, INPUT);

  ledcSetup(0, 440, 10);
  ledcAttachPin(BUZZER, 0);
  ledcWrite(0, 1024);

  g_mode = 1;
  setLED(g_mode);
}

void loop()
{
  // put your main code here, to run repeatedly:
  while (digitalRead(SW_L) & digitalRead(SW_R)) {
    continue;
  }
  if (digitalRead(SW_R) == 0) {
    g_mode++;
    if (g_mode > 15) {
      g_mode = 1;
    } else {
      ledcWriteTone(0, INC_FREQ);
      delay(30);
      ledcWrite(0, 1024);
    }
    setLED(g_mode);
  }
  if (digitalRead(SW_L) == 0) {
    ledcWriteTone(0, INC_FREQ);
    delay(80);
    ledcWriteTone(0, DEC_FREQ);
    delay(80);
    ledcWrite(0, 1024);
    delay(300);
    execByMode(g_mode);
  }
  while (!(digitalRead(SW_L) & digitalRead(SW_R))) {
    continue;
  }
  delay(30);
}
