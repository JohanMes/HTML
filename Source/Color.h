#ifndef COLOR_H
#define COLOR_H

#include <windows.h>

class Color {
	public:
		Color(COLORREF value);
		Color(BYTE r,BYTE g,BYTE b);
		Color(const char* value);
		~Color();
		
		COLORREF colorref;
		
		void SetValue(const char* text);
		void SetValue(BYTE r,BYTE g,BYTE b);
};

#endif
