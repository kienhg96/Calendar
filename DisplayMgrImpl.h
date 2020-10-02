#ifndef __DISPLAY_MGR_IMPL__
#define __DISPLAY_MGR_IMPL__
#include "DisplayManager.h"

class DisplayMgrImpl : public DisplayManager {
private:
	static DisplayMgrImpl * _instance;
	DisplayMgrImpl();
public:
	~DisplayMgrImpl();
	static DisplayMgrImpl * instance();

	void setValue(int mode, int32_t value);
	void setVisible(int mode, bool visible);
};

#endif
