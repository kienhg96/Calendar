#include "TimeController.h"
#include <DS1307RTC.h>
#include <math.h>

TimeController * TimeController::_instance = 0;

TimeController * TimeController::instance() {
	if (!_instance) {
		_instance = new TimeController();
	}
	return _instance;
}

TimeController::TimeController() {
	_setupTimeMode = NO_SETUP;
}

TimeController::~TimeController() {}

void TimeController::init(Scheduler * scheduler, ButtonManager * buttonMgr) {
	Serial.println("TimeController::init");
	scheduler->schedule(readTime, this, 500);
	buttonMgr->setButtonCallback(1, changeModeCallback, this);
	buttonMgr->setButtonCallback(2, decreaseValueCallback, this);
	buttonMgr->setButtonCallback(3, increaseValueCallback, this);
}

void TimeController::readTime(void * data) {
	TimeController * self = (TimeController *) data;

	if (self->_setupTimeMode != NO_SETUP) return;

	if (!RTC.read(self->_tm)) {
		if (RTC.chipPresent()) {
			self->setupInitialTime();
		} else {
			Serial.println("DS1307 read error! Please check the circuitry.");
		}
	}
}

void TimeController::setupInitialTime() {
	Serial.print("Setup Initital Time...");
	_setupTimeMode = SETUP_INITIAL_TIME;

	bool parse = false;
	bool config = false;

	if (parseInitialDate(__DATE__) && parseInitialTime(__TIME__)) {
		parse = true;
		if (RTC.write(_tm)) {
			config = true;
		}
	}

	if (parse && config) {
		Serial.print("DS1307 configured Time = ");
		Serial.print(__TIME__);
		Serial.print(", Date = ");
		Serial.println(__DATE__);
	} else if (parse) {
		Serial.println("DS1307 Communication Error :-{");
		Serial.println("Please check your circuitry");
	} else {
		Serial.print("Could not parse info from the compiler, Time=");
		Serial.print(__TIME__);
		Serial.print(", Date=");
		Serial.println(__DATE__);
	}
	_setupTimeMode = 0;
}

bool TimeController::parseInitialTime(const char *str) {
	int Hour, Min, Sec;
	if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
	_tm.Hour = Hour;
	_tm.Minute = Min;
	_tm.Second = Sec;
	return true;
}

bool TimeController::parseInitialDate(const char *str) {
	char *monthName[12] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	char Month[12];
	int Day, Year;
	uint8_t monthIndex;

	if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
	for (monthIndex = 0; monthIndex < 12; monthIndex++) {
		if (strcmp(Month, monthName[monthIndex]) == 0) break;
	}
	if (monthIndex >= 12) return false;
	_tm.Day = Day;
	_tm.Month = monthIndex + 1;
	_tm.Year = CalendarYrToTm(Year);
	return true;
}

DateTime TimeController::getTime() {
	int year = tmYearToCalendar(_tm.Year);
	int wday = zellersAlgorithm(_tm.Day, _tm.Month, year);
	DateTime dt = {
		_tm.Day, _tm.Month, year, wday,
		_tm.Hour, _tm.Minute, _tm.Second
	};
	return dt;
}

int TimeController::zellersAlgorithm(int day, int month, int year) {
	int mon;
	if (month > 2) {
		mon = month; //for march to december month code is same as month
	} else {
		mon = (12 + month); //for Jan and Feb, month code will be 13 and 14 year--; //decrease year for month Jan and Feb
	}
	int y = year % 100; //last two digit
	int c = year / 100; //first two digit
	int w = (day + floor((13 * (mon + 1)) / 5) + y + floor(y / 4) + floor(c / 4) + (5 * c));
	w = w % 7;
	if (w < 2) {
		return 7 + w;
	} else {
		return w;
	}
}

// void TimeController::printTime() {
// 	int year = tmYearToCalendar(_tm.Year);
// 	if (!_setupTimeMode) {
// 		// DisplayManager::instance()->setDate(
// 		// 		_tm.Day, _tm.Month, year,
// 		// 		zellersAlgorithm(_tm.Day, _tm.Month, year),
// 		// 		_tm.Hour, _tm.Minute, _tm.Second
// 		// 	);
// 	}
// }

void TimeController::changeMode() {
	_setupTimeMode++;
	if (_setupTimeMode == FINAL_SETUP) {
		if (!RTC.write(_tm)) {
			Serial.println("Error when writing time");
		}
		_setupTimeMode = NO_SETUP;
	}
}

void TimeController::changeValue(bool increase) {
	int additional = increase ? 1 : -1;
	switch (_setupTimeMode) {
		case DAY_MODE:
			_tm.Day = min(dayOfMonth(_tm.Month, _tm.Year), max(1, _tm.Day + additional));
			break;
		case MONTH_MODE:
			_tm.Month = min(12, max(1, _tm.Month + additional));
			_tm.Day = min(dayOfMonth(_tm.Month, _tm.Year), _tm.Day);
			break;
		case YEAR_MODE:
			_tm.Year = min(YEAR_LIMIT_MAX, max(YEAR_LIMIT_MIN, _tm.Year + additional));
			_tm.Day = min(dayOfMonth(_tm.Month, _tm.Year), _tm.Day);
			break;
		case HOUR_MODE:
			_tm.Hour = min(23, max(0, _tm.Hour + additional));
			break;
		case MIN_MODE:
			_tm.Minute = min(59, max(0, _tm.Minute + additional));
			break;
	}
}

int TimeController::dayOfMonth(int month, int year) {
	if (month == 2) {
		if (year % 4 == 0) {
			return 29;
		} else {
			return 28;
		}
	}
	switch (month) {
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		default:
			return 31;
	}
}

int TimeController::getSetupTimeMode() {
	return _setupTimeMode;
}

void TimeController::changeModeCallback(void * data) {
	TimeController * self = (TimeController *) data;
	self->changeMode();
}

void TimeController::increaseValueCallback(void * data) {
	TimeController * self = (TimeController *) data;
	self->changeValue(true);
}

void TimeController::decreaseValueCallback(void * data) {
	TimeController * self = (TimeController *) data;
	self->changeValue(false);
}
