#include "JetiEX.h"

volatile bool idle = true;
volatile unsigned long start = 0, stop = 0;
volatile uint8_t buffer[32];
volatile int diff;
volatile int high2 = 0;
volatile int high = 0;
volatile int low2 = 0;
volatile int low = 0;
volatile int idx = 0;
volatile bool start_bit = true;
volatile uint8_t data = 0;
volatile int c_digit = 0;
volatile int c_buffer = 0;
volatile int c_buffer2 = 0;

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
  if(c_buffer != c_buffer2) {
    c_buffer2 = c_buffer;
    for(int i = 0; i <= c_buffer; i++) {
      Serial.print(buffer[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }


  return true;
}

void rxInterrupt()
{
  if (idle)
  {
    // set start / stop according to measure idle time
    digitalRead(context->rxPin) ? start = micros() : stop = micros();

    // check if 20ms idle time is reached (sync is over)
    if (stop - start > 20000)
    {
      high = stop-start;
      idle = false;
    }
  }
  else
  {
    //write data to buffer
    if(c_digit >= 7) {
      c_digit = 0;
      buffer[c_buffer] = data;
      c_buffer++;
    }



    // read data
    if (start < stop)
    {    
      ///LOW BITS///
      start = micros();
      low=(start-stop);
      if(start_bit) {low -= 100; start_bit = false;} //subtract start bit from data

      //write data
      for(int i = timeToBits(low, context->baud, 200); i > 0; i--) {
        data &= ~(1<<c_digit);
        c_digit++;
      }
      ////////////
    }
    else
    {
      ///HIGH BITS///
      stop = micros();
      // high bits
      high=(stop-start);

      //write data
      for(int i = timeToBits(high, context->baud, 200); i > 0; i--) {
        data |= (1<<c_digit);
        c_digit++;
      }
      ////////////
    }
  }
}

int timeToBits(int time, int baud, int baudTol) {
  return time / (1000000/baud+baudTol); //time divided by bit Timeus
}