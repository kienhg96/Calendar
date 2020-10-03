#include "DisplayMgrImpl.h"
#include <Arduino.h>

DisplayMgrImpl * DisplayMgrImpl::_instance = nullptr;

DisplayMgrImpl * DisplayMgrImpl::instance() {
	if (!_instance) {
		_instance = new DisplayMgrImpl();
	}
	return _instance;
}

DisplayMgrImpl::DisplayMgrImpl() : DisplayManager() {
	_ledControl = new LedControl(LED_DATA_PIN, LED_CLK_PIN, LED_LOAD_PIN, 2);
	_ledLayout = new LedLayout();
	_cache = new unsigned char[LAYOUT_OUTPUT_SIZE];
	pinMode(LED_BLINING_PIN, OUTPUT);
}

DisplayMgrImpl::~DisplayMgrImpl() {
	delete _ledControl;
	delete _ledLayout;
	delete[] _cache;
}

void DisplayMgrImpl::setVisible(int mode, bool visible) {
	bool flush = true;
	switch (mode) {
		case DAY_MODE:
			_ledLayout->setVisible(0, visible);
			_ledLayout->setVisible(1, visible);
			break;
		case MONTH_MODE:
			_ledLayout->setVisible(2, visible);
			_ledLayout->setVisible(3, visible);
			break;
		case YEAR_MODE:
			_ledLayout->setVisible(4, visible);
			_ledLayout->setVisible(5, visible);
			break;
		case HOUR_MODE:
			_ledLayout->setVisible(6, visible);
			_ledLayout->setVisible(7, visible);
			break;
		case MIN_MODE:
			_ledLayout->setVisible(8, visible);
			_ledLayout->setVisible(9, visible);
			break;
		default:
			flush = false;
			setLedVisible(visible);
	}
	if (flush) {
		flushData();
	}
}

void DisplayMgrImpl::setValue(int mode, int32_t value) {
	bool flush = true;
	switch (mode) {
		case DAY_MODE:
			_ledLayout->setValue(0, value / 10);
			_ledLayout->setValue(1, value % 10);
			break;
		case MONTH_MODE:
			_ledLayout->setValue(2, value / 10);
			_ledLayout->setValue(3, value % 10);
			break;
		case YEAR_MODE:
			_ledLayout->setValue(4, value / 10);
			_ledLayout->setValue(5, value % 10);
			break;
		case HOUR_MODE:
			_ledLayout->setValue(6, value / 10);
			_ledLayout->setValue(7, value % 10);
			break;
		case MIN_MODE:
			_ledLayout->setValue(8, value / 10);
			_ledLayout->setValue(9, value % 10);
			break;
		case LUNAR_DAY_MODE:
			_ledLayout->setValue(10, value / 10);
			_ledLayout->setValue(11, value % 10);
			break;
		case LUNAR_MONTH_MODE:
			_ledLayout->setValue(12, value / 10);
			_ledLayout->setValue(13, value % 10);
			break;
		case WEEK_DAY_MODE:
			_ledLayout->setValue(14, value);
			break;
		case TEMPERATURE_MODE:
			_ledLayout->setValue(15, value / 10);
			_ledLayout->setValue(16, value % 10);
			break;
		default:
			flush = false;
	}
	if (flush) {
		flushData();
	}
}

void DisplayMgrImpl::flushData() {
	unsigned char * data = _ledLayout->output();
	for (int i = 0; i < LAYOUT_OUTPUT_SIZE; i++) {
		if (_cache[i] != data[i]) { // Cache missing, transfer data
			_ledControl->setRow(i / 8, i % 8, data[i]);
			_cache[i] = data[i]; // Add to cache
		}
	}
}

void DisplayMgrImpl::setLedVisible(bool visible) {
	digitalWrite(LED_BLINING_PIN, visible ? HIGH : LOW);
}
