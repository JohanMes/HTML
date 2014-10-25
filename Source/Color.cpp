#include "Color.h"
#include "utils.h"

Color::Color(COLORREF value) {
	this->colorref = value; // direct, geen functies ofzo
}

Color::Color(BYTE r,BYTE g,BYTE b) {
	SetValue(r,g,b);
}

Color::Color(const char* value) {
	SetValue(value);
}

Color::~Color() {
	
}

void Color::SetValue(const char* value) {
	if(SameStr(value,"white")) {
		SetValue(255,255,255);
	} else if(SameStr(value,"silver")) {
		SetValue(192,192,192);
	} else if(SameStr(value,"gray")) {
		SetValue(128,128,128);
	} else if(SameStr(value,"black")) {
		SetValue(0,0,0);
	} else if(SameStr(value,"red")) {
		SetValue(255,0,0);
	} else if(SameStr(value,"maroon")) {
		SetValue(128,0,0);
	} else if(SameStr(value,"yellow")) {
		SetValue(255,255,0);
	} else if(SameStr(value,"olive")) {
		SetValue(128,128,0);
	} else if(SameStr(value,"lime")) {
		SetValue(0,255,0);
	} else if(SameStr(value,"green")) {
		SetValue(0,128,0);
	} else if(SameStr(value,"aqua")) {
		SetValue(0,255,255);
	} else if(SameStr(value,"teal")) {
		SetValue(0,128,128);
	} else if(SameStr(value,"blue")) {
		SetValue(0,0,255);
	} else if(SameStr(value,"navy")) {
		SetValue(0,0,128);
	} else if(SameStr(value,"fuchsia")) {
		SetValue(255,0,255);
	} else if(SameStr(value,"purple")) {
		SetValue(128,0,128);
	} else {
		SetValue(0,0,0);
	}
}

void Color::SetValue(BYTE r,BYTE g,BYTE b) {
	this->colorref = RGB(r,g,b);
}
