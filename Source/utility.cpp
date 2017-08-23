#include <Windows.h>

#include "utility.h"

float utility::getTimestamp() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	return static_cast<float>(st.wMilliseconds);
}
