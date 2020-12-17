#include <Clock.h>

Clock::Clock(){
    seconds = 0;
    minutes = 0;
    hours = 0;
    days = 0;
    months = 0;
    years = 0; 
}

void Clock::advance(){
      seconds++;
      if(seconds == 60){
        seconds = 0;
        minutes++;
      }
      if(minutes == 60){
        minutes = 0;
        hours++;  
      }
      if(hours == 24){
        hours = 0;
        days++;  
      }
      if(months == 1 || months == 3 || months == 5 || months == 7 || months == 8 || months == 10 || months ==12){
        if(days == 32){
          days = 1;
          months++;  
        }  
      }else if(months == 2){
        if(isLeapYear(years)){
          if(days == 30){
            days = 1;
            months++;  
          }  
        }else{
          if(days == 29){
            days = 1;
            months++;  
          }   
        }  
      }else{
        if(days == 31){
          days = 1;
          months++; 
        }  
      }
      if(months==13){
        months = 1;
        years++;  
      }   
    }

boolean Clock::isLeapYear(int year){
    if((year%4==0 && year%100!=0) || year%400==0){
      return true;
    }else{
      return false;  
    }
  }
