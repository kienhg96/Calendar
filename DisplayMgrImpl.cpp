#include "DisplayMgrImpl.h"

DisplayMgrImpl * DisplayMgrImpl::_instance = nullptr;

DisplayMgrImpl * DisplayMgrImpl::instance() {
	if (!_instance) {
		_instance = new DisplayMgrImpl();
	}
	return _instance;
}

DisplayMgrImpl::DisplayMgrImpl() : DisplayManager() {}

DisplayMgrImpl::~DisplayMgrImpl() {}


void DisplayMgrImpl::setVisible(int mode, bool visible) {
	// TODO: Set Display visible here
	switch (mode) {
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

void DisplayMgrImpl::setValue(int mode, int32_t value) {
	switch (mode) {
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
