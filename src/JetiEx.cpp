#include "JetiEx.h"

using namespace std;

#define INTERRUPT_PIN 17

#define SYNC_LENGTH 200

#define BAUD 9600

#define RING_BUFFER_SIZE 700

unsigned long timings[RING_BUFFER_SIZE];
vector<uint8_t> data;

unsigned int syncIndex1 = 0; // index of the first sync signal
unsigned int syncIndex2 = 0; // index of the second sync signal
bool received = false;

void ex_setup()
{
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), ex_handler, CHANGE);
  Serial.println("[EX] Setup");
}

bool _isSync(unsigned int index)
{
  if(timings[index] < SYNC_LENGTH) return false;
  return true;
}

void ex_handler()
{
  static unsigned int duration = 0;
  static unsigned long start = 0;
  static unsigned long stop = 0;
  static unsigned int ringIndex = 0;
  static unsigned int syncCount = 0;
  // ignore if we haven't processed the previous received signal
  if (received == true)
  {
    return;
  }
  // calculating timing since last change
  (start < stop) ? start = micros() : stop = micros(); // set start / stop for next measure
  
  if(start<stop) duration = (stop-start)/100;
  else duration = (start-stop)/100;

  // store data in ring buffer
  ringIndex = (ringIndex + 1) % RING_BUFFER_SIZE;
  timings[ringIndex] = duration;
  // detect sync signal
  if (_isSync(ringIndex))
  {
    syncCount++;
    // first time sync is seen, record buffer index
    if (syncCount == 1)
    {
      syncIndex1 = (ringIndex + 1) % RING_BUFFER_SIZE;
    }
    else if (syncCount == 2)
    {
      // second time sync is seen, start bit conversion
      syncCount = 0;
      syncIndex2 = (ringIndex) % RING_BUFFER_SIZE;
      unsigned int changeCount = (syncIndex2 < syncIndex1) ? (syncIndex2 + RING_BUFFER_SIZE - syncIndex1) : (syncIndex2 - syncIndex1);
      received = true;
    }
  }
}

void ex_process()
{
  if (!received)
    return;

  uint8_t dataByte = 0;
  int dataIndex = 0;

  int process_state = 0;
  bool isHigh = true;
  Serial.println("[EX] Received");
  // disable interrupt to avoid new data corrupting the buffer
  detachInterrupt(INTERRUPT_PIN);

  // loop over buffer data
  for (unsigned int i = syncIndex1, y = 0; i != syncIndex2; i = (i + 1) % RING_BUFFER_SIZE, y++)
  {
    isHigh = !isHigh;
    for (int bit = 0; bit < timings[i]; bit++)
    {
      if (dataIndex > 7)
      {
        Serial.print(dataByte, HEX); // debugging raw hex data
        //data.push_back(dataByte);
        dataIndex = 0;
        process_state++;
      }

      switch (process_state)
      {
      case 0:
        //Serial.print("Start: ");
        //Serial.println(isHigh);
        // 1 start bit
        process_state++;
        break;

      case 1:
        // 8 data-bits
        //Serial.print("Data ");
        //Serial.print(dataIndex);
        //Serial.print(": ");
        //Serial.println(isHigh);
        isHigh ? dataByte |= (1 << dataIndex) : dataByte &= ~(1 << dataIndex);
        dataIndex++;
        break;

      case 2:
        // 1 mode-bit
        //Serial.print("Mode: ");
        //Serial.println(isHigh);
        process_state++;
        break;

      case 3:
        // 1 odd-parity-bit
        //Serial.print("Parity: ");
        //Serial.println(isHigh);
        process_state++;
        break;

      case 4:
        // 1 stop-bit
        //Serial.print("Stop: ");
        //Serial.println(isHigh);
        process_state++;
        break;

      case 5:
        // 2 stop-bit
        //Serial.print("Stop: ");
        //Serial.println(isHigh);
        process_state = 0;
        break;
      }
    }
  }

  //evaluate data
  //ex_evaluate();
  data.clear();

  // ready for next data
  received = false;
  syncIndex1 = 0;
  syncIndex2 = 0;
  attachInterrupt(INTERRUPT_PIN, ex_handler, CHANGE);
}

void ex_evaluate()
{
  Serial.println("[EX] Evaluate");
  Serial.print("[EX] Data: ");
  for (int i = 0; i < data.size(); i++)
  {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println("[EX] End");
}