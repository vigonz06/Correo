#include "utilsWin.h"

void message(char* word)
{
	MessageBox(NULL, word, "Error", MB_OK | MB_ICONERROR);
}

int getKey()
{
	int key; DWORD cNumRead; INPUT_RECORD irInBuf;
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

	do
	{
		FlushConsoleInputBuffer(hStdIn);
		do
		{
			ReadConsoleInput(hStdIn, &irInBuf, 1, &cNumRead);
		} while (irInBuf.EventType != KEY_EVENT || irInBuf.Event.KeyEvent.bKeyDown);
		if (irInBuf.Event.KeyEvent.uChar.AsciiChar == 0)
		{
			key = irInBuf.Event.KeyEvent.wVirtualKeyCode;
		}
		else key = irInBuf.Event.KeyEvent.uChar.AsciiChar;
	} while (key != ESCAPE && key != LEFT && key != UP
		&& key != RIGHT  && key != DOWN && key != ENTER);

	return key;
}