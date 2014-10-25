#ifndef LENGTH_H
#define LENGTH_H

#include <windows.h>

class Length {
	HDC hdc;
	public:
		Length(HDC hdc,int parentlength,const char* value);
		~Length();
		
		void SetValue(int parentlength,const char* value);
		void SetDC(HDC hdc);
		int value;
};

#endif
