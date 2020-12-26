#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
class Clock
{
  boolean isLeapYear(int year);

public:
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t days;
  uint8_t months;
  int years;
  Clock();
  void advance();
  int numberOfDays(int month,int year);
};

#endif