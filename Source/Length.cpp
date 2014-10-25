#include <stdio.h>
#include <string.h>

#include "Length.h"
#include "Utils.h"

Length::Length(HDC hdc,int parentlength,const char* value) {
	SetDC(hdc); // needed for pt calculation
	SetValue(parentlength,value);
}

Length::~Length() {
}

void Length::SetValue(int parentlength,const char* value) {
	float tmp = 0.0f;
	
	// Pixels
	if(strstr(value,"px") and sscanf(value,"%dpx",&this->value) == 1) { // try pixels
		return;
		
	// Percentage
	} else if(sscanf(value,"%f%%",&tmp) == 1) { // try percents
		this->value = tmp * 0.01f * parentlength;
		return;
		
	// Points
	} else if(strstr(value,"pt") and sscanf(value,"%dpt",&this->value) == 1) { // try pixels
		this->value = -MulDiv(this->value, GetDeviceCaps(hdc, LOGPIXELSY), 72); // 72 ppt
		return;
		
	// Some value...
	} else if(sscanf(value,"%d",&this->value) == 1) {
		return;
		
	// Words
	} else if(SameStr(value,"thin")) {
		this->value = 3;
	} else if(SameStr(value,"medium")) {
		this->value = 5;
	} else if(SameStr(value,"thick")) {
		this->value = 10;
	}
}

void Length::SetDC(HDC hdc) {
	this->hdc = hdc;
}
