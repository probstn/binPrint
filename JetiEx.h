#include <Arduino.h>

#ifndef JetiEX_h
#define JetiEX_h

class JetiEx
{
private:
public:
    int bitTimeus; //bit Time in microseconds
    int rxPin;
    int baud;
    
    JetiEx(int baud, int rxPin);
    void begin();
    bool available();
    void getData();
};

void rxInterrupt();

#endif