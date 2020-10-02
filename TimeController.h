#ifndef __TIME_CONTROLLER__
#define __TIME_CONTROLLER__
#include <Arduino.h>
#include "Scheduler.h";
#include "Types.h"
#include "ButtonManager.h"

class TimeController {
private:
	static TimeController * _instance;
	TimeController();
public:
	static TimeController * instance();
	~TimeController();
public:
	void init(Scheduler * scheduler, ButtonManager * buttonMgr);
	DateTime getTime();
	// void printTime();
	void changeMode();
	void changeValue(bool increase);
	int getSetupTimeMode();
	void tick();
private:
	void setupInitialTime();
	int zellersAlgorithm(int day, int month, int year);
	bool parseInitialTime(const char * str);
	bool parseInitialDate(const char * str);
	int dayOfMonth(int month, int year);
	void setVisible(int mode, bool visible);
	void setValue(int mode, int32_t value);
public:
	static void readTime(void * data);
	static void changeModeCallback(void * data);
	static void increaseValueCallback(void * data);
	static void decreaseValueCallback(void * data);
private:
	tmElements_t _tm;
	int _setupTimeMode;
};

#endif
