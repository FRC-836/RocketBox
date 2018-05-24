#ifndef DEBOUNCE_TIMER_H
#define DEBOUNCE_TIMER_H

#include "Arduino.h"

class DebounceTimer
{
  private:
    unsigned long m_refTime;
    int m_prevSignal;
    const int m_pin;

  public:
    DebounceTimer(int pin);

    int debounceSignal(unsigned long debounceTimeMs);
};

#endif
