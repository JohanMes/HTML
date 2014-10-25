#ifndef FONT_H
#define FONT_H

#include <windows.h>

class Font {
	HDC hdc;
	int updatecount;

	void Update();
	public:
		Font();
		Font(HDC hdc,int size,const char* typeface);
		~Font();
		
		HFONT handle;
		
		char* typeface;
		int sizept; // size before it went into MulDiv
		
		void SetDC(HDC hdc); // essentieel
		void SetSize(int value);
		void SetTypeFace(const char* value);
		void SetColor(COLORREF color);
		
		void BeginUpdate();
		void EndUpdate();
};

#endif
