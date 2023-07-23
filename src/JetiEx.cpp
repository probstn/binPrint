#include "JetiEX.h"

volatile bool idle = true;
volatile int t = 0;
volatile int t2 = 0;
volatile int start = 0;
volatile int stop = 0;
volatile int i = 0;
volatile int i2 = 0;
volatile bool data_ready = false;
bool isLow = false;

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
  if (i != i2)
  {
    i2 = i;
    int bits = timeToBits(t, this->baud, 0);

    if (bits < 1000)
    {

      for (int i = 0; i < bits; i++)
      {
        if (isLow)
        {
          Serial.print("0");
        }
        else
        {
          Serial.print("1");
        }
      }
    }
    else
    {
      Serial.println("/////////////////////////////////");
    }
    isLow = !isLow;
  }

  return true;
}

void rxInterrupt()
{
  (start < stop) ? start = micros() : stop = micros(); // set start / stop for next measure
  t = std::abs(start - stop);                          // time since last interrupt
  i++;                                                 // count interrupts

  if (t > 20000)
    data_ready = true; /*data ready*/

  if (data_ready)
  {
    /*currently receiving*/
  }
}

int timeToBits(int time, int baud, int baudTol)
{
  return time / (100); // time divided by bit Timeus
}