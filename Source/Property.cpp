#include <string.h>

#include "Property.h"

Property::Property(const char* name,const char* value) {
	this->name = new char[strlen(name)+1];
	strcpy(this->name,name);
	
	this->value = new char[strlen(value)+1];
	strcpy(this->value,value);
	
	SetType();
}

Property::~Property() {
	delete[] name;
	delete[] value;
}

void Property::SetType() {
	if(!strcmp(name,"background-color")) {
		type = ptBackgroundColor;
	} else if(!strcmp(name,"color")) {
		type = ptColor;
	} else if(!strcmp(name,"border-color")) {
		type = ptBorderColor;
	} else if(!strcmp(name,"height")) {
		type = ptHeight;
	} else if(!strcmp(name,"width")) {
		type = ptWidth;
	} else if(!strcmp(name,"margin-left")) {
		type = ptMarginLeft;
	} else if(!strcmp(name,"margin-top")) {
		type = ptMarginTop;
	} else if(!strcmp(name,"margin-right")) {
		type = ptMarginRight;
	} else if(!strcmp(name,"margin-bottom")) {
		type = ptMarginBottom;
	} else if(!strcmp(name,"border-width")) {
		type = ptBorderWidth;
	} else if(!strcmp(name,"font")) {
		type = ptFont;
	} else if(!strcmp(name,"font-size")) {
		type = ptFontSize;
	} else {
		type = ptUnknown;
	}
}
