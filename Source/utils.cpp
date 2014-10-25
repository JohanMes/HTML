#include <windows.h>

#include "utils.h"

HWND hwnd;
Page* page = NULL;

void MsgBox(const char* text,const char* caption) {
	MessageBox(hwnd,text,caption,MB_OK);
}

char* StripChar(const char* text,char value) {
	const char* start = text;
	while(start && *start == value) {
		start++;
	}
	
	const char* end = &text[strlen(text)-1];
	while(end and *end == value) {
		end--;
	}
	
	// Create stripped text
	char* result = new char[end - start + 2];
	strncpy(result,start,end-start + 1);
	result[end-start+1] = 0;
	
	// Assign
	return result;
}

char* StripQuotes(const char* text) {
	return StripChar(text,'"');
}

char* TrimLeft(char* text) {
	while(isspace(*text)) {
		*text = 0; // zero fill
		text++;
	}
	return text;
}

char* TrimRight(char* text) {
	char* end = text + strlen(text) - 1;
	while(isspace(*end)) {
		*end = 0; // zero fill
		end--;
	}
	return text;
}

char* Trim(char* text) {
	return TrimLeft(TrimRight(text));
}

bool SameStr(const char* str1,const char* str2) {
	return !strcmp(str1,str2);
}
