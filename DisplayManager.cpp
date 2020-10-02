#include "DisplayManager.h"

DisplayManager * DisplayManager::_instance = nullptr;

DisplayManager * DisplayManager::instance() {
	if (!_instance) {
		_instance = new DisplayManager();
	}
	return _instance;
}

DisplayManager::DisplayManager() {
	_blinkingMode = NO_SETUP;
	_visible = true;
}

DisplayManager::~DisplayManager() {}

void DisplayManager::init(
	Scheduler * scheduler,
	TimeController * timeController,
	TemperatureReader * tempReader,
	LunarConverter * lunarConv
) {
	_timeCtrl = timeController;
	_tempReader = tempReader;
	_lunarConv = lunarConv;
	scheduler->schedule(updateTempDisplay, this, 200);
	scheduler->schedule(updateTimeDisplay, this, 500);
	scheduler->schedule(updateBlinking, this, 500);
}

void DisplayManager::updateTempDisplay(void * data) {
	DisplayManager * self = (DisplayManager *) data;
	int temp = self->_tempReader->temperature();
	self->setValue(TEMPERATURE_MODE, temp);
}

void DisplayManager::updateTimeDisplay(void * data) {
	DisplayManager * self = (DisplayManager *) data;
	DateTime dt = self->_timeCtrl->getTime();
	Date date = { dt.day, dt.month, dt.year };
	LunarDate lunarDate = self->_lunarConv->convert(date);

	self->setValue(DAY_MODE, dt.day);
	self->setValue(MONTH_MODE, dt.day);
	self->setValue(YEAR_MODE, dt.day);
	self->setValue(HOUR_MODE, dt.day);
	self->setValue(MIN_MODE, dt.day);
	self->setValue(WEEK_DAY_MODE, dt.day);

	self->setValue(LUNAR_DAY_MODE, lunarDate.day);
	self->setValue(LUNAR_MONTH_MODE, lunarDate.month);
}

void DisplayManager::updateBlinking(void * data) {
	DisplayManager * self = (DisplayManager *) data;
	self->setVisible(self->_blinkingMode, !self->_visible);
}

void DisplayManager::tick() {
	if (_blinkingMode == _timeCtrl->getSetupTimeMode()) return;
	bool visible = _visible;
	setVisible(_blinkingMode, true);
	_blinkingMode = _timeCtrl->getSetupTimeMode();
	setVisible(_blinkingMode, visible); // Restore last state
}

void DisplayManager::setVisible(int mode, bool visible) {
	_visible = visible;
	// TODO: Set Display visible here
	switch (_blinkingMode) {
		case DAY_MODE:
			break;
		case MONTH_MODE:
			break;
		case YEAR_MODE:
			break;
		case HOUR_MODE:
			break;
		case MIN_MODE:
			break;
		default:
		// Blink LED
			break;
	}
}

void DisplayManager::setValue(int mode, int32_t value) {
	switch (_blinkingMode) {
		case DAY_MODE:
			break;
		case MONTH_MODE:
			break;
		case YEAR_MODE:
			break;
		case HOUR_MODE:
			break;
		case MIN_MODE:
			break;
		case TEMPERATURE_MODE:
			break;
		case LUNAR_DAY_MODE:
			break;
		case LUNAR_MONTH_MODE:
			break;
		case WEEK_DAY_MODE:
			break;
	}
}
