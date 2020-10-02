#ifndef __LUNAR_CONVERTER__
#define __LUNAR_CONVERTER__
#include "Types.h"

class LunarConverter {
private:
	static LunarConverter * _instance;
	LunarConverter();
public:
	static LunarConverter * instance();
	~LunarConverter();
public:
	LunarDate convert(Date date);
private:
	Date _date;
	LunarDate _lunarDate;

// Algorithm methods
private:
	static int32_t jdFromDate(int32_t dd, int32_t mm, int32_t yy);
	static Date jdToDate(int32_t jd);
	static float32_t NewMoon(int32_t k);
	static float32_t SunLongitude(float32_t jdn);
	static int32_t getSunLongitude(int32_t dayNumber, int32_t timeZone);
	static int32_t getNewMoonDay(int32_t k, int32_t timeZone);
	static int32_t getLunarMonth11(int32_t yy, int32_t timeZone);
	static int32_t getLeapMonthOffset(int32_t a11, int32_t timeZone);
	static LunarDate convertSolar2Lunar(int32_t dd, int32_t mm, int32_t yy, int32_t timeZone);
	static Date convertLunar2Solar(int32_t lunarDay, int32_t lunarMonth, int32_t lunarYear, int32_t lunarLeap, int32_t timeZone);
};

#endif
