#ifndef __DISPLAY_MGR_IMPL__
#define __DISPLAY_MGR_IMPL__
#include "DisplayManager.h"
#include "LedLayout.h"
#include <LedControl.h>
#include "Pins.h"

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
