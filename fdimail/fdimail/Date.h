#ifndef MY_DATE
#define MY_DATE

#include <sstream>
#include <string>
#include <ctime>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

typedef std::time_t Date;

std::string showDate(Date d);
std::string showDay(Date d);

Date turnDate(char* str);

#endif
