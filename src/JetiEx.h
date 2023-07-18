#include <Arduino.h>

#ifndef JetiEX_h
#define JetiEX_h

class JetiEx
{
private:
    int baud;

public:
    int rxPin;
    
    JetiEx(int baud, int rxPin);
    void begin();
    bool available();
    void getData();
};

void rxInterrupt();

#endif