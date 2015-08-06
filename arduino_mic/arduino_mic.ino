#include <pt.h>
#define PT_DELAY(pt, ms, ts) \
    ts = millis(); \
    PT_WAIT_WHILE(pt, millis()-ts < (ms));

struct pt mode1;

PT_THREAD(ReadMic(struct pt* pt))
{
  static uint32_t ts;
  PT_BEGIN(pt);
  while (1)
  {
    static int micValue = analogRead(A0);
    Serial.print(micValue);
    Serial.print("\r");
    PT_DELAY(pt, 100, ts);
  }
  PT_END(pt);
}

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  PT_INIT(&mode1);
}

void loop() {
  ReadMic(&mode1);
}
