#include "JetiEx.h"

JetiEx jetiex = JetiEx(9600, 17);

void setup() {
  Serial.begin(115200);
  jetiex.begin();
}

void loop() {
  jetiex.available();
}