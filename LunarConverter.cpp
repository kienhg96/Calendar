#include "LunarConverter.h"
#include <math.h>

LunarConverter * LunarConverter::_instance = nullptr;

LunarConverter * LunarConverter::instance() {
	if (!_instance) {
		_instance = new LunarConverter();
	}
	return _instance;
}

LunarConverter::LunarConverter() {
	_date = { 0, 0, 0 };
	_lunarDate = { 0 , 0, 0 };
}

LunarConverter::~LunarConverter() {}

LunarDate LunarConverter::convert(Date date) {
	if (!datecmp(date, _date)) {
		_date = date;
		_lunarDate = convertSolar2Lunar(date.day, date.month, date.year, TIMEZONE);
	}
	return _lunarDate;
}

// Algorithm methods
int32_t LunarConverter::jdFromDate(int32_t dd, int32_t mm, int32_t yy) {
	int32_t a, y, m, jd;
	a = (14 - mm) / 12;
	y = yy + 4800 - a;
	m = mm + 12 * a - 3;
	jd = dd + ((153 * m + 2) / 5) + 365 * y + (y / 4) - (y / 100) + (y / 400) - 32045;
	if (jd < 2299161) {
		jd = dd + ((153 * m + 2) / 5) + 365 * y + (y / 4) - 32083;
	}
	return jd;
}

Date LunarConverter::jdToDate(int32_t jd) {
	int32_t a, b, c, d, e, m, day, month, year;
	if (jd > 2299160) { // After 5/10/1582, Gregorian calendar
		a = jd + 32044;
		b = ((4 * a + 3) / 146097);
		c = a - ((b * 146097) / 4);
	} else {
		b = 0;
		c = jd + 32082;
	}
	d = ((4 * c + 3) / 1461);
	e = c - ((1461 * d) / 4);
	m = ((5 * e + 2) / 153);
	day = e - ((153 * m + 2) / 5) + 1;
	month = m + 3 - 12 * (m / 10);
	year = b * 100 + d - 4800 + (m / 10);
	Date result = { day, month, year };
	return result;
}

float32_t LunarConverter::NewMoon(int32_t k) {
	float32_t T, T2, T3, dr, Jd1, M, Mpr, F, C1, deltat, JdNew;
	T = k / 1236.85f; // Time in Julian centuries from 1900 January 0.5
	T2 = T * T;
	T3 = T2 * T;
	dr = PI / 180.0f;
	Jd1 = 2415020.75933f + 29.53058868f * k + 0.0001178f * T2 - 0.000000155f * T3;
	Jd1 = Jd1 + 0.00033f * sin((166.56f + 132.87f * T - 0.009173f * T2) * dr); // Mean new moon
	M = 359.2242f + 29.10535608f * k - 0.0000333f * T2 - 0.00000347f * T3; // Sun's mean anomaly
	Mpr = 306.0253f + 385.81691806f * k + 0.0107306f * T2 + 0.00001236f * T3; // Moon's mean anomaly
	F = 21.2964f + 390.67050646f * k - 0.0016528f * T2 - 0.00000239f * T3; // Moon's argument of latitude
	C1 = (0.1734f - 0.000393f * T) * sin(M * dr) + 0.0021f * sin(2 * dr * M);
	C1 = C1 - 0.4068f * sin(Mpr * dr) + 0.0161f * sin(dr * 2 * Mpr);
	C1 = C1 - 0.0004f * sin(dr * 3 * Mpr);
	C1 = C1 + 0.0104f * sin(dr * 2 * F) - 0.0051f * sin(dr *(M + Mpr));
	C1 = C1 - 0.0074f * sin(dr * (M - Mpr)) + 0.0004f * sin(dr *(2 * F + M));
	C1 = C1 - 0.0004f * sin(dr * (2 * F - M)) - 0.0006f * sin(dr *(2 * F + Mpr));
	C1 = C1 + 0.0010f * sin(dr * (2 * F - Mpr)) + 0.0005f * sin(dr *(2 * Mpr + M));
	if (T < -11) {
		deltat = 0.001f + 0.000839f * T + 0.0002261f * T2 - 0.00000845f * T3 - 0.000000081f * T * T3;
	} else {
		deltat = -0.000278f + 0.000265f * T + 0.000262f * T2;
	};
	JdNew = Jd1 + C1 - deltat;
	return JdNew;
}

float32_t LunarConverter::SunLongitude(float32_t jdn) {
	float32_t T, T2, dr, M, L0, DL, L;
	T = (jdn - 2451545.0f) / 36525; // Time in Julian centuries from 2000-01-01 12:00:00 GMT
	T2 = T * T;
	dr = PI / 180.0f; // degree to radian
	M = 357.52910f + 35999.05030f * T - 0.0001559f * T2 - 0.00000048f * T * T2; // mean anomaly, degree
	L0 = 280.46645f + 36000.76983f * T + 0.0003032f * T2; // mean longitude, degree
	DL = (1.914600f - 0.004817f * T - 0.000014f * T2) * sin(dr * M);
	DL = DL + (0.019993f - 0.000101f * T) * sin(dr * 2 * M) + 0.000290f * sin(dr * 3 * M);
	L = L0 + DL; // true longitude, degree
	L = L*dr;
	L = L - PI * 2 * (floor(L / (PI * 2))); // Normalize to (0, 2*PI)
	return L;
}

int32_t LunarConverter::getSunLongitude(int32_t dayNumber, int32_t timeZone) {
	return INT(SunLongitude(dayNumber - 0.5f - timeZone / 24.0f) / PI * 6);
}

int32_t LunarConverter::getNewMoonDay(int32_t k, int32_t timeZone) {
	return INT(NewMoon(k) + 0.5f + timeZone / 24.0f);
}

int32_t LunarConverter::getLunarMonth11(int32_t yy, int32_t timeZone) {
	int32_t k, off, nm, sunLong;
	off = jdFromDate(31, 12, yy) - 2415021;
	k = INT(off / 29.530588853f);
	nm = getNewMoonDay(k, timeZone);
	sunLong = getSunLongitude(nm, timeZone); // sun longitude at local midnight
	if (sunLong >= 9) {
		nm = getNewMoonDay(k - 1, timeZone);
	}
	return nm;
}

/* Find the index of the leap month after the month starting on the day a11. */
int32_t LunarConverter::getLeapMonthOffset(int32_t a11, int32_t timeZone) {
	int32_t k, last, arc, i;
	k = INT((a11 - 2415021.076998695f) / 29.530588853f + 0.5f);
	last = 0;
	i = 1; // We start with the month following lunar month 11
	arc = getSunLongitude(getNewMoonDay(k + i, timeZone), timeZone);
	do {
		last = arc;
		i++;
		arc = getSunLongitude(getNewMoonDay(k + i, timeZone), timeZone);
	} while (arc != last && i < 14);
	return i - 1;
}

/* Comvert solar date dd/mm/yyyy to the corresponding lunar date */
LunarDate LunarConverter::convertSolar2Lunar(int32_t dd, int32_t mm, int32_t yy, int32_t timeZone) {
	int32_t k, dayNumber, monthStart, a11, b11, lunarDay, lunarMonth, lunarYear, lunarLeap;
	dayNumber = jdFromDate(dd, mm, yy);
	k = INT((dayNumber - 2415021.076998695f) / 29.530588853f);
	monthStart = getNewMoonDay(k + 1, timeZone);
	if (monthStart > dayNumber) {
		monthStart = getNewMoonDay(k, timeZone);
	}
	//alert(dayNumber+" -> "+monthStart);
	a11 = getLunarMonth11(yy, timeZone);
	b11 = a11;
	if (a11 >= monthStart) {
		lunarYear = yy;
		a11 = getLunarMonth11(yy - 1, timeZone);
	} else {
		lunarYear = yy+1;
		b11 = getLunarMonth11(yy + 1, timeZone);
	}
	lunarDay = dayNumber-monthStart+1;
	int32_t diff = (monthStart - a11) / 29;
	lunarLeap = 0;
	lunarMonth = diff + 11;
	if (b11 - a11 > 365) {
		int32_t leapMonthDiff = getLeapMonthOffset(a11, timeZone);
		if (diff >= leapMonthDiff) {
			lunarMonth = diff + 10;
			if (diff == leapMonthDiff) {
				lunarLeap = 1;
			}
		}
	}
	if (lunarMonth > 12) {
		lunarMonth = lunarMonth - 12;
	}
	if (lunarMonth >= 11 && diff < 4) {
		lunarYear -= 1;
	}
	LunarDate result = { lunarDay, lunarMonth, lunarYear, lunarLeap };
	return result;
}

/* Convert a lunar date to the corresponding solar date */
Date LunarConverter::convertLunar2Solar(int32_t lunarDay, int32_t lunarMonth, int32_t lunarYear, int32_t lunarLeap, int32_t timeZone) {
	int32_t k, a11, b11, off, leapOff, leapMonth, monthStart;
	if (lunarMonth < 11) {
		a11 = getLunarMonth11(lunarYear - 1, timeZone);
		b11 = getLunarMonth11(lunarYear, timeZone);
	} else {
		a11 = getLunarMonth11(lunarYear, timeZone);
		b11 = getLunarMonth11(lunarYear + 1, timeZone);
	}
	k = INT(0.5f + (a11 - 2415021.076998695f) / 29.530588853f);
	off = lunarMonth - 11;
	if (off < 0) {
		off += 12;
	}
	if (b11 - a11 > 365) {
		leapOff = getLeapMonthOffset(a11, timeZone);
		leapMonth = leapOff - 2;
		if (leapMonth < 0) {
			leapMonth += 12;
		}
		if (lunarLeap != 0 && lunarMonth != leapMonth) {
			Date result = { 0, 0, 0 };
			return result;
		} else if (lunarLeap != 0 || off >= leapOff) {
			off += 1;
		}
	}
	monthStart = getNewMoonDay(k + off, timeZone);
	return jdToDate(monthStart + lunarDay - 1);
}
