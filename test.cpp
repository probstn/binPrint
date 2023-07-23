#include "JetiEx.h"

JetiEx jetiex = JetiEx(9600, 2);

void setup() {
  Serial.begin(9600);
  jetiex.begin();
}

void loop() {
  jetiex.available();
}