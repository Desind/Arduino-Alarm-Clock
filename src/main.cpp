#include <Arduino.h>

#include <DHT.h>
#include <virtuabotixRTC.h>
#include <Clock.h>

#define DHTPIN A0
#define DHTTYPE DHT11
#define BUZZER A3
#define BUTTON_CHAIN A4

#define DIGIT_DASH 20
#define DIGIT_H 21
#define DIGIT_C 22
#define DIGIT_DEGREE 23
#define DIGIT_EMPTY 24

#define MODE_CLOCK 0
#define MODE_CHANGE_TIME_HOURS 11
#define MODE_CHANGE_TIME_MINUTES 12
#define MODE_CHANGE_TIME_SECONDS 13
#define MODE_CHANGE_TIME_DAYS 14
#define MODE_CHANGE_TIME_MONTHS 15
#define MODE_CHANGE_TIME_YEARS 16
#define MODE_CHANGE_ALARM 2
#define MODE_ALARM 3

#define BUT_PLUS 1
#define BUT_MINUS 2
#define BUT_ALARM 3
#define BUT_TIME 4

DHT dht(DHTPIN, DHTTYPE);
virtuabotixRTC RTC(13, A1, A2);
Clock clk;

Clock changedClk;

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

int digitDelay = 1;

float humidity;
float temperature;

int modeTimer = 0;
int secondTimeBase;

int clockMode = 0;

boolean buttonPressed = false;
int buttonNumber = 0;

int changeTimeStatus = 0;

void setup()
{
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
	digitalWrite(digit1, LOW);
	digitalWrite(digit2, LOW);
	digitalWrite(digit3, LOW);
	digitalWrite(digit4, LOW);
	digitalWrite(digit5, LOW);
	digitalWrite(digit6, LOW);
	digitalWrite(digit7, LOW);
	digitalWrite(digit8, LOW);

	humidity = dht.readHumidity();
	temperature = dht.readTemperature();
	RTC.updateTime();
	clk.seconds = RTC.seconds;
	clk.minutes = RTC.minutes;
	clk.hours = RTC.hours;
	clk.days = RTC.dayofmonth;
	clk.months = RTC.month;
	clk.years = RTC.year;

	//tone(BUZZER, 970, 2000);
}
/*void setDS1302Time(uint8_t seconds, uint8_t minutes,uint8_t hours, uint8_t dayofweek,uint8_t dayofmonth, uint8_t month,int year); */

//BUTTON 1 +
//BUTTON 2 -



void loop()
{

	if (clk.hours % 2 == 0)
	{
		secondTimeBase = 1001;
	}
	else
	{
		secondTimeBase = 1000;
	}
	secondCounterNew = int(millis() % secondTimeBase);
	if (secondCounterPrev > secondCounterNew)
	{
		clk.advance();
	}
	secondCounterPrev = secondCounterNew;


	int buttonChain = analogRead(BUTTON_CHAIN);
	if (buttonChain > 1010)
	{
		//BUTTON 1 +
		if (buttonPressed == false)
		{
			switch (clockMode){
				case MODE_CLOCK:{
					if (modeTimer > 3500)
					{
						modeTimer = 0;
						break;
					}else if (modeTimer > 3000)
					{
						modeTimer = 3500;
						break;
					}else if (modeTimer > 0)
					{
						modeTimer = 3000;
						break;
					}
					break;
				}
				case MODE_CHANGE_TIME_HOURS:{
					if (changedClk.hours==23){
						changedClk.hours=0;
						break;
					}
					changedClk.hours++;
					break;
				}
				case MODE_CHANGE_TIME_MINUTES:{
					if (changedClk.minutes == 59){
						changedClk.minutes = 0;
						break;
					}
					changedClk.minutes++;
					break;
				}
				case MODE_CHANGE_TIME_SECONDS:{
					if (changedClk.seconds == 59)
					{
						changedClk.seconds = 0;
						break;
					}
					changedClk.seconds++;
					break;
				}
				case MODE_CHANGE_TIME_YEARS:{
					changedClk.years++;
					break;
				}
				case MODE_CHANGE_TIME_MONTHS:{
					if (changedClk.months == 12)
					{
						changedClk.months = 1;
						break;
					}
					changedClk.months++;
					break;
				}
				case MODE_CHANGE_TIME_DAYS:{
					if(changedClk.days == changedClk.numberOfDays(changedClk.months,changedClk.years)){
						changedClk.days = 1;
						break;
					}
					changedClk.days++;
					break;
				}
			}
		}
		buttonPressed = true;
	}
	else if (buttonChain > 700)
	{
		//BUTTON 2 -
		if(buttonPressed == false){
			switch (clockMode)
			{
				case MODE_CLOCK:
				{
					if (modeTimer > 3500)
					{
						modeTimer = 3001;
						break;
					}
					else if (modeTimer > 3000)
					{
						modeTimer = 0;
						break;
					}
					else if (modeTimer > 0)
					{
						modeTimer = 3501;
						break;
					}
				}
				case MODE_CHANGE_TIME_HOURS:
				{
					if (changedClk.hours == 0)
					{
						changedClk.hours = 23;
						break;
					}
					changedClk.hours--;
					break;
				}
				case MODE_CHANGE_TIME_MINUTES:
				{
					if (changedClk.minutes == 0)
					{
						changedClk.minutes = 59;
						break;
					}
					changedClk.minutes--;
					break;
				}
				case MODE_CHANGE_TIME_SECONDS:
				{
					if (changedClk.seconds == 0)
					{
						changedClk.seconds = 59;
						break;
					}
					changedClk.seconds--;
					break;
				}
				case MODE_CHANGE_TIME_YEARS:
				{
					changedClk.years--;
					break;
				}
				case MODE_CHANGE_TIME_MONTHS:
				{
					if (changedClk.months == 1)
					{
						changedClk.months = 12;
						break;
					}
					changedClk.months--;
					break;
				}
				case MODE_CHANGE_TIME_DAYS:
				{
					if (changedClk.days == 1)
					{
						changedClk.days = changedClk.numberOfDays(changedClk.months, changedClk.years);
						break;
					}
					changedClk.days--;
					break;
				}
			}
		}
		buttonPressed = true;
	}
	else if (buttonChain > 400)
	{
		//BUTTON 3 ALARM
		buttonPressed = true;
	}
	else if (buttonChain > 250)
	{
		//BUTTON 4 TIME
		if(buttonPressed == false){
			switch (clockMode){
				case MODE_CLOCK:{
					changedClk.years = clk.years;
					changedClk.months = clk.months;
					changedClk.days = clk.days;
					changedClk.hours = clk.hours;
					changedClk.minutes = clk.minutes;
					changedClk.seconds = clk.seconds;
					clockMode = MODE_CHANGE_TIME_SECONDS;
					break;
				}
				case MODE_CHANGE_TIME_SECONDS:{
					clockMode = MODE_CHANGE_TIME_MINUTES;
					break;
				}
				case MODE_CHANGE_TIME_MINUTES:{
					clockMode = MODE_CHANGE_TIME_HOURS;
					break;
				}
				case MODE_CHANGE_TIME_HOURS:{
					clockMode = MODE_CHANGE_TIME_YEARS;
					break;
				}
				case MODE_CHANGE_TIME_YEARS:{
					clockMode = MODE_CHANGE_TIME_MONTHS;
					break;
				}
				case MODE_CHANGE_TIME_MONTHS:{
					clockMode = MODE_CHANGE_TIME_DAYS;
					break;
				}
				case MODE_CHANGE_TIME_DAYS:{
					clk.years = changedClk.years;
					clk.months = changedClk.months;
					clk.days = changedClk.days;
					clk.hours = changedClk.hours;
					clk.minutes = changedClk.minutes;
					clk.seconds = changedClk.seconds;
					RTC.setDS1302Time(clk.seconds, clk.minutes, clk.hours, 1, clk.days, clk.months, clk.years);
					clockMode = MODE_CLOCK;
					break;
				}
			}
		}
		buttonPressed = true;
	}
	else
	{
		buttonPressed = false;
	}
	//SHOW TIME + TEMP + HUMIDITY + DATE
	switch (clockMode){
		case MODE_CLOCK:{
			if (modeTimer > 4000)
			{
				modeTimer = 0;
			}
			else if (modeTimer > 3500)
			{
				printDate();
			}
			else if (modeTimer > 3000)
			{
				if (modeTimer == 3001)
				{
					humidity = dht.readHumidity();
					temperature = dht.readTemperature();
				}
				printTemp();
				printHumidity();
			}
			else
			{
				printTime();
			}
			modeTimer++;
			break;
		}
		case MODE_CHANGE_TIME_HOURS:{
			if((secondCounterNew%1000)<500){
				screenUpdate(((changedClk.hours / 10) % 10), (changedClk.hours % 10), DIGIT_EMPTY, ((changedClk.minutes / 10) % 10), (changedClk.minutes % 10), DIGIT_EMPTY, ((changedClk.seconds / 10) % 10), (changedClk.seconds % 10));
			}else{
				screenUpdate(DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_EMPTY, ((changedClk.minutes / 10) % 10), (changedClk.minutes % 10), DIGIT_EMPTY, ((changedClk.seconds / 10) % 10), (changedClk.seconds % 10));
			}
			break;
		}
		case MODE_CHANGE_TIME_MINUTES:{
			if ((secondCounterNew % 1000) < 500)
			{
				screenUpdate(((changedClk.hours / 10) % 10), (changedClk.hours % 10), DIGIT_EMPTY, ((changedClk.minutes / 10) % 10), (changedClk.minutes % 10), DIGIT_EMPTY, ((changedClk.seconds / 10) % 10), (changedClk.seconds % 10));
			}
			else
			{
				screenUpdate(((changedClk.hours / 10) % 10), (changedClk.hours % 10), DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_EMPTY, ((changedClk.seconds / 10) % 10), (changedClk.seconds % 10));
			}
			break;
		}
		case MODE_CHANGE_TIME_SECONDS:{
			if ((secondCounterNew % 1000) < 500)
			{
				screenUpdate(((changedClk.hours / 10) % 10), (changedClk.hours % 10), DIGIT_EMPTY, ((changedClk.minutes / 10) % 10), (changedClk.minutes % 10), DIGIT_EMPTY, ((changedClk.seconds / 10) % 10), (changedClk.seconds % 10));
			}
			else
			{
				screenUpdate(((changedClk.hours / 10) % 10), (changedClk.hours % 10), DIGIT_EMPTY, ((changedClk.minutes / 10) % 10), (changedClk.minutes % 10), DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_EMPTY);
			}
			break;
		}
		case MODE_CHANGE_TIME_DAYS:{
			if ((secondCounterNew % 1000) < 500)
			{
				screenUpdate(((changedClk.days / 10) % 10), (changedClk.days % 10), DIGIT_DASH, ((changedClk.months / 10) % 10), (changedClk.months % 10), DIGIT_DASH, ((changedClk.years / 10) % 10), (changedClk.years % 10));
			}
			else
			{
				screenUpdate(DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_DASH, ((changedClk.months / 10) % 10), (changedClk.months % 10), DIGIT_DASH, ((changedClk.years / 10) % 10), (changedClk.years % 10));
			}
			break;
		}
		case MODE_CHANGE_TIME_MONTHS:{
			if ((secondCounterNew % 1000) < 500)
			{
				screenUpdate(((changedClk.days / 10) % 10), (changedClk.days % 10), DIGIT_DASH, ((changedClk.months / 10) % 10), (changedClk.months % 10), DIGIT_DASH, ((changedClk.years / 10) % 10), (changedClk.years % 10));
			}
			else
			{
				screenUpdate(((changedClk.days / 10) % 10), (changedClk.days % 10), DIGIT_DASH, DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_DASH, ((changedClk.years / 10) % 10), (changedClk.years % 10));
			}
			break;
		}
		case MODE_CHANGE_TIME_YEARS:{
			if ((secondCounterNew % 1000) < 500)
			{
				screenUpdate(((changedClk.days / 10) % 10), (changedClk.days % 10), DIGIT_DASH, ((changedClk.months / 10) % 10), (changedClk.months % 10), DIGIT_DASH, ((changedClk.years / 10) % 10), (changedClk.years % 10));
			}
			else
			{
				screenUpdate(((changedClk.days / 10) % 10), (changedClk.days % 10), DIGIT_DASH, ((changedClk.months / 10) % 10), (changedClk.months % 10), DIGIT_DASH, DIGIT_EMPTY, DIGIT_EMPTY);
			}
			break;
		}
	}
}

void screenUpdate(int C1 = DIGIT_EMPTY, int C2 = DIGIT_EMPTY, int C3 = DIGIT_EMPTY, int C4 = DIGIT_EMPTY, int C5 = DIGIT_EMPTY, int C6 = DIGIT_EMPTY, int C7 = DIGIT_EMPTY, int C8 = DIGIT_EMPTY)
{
	//DITIT 1
	shiftOut(data, clock, MSBFIRST, digit[C1]);
	digitalWrite(latch, HIGH);
	digitalWrite(digit1, HIGH);
	delay(digitDelay);
	digitalWrite(digit1, LOW);
	digitalWrite(latch, LOW);
	//DIGIT 2
	shiftOut(data, clock, MSBFIRST, digit[C2]);
	digitalWrite(latch, HIGH);
	digitalWrite(digit2, HIGH);
	delay(digitDelay);
	digitalWrite(digit2, LOW);
	digitalWrite(latch, LOW);
	//DIGIT 3
	shiftOut(data, clock, MSBFIRST, digit[C3]);
	digitalWrite(latch, HIGH);
	digitalWrite(digit3, HIGH);
	delay(digitDelay);
	digitalWrite(digit3, LOW);
	digitalWrite(latch, LOW);
	//DIGIT 4
	shiftOut(data, clock, MSBFIRST, digit[C4]);
	digitalWrite(latch, HIGH);
	digitalWrite(digit4, HIGH);
	delay(digitDelay);
	digitalWrite(digit4, LOW);
	digitalWrite(latch, LOW);
	//DIGIT 5
	shiftOut(data, clock, MSBFIRST, digit[C5]);
	digitalWrite(latch, HIGH);
	digitalWrite(digit5, HIGH);
	delay(digitDelay);
	digitalWrite(digit5, LOW);
	digitalWrite(latch, LOW);
	//DIGIT 6
	shiftOut(data, clock, MSBFIRST, digit[C6]);
	digitalWrite(latch, HIGH);
	digitalWrite(digit6, HIGH);
	delay(digitDelay);
	digitalWrite(digit6, LOW);
	digitalWrite(latch, LOW);
	//DIGIT 7
	shiftOut(data, clock, MSBFIRST, digit[C7]);
	digitalWrite(latch, HIGH);
	digitalWrite(digit7, HIGH);
	delay(digitDelay);
	digitalWrite(digit7, LOW);
	digitalWrite(latch, LOW);
	//DIGIT 8
	shiftOut(data, clock, MSBFIRST, digit[C8]);
	digitalWrite(latch, HIGH);
	digitalWrite(digit8, HIGH);
	delay(digitDelay);
	digitalWrite(digit8, LOW);
	digitalWrite(latch, LOW);
}

void printTime()
{
	int hours = clk.hours;
	int minutes = clk.minutes;
	int seconds = clk.seconds;
	screenUpdate(((hours / 10) % 10), (hours % 10), DIGIT_EMPTY, ((minutes / 10) % 10), (minutes % 10), DIGIT_EMPTY, ((seconds / 10) % 10), (seconds % 10));
}

void printDate()
{
	int day = clk.days;
	int month = clk.months;
	int year = clk.years;
	screenUpdate(((day / 10) % 10), (day % 10), DIGIT_DASH, ((month / 10) % 10), (month % 10), DIGIT_DASH, ((year / 10) % 10), (year % 10));
}

void printTemp()
{
	int temp = round(temperature);
	if (temp > 9)
	{
		screenUpdate((temp / 10) % 10, (temp % 10), DIGIT_DEGREE, DIGIT_C);
	}
	else if (temp > -1)
	{
		screenUpdate((temp % 10), DIGIT_DEGREE, DIGIT_C);
	}
	else if (temp > -10)
	{
		screenUpdate(DIGIT_DASH, -(temp % 10), DIGIT_DEGREE, DIGIT_C);
	}
	else
	{
		screenUpdate(DIGIT_DASH, -(temp / 10) % 10, -(temp % 10), DIGIT_DEGREE, DIGIT_C);
	}
}

void printHumidity()
{
	int hum = round(humidity);
	if (hum == 100)
	{
		screenUpdate(DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_H, 1, 0, 0);
	}
	else if (hum > 9)
	{
		screenUpdate(DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_EMPTY, DIGIT_H, (hum / 10) % 10, hum % 10);
	}
}