#ifndef UTILS_H
#define UTILS_H

#include <windows.h>

#include "Page.h"
#include "Drawer.h"

extern HWND hwnd;
extern Page* page; // todo: add drawer too?

void MsgBox(const char* text,const char* caption);
char* StripChar(const char* text,char value);
char* StripQuotes(const char* text);
char* TrimLeft(char* text);
char* TrimRight(char* text);
char* Trim(char* text);
bool SameStr(const char* str1,const char* str2);

#endif
