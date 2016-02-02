#ifndef UTILSWIN
#define UTILSWIN

#include <Windows.h>

enum keys { UP = VK_UP, RIGHT = VK_RIGHT, LEFT = VK_LEFT, DOWN = VK_DOWN, ESCAPE = VK_ESCAPE, ENTER = VK_RETURN };

int getKey();

void message(char* word);

#endif
