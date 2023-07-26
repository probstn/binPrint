#include <Arduino.h>
#include <vector>

double ex_getTemp();

void ex_run(void * pvParameters);

void ex_setup();

void ex_handler();

bool _isSync(unsigned int idx);

void ex_process();

void ex_evaluate();