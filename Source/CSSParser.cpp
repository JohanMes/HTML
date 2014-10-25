#include <stdio.h>

#include "CSSParser.h"
#include "utils.h"

CSSParser::CSSParser(const char* filename,StyleVector* styles) {
	this->styles = styles;
	currentstyle = NULL;
	Load(filename);
}

CSSParser::~CSSParser() {
	//WriteToLog(); // last will
}

void CSSParser::Load(const char* filename) {
	FILE* css = fopen(filename,"rb");
	if(css) {
		
		// How big is the file?
		fseek(css,0,SEEK_END);
		unsigned int size = ftell(css);
		fseek(css,0,SEEK_SET);
		
		// Read the whole thing
		char* buffer = new char[size+1];
		fread(buffer,sizeof(char),size,css);
		buffer[size] = 0;
		
		Parse(buffer);
		WriteToLog();

		delete[] buffer;
		
		fclose(css);
	} else {
		MsgBox(filename,"Error opening file!");
	}
}

Style* CSSParser::AddStyle(const char* name) {
	Style* style = new Style(name);
	styles->push_back(style);
	return style;	
}

Style* CSSParser::AddStyle(const char* ptr,int offset) {
	
	// Walk back from { until nonblank
	offset--; // skip {
	while(offset >= 0 and ptr[offset] <= 32) { // skip blank
		offset--;
	}
	int end = offset; // end of identifier
	
	while(offset >= 1 and ptr[offset-1] > 32) { // skip word
		offset--;
	}
	int begin = offset;
	
	char* name = new char[end - begin + 2];
	strncpy(name,&ptr[begin],end - begin + 1);
	name[end - begin + 1] = 0;
	
	Style* style = new Style(name);
	styles->push_back(style);
	return style;
}

Style* CSSParser::GetStyle(const char* name) {
	for(unsigned int i = 0;i < styles->size();i++) {
		if(!strcmp((*styles)[i]->name,name)) {
			return (*styles)[i];
		}
	}
	return NULL;
}

void CSSParser::Parse(const char* text) {
	const char* ptr = text;
	
	// Add styles, separated by {
	while(1) {
		unsigned int styledelim = strcspn(ptr,"{");
		if(styledelim > 0) {
			switch(*(ptr + styledelim)) {
				case '{': {
					ptr += styledelim;
					currentstyle = AddStyle(text,ptr - text); // add to list
					currentstyle->Process(ptr);
					ptr++; // step over delimiter
					break;
				}
				default: {
					PostProcess();
					return; // null terminator
				}
			}
		}
	}
}

void CSSParser::PostProcess() {
	
	// Document defaults
	Style* global = GetStyle("html");
	if(!global) {
		global = AddStyle("html");
	}
	
	global->AddProperty("color","black");
	global->AddProperty("width","100%");
	global->AddProperty("height","100%");
	global->AddProperty("margin-left","5px");
	global->AddProperty("margin-top","5px");
	global->AddProperty("margin-right","5px");	
	global->AddProperty("margin-bottom","5px");
	global->AddProperty("font","Times New Roman");
	global->AddProperty("font-size","12");
	global->AddProperty("background-color","white");
	global->AddProperty("border-color","white");
	global->AddProperty("font-color","black");
}

void CSSParser::WriteToLog() {
	FILE* text = fopen("CSSlog.txt","w");
	if(text) {
		for(unsigned int i = 0;i < styles->size();i++) {
			Style* style = (*styles)[i];
			fprintf(text,"%s {\n",style->name);
			for(unsigned int j = 0;j < style->properties.size();j++) {
				fprintf(text,"    %s:%s;\n",style->properties[j]->name,style->properties[j]->value);
			}
			fprintf(text,"}\n\n");
		}
		fclose(text);
	}
}
