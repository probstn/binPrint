#include "JetiEX.h"

volatile bool idle = true;
volatile int t = 0;
volatile int t2 = 0;
volatile int start = 0;
volatile int stop = 0;
volatile int i = 0;
volatile int i2 = 0;

JetiEx *context;

int timeToBits(int time, int baud, int baudTol);

JetiEx::JetiEx(int baud, int rxPin)
{
  this->rxPin = rxPin;
  this->baud = baud;
  this->bitTimeus = 1000000 / baud;
}

void JetiEx::begin()
{
  pinMode(this->rxPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(this->rxPin), rxInterrupt, CHANGE);
  context = this;
  Serial.println("JetiEX initialized");
}

bool JetiEx::available()
{
  if(i != i2) {
    i2 = i;
    Serial.println(t);
  }


  return true;
}

void rxInterrupt()
{
  /*
  if (idle)
  {
    // set start / stop according to measure idle time
    digitalRead(context->rxPin) ? start = micros() : stop = micros();

    // check if 20ms idle time is reached (sync is over)
    if (stop - start > 900000)
    {
      idle = false;
    }
  }
  else
  {
  */
      (start < stop) ? start = micros() : stop=micros(); //set start / stop for next measure
      t=std::abs(start-stop); //time since last interrupt
      i++;
  //}
}

int timeToBits(int time, int baud, int baudTol) {
  return time / (1000000/baud+baudTol); //time divided by bit Timeus
}