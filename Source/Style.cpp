#include <string.h>

#include "Style.h"
#include "utils.h"

Style::Style() {
	name = NULL;
}

Style::Style(const char* name) {
	this->name = new char[strlen(name) + 1];
	strcpy(this->name,name);
}

Style::~Style() {
	delete[] name;
}

void Style::Process(const char* styletext) {
	
	const char* ptr = styletext + 1; // skip over {

	// add properties, separated by ;
	while(1) {
		unsigned int propdelim = strcspn(ptr,";{");
		if(propdelim > 0) {
			switch(*(ptr + propdelim)) {
				case ';': {
					AddProperty(ptr,propdelim);
					ptr += propdelim + 1; // step over delimiter
					break;
				}
				default: {
					return;
				}
			}
		}
	}
}

Property* Style::GetProperty(const char* name) {
	for(unsigned int i = 0;i < properties.size();i++) {
		if(!strcmp(properties[i]->name,name)) {
			return properties[i];
		}
	}
	return NULL;
}

Property* Style::GetProperty(PropertyType type) {
	for(unsigned int i = 0;i < properties.size();i++) {
		if(properties[i]->type == type) {
			return properties[i];
		}
	}
	return NULL;
}

void Style::AddProperty(const char* ptr,unsigned int length) { // ptr contains starting delim 

	// cut it up by :
	unsigned int namelen = strcspn(ptr,":");
		
	// Copy part before :
	char* name = new char[namelen];
	strncpy(name,ptr+1,namelen-1);
	name[namelen-1] = 0;
	name = Trim(name);
	
	// Copy part after :
	char* value = new char[length - namelen];
	strncpy(value,ptr + namelen + 1,length - namelen - 1);
	value[length - namelen - 1] = 0;
	value = Trim(value);
	
	AddProperty(name,value);
}

void Style::AddProperty(Property* property) {
	AddProperty(property->name,property->value);
}

void Style::AddProperty(const char* name,const char* value) {
	
	// Always add unique
	for(unsigned int i = 0;i < properties.size();i++) {
		if(!strcmp(properties[i]->name,name)) { // found same, replace
			delete properties[i]; // gooi ouwe zooi weg
			properties[i] = new Property(name,value);
			return;
		}
	}
	
	// Of gooi het er achteraan op
	properties.push_back(new Property(name,value));
}
