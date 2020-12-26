#include <Clock.h>

Clock::Clock()
{
  seconds = 0;
  minutes = 0;
  hours = 0;
  days = 0;
  months = 0;
  years = 0;
}

void Clock::advance()
{
  seconds++;
  if (seconds > 59)
  {
    seconds = 0;
    minutes++;
  }
  if (minutes > 59)
  {
    minutes = 0;
    hours++;
  }
  if (hours > 23)
  {
    hours = 0;
    days++;
  }
  if (months == 1 || months == 3 || months == 5 || months == 7 || months == 8 || months == 10 || months == 12)
  {
    if (days > 31)
    {
      days = 1;
      months++;
    }
  }
  else if (months == 2)
  {
    if (isLeapYear(years))
    {
      if (days > 29)
      {
        days = 1;
        months++;
      }
    }
    else
    {
      if (days > 28)
      {
        days = 1;
        months++;
      }
    }
  }
  else
  {
    if (days > 30)
    {
      days = 1;
      months++;
    }
  }
  if (months > 12)
  {
    months = 1;
    years++;
  }
}

boolean Clock::isLeapYear(int year)
{
  if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

int Clock::numberOfDays(int month, int year){
  switch(month){
    case 1:{
      return 31;
    }
    case 2:{
      if(isLeapYear(year)){
        return 29;
      }else{
        return 28;
      }
    }
    case 3:{
      return 31;
    }
    case 4:{
      return 30;
    }
    case 5:{
      return 31;
    }
    case 6:{
      return 30;
    }
    case 7:{
      return 31;
    }
    case 8:{
      return 31;
    }
    case 9:{
      return 30;
    }
    case 10:{
      return 31;
    }
    case 11:{
      return 30;
    }
    case 12:{
      return 31;
    }
  }
}
