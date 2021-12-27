#ifndef ds3231_H_
#define ds3231_H_

#include "stm32f1xx_hal.h"

/* Registers location */
#define DS3231_CONTROL    0
#define DS3231_SECONDS    1
#define DS3231_MINUTES    2
#define DS3231_HOURS     3
#define DS3231_DAY      4
#define DS3231_DATE      5
#define DS3231_MONTH     6
#define DS3231_YEAR      7

uint8_t BCD2DEC(uint8_t c);
uint8_t DEC2BCD(uint8_t c);
void DS3231_sendData(I2C_HandleTypeDef hi2, uint8_t DEV_ADDR);
void DS3231_setDate(I2C_HandleTypeDef hi2, uint8_t DEV_ADDR);
uint8_t* I2C_ReadRawData(I2C_HandleTypeDef hi2, uint8_t DEV_ADDR);
void I2C_ReadCalendarData(I2C_HandleTypeDef hi2, uint8_t DEV_ADDR);


char* readSeconds(void);
char* readMinutes(void);
char* readHours(void);
char* readDay(void);
char* readDate(void);
char* readMonth(void);
char* readYear(void);


void setSeconds(uint8_t sec);
void setMinutes(uint8_t min);
void setHour(uint8_t hour);
void setDay(uint8_t day);
void setDate(uint8_t date);
void setMonth(uint8_t month);
void setYear(uint8_t year);


/*
uint8_t readIntSeconds();
*/
#endif /* ds3231_H_ */