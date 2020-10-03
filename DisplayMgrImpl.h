#ifndef __DISPLAY_MGR_IMPL__
#define __DISPLAY_MGR_IMPL__
#include "DisplayManager.h"
#include "LedLayout.h"
#include <LedControl.h>

#define LED_DATA_PIN 12
#define LED_CLK_PIN 11
#define LED_LOAD_PIN 10
#define LED_BLINING_PIN 1

class DisplayMgrImpl : public DisplayManager {
private:
	static DisplayMgrImpl * _instance;
	DisplayMgrImpl();
public:
	~DisplayMgrImpl();
	static DisplayMgrImpl * instance();

	void setValue(int mode, int32_t value);
	void setVisible(int mode, bool visible);
private:
	void flushData();
	void setLedVisible(bool visible);
private:
	LedControl * _ledControl;
	LedLayout * _ledLayout;
	unsigned char * _cache;
};

#endif
