#include <Arduino.h>

#include <DHT.h>
#include <virtuabotixRTC.h>
#include <Clock.h>

#define DHTPIN A0
#define DHTTYPE DHT11

#define DIGIT_DASH 20
#define DIGIT_H 21
#define DIGIT_C 22
#define DIGIT_DEGREE 23
#define DIGIT_EMPTY 24

#define DELAY_DAY 1
#define DELAY_NIGHT 0

DHT dht(DHTPIN, DHTTYPE);
virtuabotixRTC RTC(13, A1, A2);
Clock clk;

void screenUpdate(int C1, int C2, int C3, int C4, int C5, int C6, int C7, int C8);
void printTime();
void printDate();
void printTemp(); 
void printHumidity();

const int clock = 13;
const int latch = 12;
const int data = 8;

const int digit1 = 10;
const int digit2 = 9;
const int digit3 = 7;
const int digit4 = 6;
const int digit5 = 5;
const int digit6 = 4;
const int digit7 = 3;
const int digit8 = 2;

const byte digit[25] =
{
  B00000011, //0
  B10011111, //1
  B00100101, //2
  B00001101, //3
  B10011001, //4
  B01001001, //5
  B01000001, //6
  B00011011, //7
  B00000001, //8
  B00001001, //9
  B00000010, //0.
  B10011110, //1.
  B00100100, //2.
  B00001100, //3.
  B10011000, //4.
  B01001000, //5.
  B01000000, //6.
  B00011010, //7.
  B00000000, //8.
  B00001000, //9.
  B11111101, //-
  B10010001, //H
  B01100011, //C
  B00111001, //*
  B11111111  //empty
};

int secondCounterPrev = 0;
int secondCounterNew = 0;

float humidity;
float temperature;

int modeTimer = 0;
int secondTimeBase;

void setup() {
  dht.begin();
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  pinMode(digit5, OUTPUT);
  pinMode(digit6, OUTPUT);
  pinMode(digit7, OUTPUT);
  pinMode(digit8, OUTPUT);
  digitalWrite(digit1,LOW);
  digitalWrite(digit2,LOW);
  digitalWrite(digit3,LOW);
  digitalWrite(digit4,LOW);
  digitalWrite(digit5,LOW);
  digitalWrite(digit6,LOW);
  digitalWrite(digit7,LOW);
  digitalWrite(digit8,LOW);

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  RTC.updateTime();
  clk.seconds = RTC.seconds;
  clk.minutes = RTC.minutes;
  clk.hours = RTC.hours;
  clk.days = RTC.dayofmonth;
  clk.months = RTC.month;
  clk.years = RTC.year;
}
/*void setDS1302Time(uint8_t seconds, uint8_t minutes,uint8_t hours, uint8_t dayofweek,uint8_t dayofmonth, uint8_t month,int year); */  
void loop() {
  if(clk.hours%3==0){
      secondTimeBase=1001;
  }else{
      secondTimeBase=1000;
  }
  secondCounterNew = int(millis()%secondTimeBase);
  if(secondCounterPrev > secondCounterNew){
    clk.advance(); 
  }
  secondCounterPrev = secondCounterNew;
  
  if(modeTimer>4000){
    modeTimer = 0;  
  }else if(modeTimer>3500){
    printDate();  
  }else if(modeTimer>3000){
    if(modeTimer==3001){
      humidity = dht.readHumidity();
      temperature = dht.readTemperature();
    }
    printTemp();
    printHumidity();
  }else{
    printTime();  
  }
  modeTimer++;
}

void screenUpdate(int C1=DIGIT_EMPTY, int C2=DIGIT_EMPTY, int C3=DIGIT_EMPTY, int C4=DIGIT_EMPTY, int C5=DIGIT_EMPTY, int C6=DIGIT_EMPTY, int C7=DIGIT_EMPTY, int C8=DIGIT_EMPTY){
   //DITIT 1
   shiftOut(data, clock, MSBFIRST, digit[C1]);
   digitalWrite(latch, HIGH);
   digitalWrite(digit1,HIGH);
   delay(DELAY_DAY);
   digitalWrite(digit1,LOW);
   digitalWrite(latch, LOW);
   //DIGIT 2
   shiftOut(data, clock, MSBFIRST, digit[C2]);
   digitalWrite(latch, HIGH);
   digitalWrite(digit2,HIGH);
   delay(DELAY_DAY);
   digitalWrite(digit2,LOW);
   digitalWrite(latch, LOW);
   //DIGIT 3
   shiftOut(data, clock, MSBFIRST, digit[C3]);
   digitalWrite(latch, HIGH);
   digitalWrite(digit3,HIGH);
   delay(DELAY_DAY);
   digitalWrite(digit3,LOW);
   digitalWrite(latch, LOW);
   //DIGIT 4
   shiftOut(data, clock, MSBFIRST, digit[C4]);
   digitalWrite(latch, HIGH);
   digitalWrite(digit4,HIGH);
   delay(DELAY_DAY);
   digitalWrite(digit4,LOW);
   digitalWrite(latch, LOW);
   //DIGIT 5
   shiftOut(data, clock, MSBFIRST, digit[C5]);
   digitalWrite(latch, HIGH);
   digitalWrite(digit5,HIGH);
   delay(DELAY_DAY);
   digitalWrite(digit5,LOW);
   digitalWrite(latch, LOW);
   //DIGIT 6
   shiftOut(data, clock, MSBFIRST, digit[C6]);
   digitalWrite(latch, HIGH);
   digitalWrite(digit6,HIGH);
   delay(DELAY_DAY);
   digitalWrite(digit6,LOW);
   digitalWrite(latch, LOW);
   //DIGIT 7
   shiftOut(data, clock, MSBFIRST, digit[C7]);
   digitalWrite(latch, HIGH);
   digitalWrite(digit7,HIGH);
   delay(DELAY_DAY);
   digitalWrite(digit7,LOW);
   digitalWrite(latch, LOW);
   //DIGIT 8
   shiftOut(data, clock, MSBFIRST, digit[C8]);
   digitalWrite(latch, HIGH);
   digitalWrite(digit8,HIGH);
   delay(DELAY_DAY);
   digitalWrite(digit8,LOW);
   digitalWrite(latch, LOW);
}

void printTime(){
  int hours = clk.hours;
  int minutes = clk.minutes;
  int seconds = clk.seconds;  
  screenUpdate(((hours/10)%10),(hours%10),DIGIT_EMPTY,((minutes/10)%10),(minutes%10),DIGIT_EMPTY,((seconds/10)%10),(seconds%10));
}

void printDate(){
  int day = clk.days;
  int month = clk.months;
  int year = clk.years;  
  screenUpdate(((day/10)%10),(day%10),DIGIT_DASH,((month/10)%10),(month%10),DIGIT_DASH,((year/10)%10),(year%10));
}

void printTemp(){
  int temp = round(temperature);
  if(temp>9){
    screenUpdate((temp/10)%10,(temp%10),DIGIT_DEGREE,DIGIT_C);  
  }else if(temp>-1){
    screenUpdate((temp%10),DIGIT_DEGREE,DIGIT_C);
  }else if(temp>-10){
    screenUpdate(DIGIT_DASH,-(temp%10),DIGIT_DEGREE,DIGIT_C);  
  }else{
    screenUpdate(DIGIT_DASH,-(temp/10)%10,-(temp%10),DIGIT_DEGREE,DIGIT_C);  
  }  
}

void printHumidity(){
  int hum = round(humidity);
  if(hum == 100){
    screenUpdate(DIGIT_EMPTY,DIGIT_EMPTY,DIGIT_EMPTY,DIGIT_EMPTY,DIGIT_H,1,0,0);  
  }else if(hum>9){
    screenUpdate(DIGIT_EMPTY,DIGIT_EMPTY,DIGIT_EMPTY,DIGIT_EMPTY,DIGIT_EMPTY,DIGIT_H,(hum/10)%10,hum%10);  
  }  
} 