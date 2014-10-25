#ifndef CSSPARSER_H
#define CSSPARSER_H

#include "Style.h"

class CSSParser {
	Style* currentstyle;
	public:
		CSSParser(const char* filename,StyleVector* styles);
		~CSSParser();
		
		StyleVector* styles;
		
		void Load(const char* filename);
		void Parse(const char* text);
		Style* AddStyle(const char* name);
		Style* AddStyle(const char* ptr,int offset); // int used for underflow check
		Style* GetStyle(const char* name);
		void PostProcess();
		void WriteToLog();
};

#endif
