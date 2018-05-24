#include "DebounceTimer.h"


DebounceTimer::DebounceTimer(int pin) :
  m_pin(pin)
{
  //get initial signal value
  m_prevSignal = digitalRead(m_pin);
}

int DebounceTimer::debounceSignal(unsigned long debounceTimeMs)
{
  //get current signal value
  int currentSignal = digitalRead(m_pin);

  //check if reference time needs updated
  if (currentSignal == LOW)
  {
    m_refTime = millis();
    return LOW;
  } //end  if (currentSignal == LOW)
  else
  {
    //only need to run this code if the signal is HIGH
    //get time signal has been HIGH
    unsigned long elapsedTime = millis() - m_refTime;

    return (elapsedTime >= debounceTimeMs) ? HIGH : LOW;
  } //end  else
}
