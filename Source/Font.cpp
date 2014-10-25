#include "Font.h"
#include "Utils.h"

Font::Font() {
	updatecount = 0;
	typeface = NULL;
}

Font::Font(HDC hdc,int size,const char* typeface) {
	
	updatecount = 0;
	BeginUpdate();
	
	// Use this for pt calculation!
	this->hdc = hdc;
	SetSize(size);
	SetTypeFace(typeface);
	
	EndUpdate();
}

Font::~Font() {
	delete[] typeface;
}

void Font::SetDC(HDC hdc) {
	this->hdc = hdc;
	SelectObject(hdc,handle);
}

void Font::SetSize(int value) {
	
	// Convert to pt
	int sizept = -MulDiv(value, GetDeviceCaps(hdc, LOGPIXELSY), 72); // 72 ppt
	
	if(sizept != this->sizept) {
		this->sizept = sizept;
		Update();
	}
}

void Font::SetTypeFace(const char* value) {
	if(!this->typeface or not SameStr(this->typeface,value)) {
		this->typeface = new char[strlen(value) + 1];
		strcpy(this->typeface,value);
		Update();
	}
}

void Font::SetColor(COLORREF color) {
	SetTextColor(hdc,color);
}

void Font::Update() {
	if(updatecount == 0) {
		DeleteObject(handle);
		handle = CreateFont(sizept,0,0,0,0,0,0,0,0,0,0,0,0,typeface);
		SelectObject(hdc,handle);
	}
}

void Font::BeginUpdate() {
	updatecount++;
}

void Font::EndUpdate() {
	updatecount--;
	if(updatecount == 0) {
		Update();
	}
}
