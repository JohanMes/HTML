#ifndef PEN_H
#define PEN_H

#include <windows.h>
#include "Color.h"

class Pen {
	HDC hdc;
	int updatecount;

	void Update();
	public:
		Pen();
		Pen(int style,int width,COLORREF color);
		
		HPEN handle;
		int style;
		int width;
		COLORREF color;
		
		void SetDC(HDC hdc); // essentieel
		void SetStyle(int value);
		void SetWidth(int value);
		void SetColor(COLORREF value);
		
		void BeginUpdate();
		void EndUpdate();
};

#endif
