#ifndef MY_DATE
#define MY_DATE

#include <string>
#include <ctime>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

std::string showDate(time_t d);
std::string showDay(time_t d);

time_t turnDate(char* str);

#endif
