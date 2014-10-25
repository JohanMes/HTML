#ifndef RECT_H
#define RECT_H

#include <windows.h>

class Rect : public RECT {
	void Update();
	public:
		Rect();
		Rect(int left,int top,int right,int bottom);
		Rect(RECT rect);
		~Rect();
		
		int width;
		int height;
		
		void SetLeft(int value);
		void SetTop(int value);
		void SetRight(int value);
		void SetBottom(int value);
		void MoveX(int value);
		void MoveY(int value);
		void GetClientRectangle(HWND hwnd);
		void Inflate(int dx,int dy);
};

#endif
