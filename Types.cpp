#include <math.h>
#include "Types.h"

int32_t INT(float32_t value) {
	return (int32_t)floor(value);
}

int32_t datecmp(Date d1, Date d2) {
	return d1.day == d2.day && d1.month == d2.month && d1.year == d2.year;
}
