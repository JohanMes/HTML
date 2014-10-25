#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "Tag.h"
#include "Style.h"
#include "Snippet.h"

class Page;

class HTMLParser {
	Tag* currenttag;
	char* buffer; // temp!

	Tag* AddTag(const char* ptr,unsigned int length);
	Snippet* AddSnippet(const char* value,unsigned int index);
	Tag* GetTagByIndex(TagVector* list,unsigned int index);
	void PostProcess();
	void PostProcessTags(); // set head, link, etc
	void PostProcessTag(Tag* tag);
	void PostProcessText();
	void ParseTags();
	void ParseContent();
	public:
		HTMLParser(Page* parent);
		HTMLParser(const char* filename,Page* parent);
		~HTMLParser();
		
		TagVector* tags; // send to Page
		StyleVector* styles; // send to Page
		SnippetVector* text; // send to Page
	
		void Load(const char* filename);
		void Parse(const char* text); // store result in Page or something		
};

#endif
