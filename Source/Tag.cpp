#include <string.h>

#include "Tag.h"
#include "utils.h"

Tag::Tag(const char* tagtext) {
	SetType(tagtext); // don't create temp copy
	SetProperties(tagtext);
	
	// Will be filled by parser
	parent = NULL;
}

Tag::Tag(const char* ptr,unsigned int length) {
	
	// create temp copy
	char* tagtext = new char[length + 1];
	strncpy(tagtext,ptr,length);
	tagtext[length] = 0;
	
	// process tmp
	SetType(tagtext);
	SetProperties(tagtext);
	
	delete[] tagtext; // remove tmp
	
	// Will be filled by parser
	parent = NULL;
}

Tag::~Tag() {
	delete[] name;

	// delete properties
	for(unsigned int i = 0;i < properties.size();i++) {
		delete properties[i];
	}
}

void Tag::SetType(const char* tagtext) {
	
	// copy parst before first space to name, else all of it
	unsigned int length = strcspn(tagtext," ");
	if(length > 0) {
		name = new char[length + 1];
		strncpy(name,tagtext,length);
		name[length] = 0;
	} else {
		name = new char[strlen(tagtext)+1];
		strcpy(name,tagtext);
	}

	if(!strcmp(name,"html")) {
		type = ttHtml;
	} else if(!strcmp(name,"body")) {
		type = ttBody;
	} else if(!strcmp(name,"b")) {
		type = ttB;
	} else if(!strcmp(name,"head")) {
		type = ttHead;
	} else if(!strcmp(name,"title")) {
		type = ttTitle;
	} else if(!strcmp(name,"div")) {
		type = ttDiv;
	} else if(!strcmp(name,"link")) {
		type = ttLink;
	} else {
		type = ttUnknown;
	}
}

void Tag::SetProperties(const char* tagtext) {
	
	const char* ptr = tagtext;
	
	// Skip name
	unsigned int length = strcspn(tagtext," ");
	if(length != strlen(tagtext)) { // name only
		ptr += length + 1; // step past space too
	} else {
		return; // niks te vinden
	}
	
	// add properties, separated by space
	while(1) {
		unsigned int propdelim = strcspn(ptr," >");
		if(propdelim > 0) {
			AddProperty(ptr,propdelim);
			if(*(ptr + propdelim) == 0) { // delimiter
				break;
			}
		}
		ptr += (propdelim + 1);
	}
}

void Tag::AddChild(Tag* child) {
	children.push_back(child);
	child->parent = this;
}

void Tag::AddProperty(const char* ptr,unsigned int length) {
	
	// cut it up by =
	unsigned int namelen = strcspn(ptr,"=");
	if(namelen != strlen(ptr)) {
		
		// Copy until =
		char* name = new char[namelen+1];
		strncpy(name,ptr,namelen);
		name[namelen] = 0;
		
		ptr += namelen + 1;
		
		// Copy all past =
		char* quotedvalue = new char[length - namelen];
		strncpy(quotedvalue,ptr,length - namelen - 1);
		quotedvalue[length - namelen -1] = 0;
		char* value = StripQuotes(quotedvalue);

		AddProperty(new Property(name,value));
	} else {
		// ???
	}
}

void Tag::AddProperty(Property* property) {
	properties.push_back(property);
}

Property* Tag::GetProperty(const char* name) {
	
	// Try our own list
	for(unsigned int i = 0;i < properties.size();i++) {
		if(!strcmp(properties[i]->name,name)) {
			return properties[i];
		}
	}
	
	// try parent
	if(parent) {
		return parent->GetProperty(name);
	} else {
		return NULL;
	}
}
