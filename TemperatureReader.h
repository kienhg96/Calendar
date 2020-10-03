#ifndef __TEMP_READER__
#define __TEMP_READER__
#include <Arduino.h>
#include "Scheduler.h"
#include "Pins.h"

class TemperatureReader {
private:
	static TemperatureReader * _instance;
	TemperatureReader();
public:
	static TemperatureReader * instance();
	~TemperatureReader();
public:
	void init(Scheduler * scheduler);
	int temperature();
public:	
	static void update(void * data);
private:
	int _temperature;
	int _counter;
	int _sum;
};

#endif
