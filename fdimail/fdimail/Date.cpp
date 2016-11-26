#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <algorithm>
#include <sstream>
#include "Date.h"

std::string showDate(time_t d)
{
	std::ostringstream resultado;
	tm ltm;
	localtime_s(&ltm, &d);
	resultado << ltm.tm_mday << '/' << 1 + ltm.tm_mon << '/' << 1900 + ltm.tm_year
		<< " (" << ltm.tm_hour << ':' << ltm.tm_min << ':' << ltm.tm_sec << ')';

	return resultado.str();
}

std::string showDay(time_t d)
{
	std::ostringstream resultado;
	tm ltm;
	localtime_s(&ltm, &d);
	resultado << ltm.tm_mday << '/' << 1 + ltm.tm_mon << '/' << 1900 + ltm.tm_year;

	return resultado.str();
}

time_t turnDate(char* str)
{
	tm* timeinfo;
	time_t rawtime;
	int year, month, day;

	std::replace(str, str + strlen(str), '/', ' ');
	std::istringstream(str) >> year >> month >> day;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon = month - 1;
	timeinfo->tm_mday = day;
	timeinfo->tm_hour = 0;
	timeinfo->tm_min = 0;
	timeinfo->tm_sec = 0;

	return mktime(timeinfo);
}