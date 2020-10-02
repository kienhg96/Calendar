#ifndef __DISPLAY_MANAGER__
#define __DISPLAY_MANAGER__
#include "Scheduler.h"
#include "TimeController.h"
#include "TemperatureReader.h"
#include "LunarConverter.h"

class DisplayManager {
private:
	static DisplayManager * _instance;
	DisplayManager();
public:
	static DisplayManager * instance();
	~DisplayManager();
public:
	void init(Scheduler * scheduler,
			TimeController * timeController,
			TemperatureReader * tempReader,
			LunarConverter * lunarConv);
	static void updateTempDisplay(void * data);
	static void updateTimeDisplay(void * data);
	static void updateBlinking(void * data);
	void setValue(int mode, int32_t value);
	void setVisible(int mode, bool visible);
	void tick();
private:
	TimeController * _timeCtrl;
	TemperatureReader * _tempReader;
	int _blinkingMode;
	bool _visible;
	LunarConverter * _lunarConv;
};

#endif
