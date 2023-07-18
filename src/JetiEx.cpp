// JetiEX.cpp
#include "JetiEX.h"

volatile bool risingEdge = false;
volatile bool fallingEdge = false;

JetiEx *context;

JetiEx::JetiEx(int baud, int rxPin) {
    this->rxPin = rxPin;
    this->baud = baud;
}

void JetiEx::begin() {
    pinMode(this->rxPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(this->rxPin), rxInterrupt, CHANGE);
    context = this;
    Serial.println("JetiEX initialized");
}

bool JetiEx::available() {
    if (risingEdge) {
    Serial.println("Rising edge interrupt occurred!");
    // Do something for the rising edge
    risingEdge = false; // Reset the flag
  }

  if (fallingEdge) {
    Serial.println("Falling edge interrupt occurred!");
    // Do something for the falling edge
    fallingEdge = false; // Reset the flag
  }
    // Check for data availability and process it
    // Implement your logic here to handle incoming data from JetiEX
    return false;
}

void rxInterrupt() {
}
