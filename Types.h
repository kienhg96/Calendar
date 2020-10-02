#ifndef __TYPES_H__
#define __TYPES_H__
#include <TimeLib.h>

#define PI 3.1415926f

typedef void (*Callback)(void *);
typedef long int32_t;
typedef float float32_t;

struct Date {
	int32_t day;
	int32_t month;
	int32_t year;
};

struct LunarDate {
	int32_t day;
	int32_t month;
	int32_t year;
	int32_t leap;
};

struct DateTime {
	int32_t day;
	int32_t month;
	int32_t year;
	int32_t wday;
	int32_t hour;
	int32_t minute;
	int32_t second;
};

int32_t INT(float32_t value);

int32_t datecmp(Date d1, Date d2);

#define NO_SETUP 0
#define DAY_MODE 1
#define MONTH_MODE 2
#define YEAR_MODE 3
#define HOUR_MODE 4
#define MIN_MODE 5
#define FINAL_SETUP 6
#define SETUP_INITIAL_TIME 6

#define TEMPERATURE_MODE 7
#define LUNAR_DAY_MODE 8
#define LUNAR_MONTH_MODE 9
#define WEEK_DAY_MODE 10

#define YEAR_LIMIT_MIN 50
#define YEAR_LIMIT_MAX 150

#define TIMEZONE 7

#endif
