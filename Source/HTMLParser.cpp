#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "HTMLParser.h"
#include "CSSParser.h"

HTMLParser::HTMLParser(Page* parent) {
	this->tags = &parent->tags; // store here
	this->styles = &parent->styles; // ...
	this->text = &parent->text;
	currenttag = NULL;
}

HTMLParser::HTMLParser(const char* file,Page* parent) {
	this->tags = &parent->tags; // store here
	this->styles = &parent->styles; // ...
	this->text = &parent->text;
	currenttag = NULL;
	Load(file);
}

HTMLParser::~HTMLParser() {
}

Tag* HTMLParser::AddTag(const char* ptr,unsigned int length) {
	Tag* tag = new Tag(ptr,length);
	
	// Add to parent list if possible
	if(currenttag) {
		currenttag->AddChild(tag);
	} else {
		tags->push_back(tag);
	}
	
	return tag;
}

Snippet* HTMLParser::AddSnippet(const char* value,unsigned int index) {
	
	// Find out which tag we're in
	Tag* parent = GetTagByIndex(tags,index);
	
	Snippet* snippet = new Snippet(value,parent,index);
	text->push_back(snippet);
	return snippet;
}

void HTMLParser::Parse(const char* text) {
	const char* ptr = text;
	while(1) {
		unsigned int tagdelim = strcspn(ptr,"<");
		
		// Advance to delimiter, *ptr is now the delimiter
		ptr += tagdelim;
		switch(*ptr) {
			case '<': {
				unsigned int taglength = strcspn(ptr,">");
				
				if(*(ptr+1) == '/' and currenttag) {
				
					// closing tag, set boundaries of opening tag
					currenttag->endtagbegin = ptr - text; // <
					currenttag->endtagend = ptr - text + taglength; // >
					currenttag = currenttag->parent;
				} else {
				
					// Add new tag to the big list
					Tag* newtag = AddTag(ptr+1,taglength-1);
					newtag->starttagbegin = ptr - text; // <
					newtag->starttagend = ptr - text + taglength; // >
					
					// Don't always update parent
					switch(newtag->type) {
						case ttLink: { // ignore
							break;
						}
						default: {
							currenttag = newtag;
						}
					}
				}
				
				// Step onto >
				ptr += taglength;			
				break;
			}
			default: {
				PostProcess();
				return; // NULl terminator found
			}
		}
		ptr++;
	}
}

void HTMLParser::PostProcessTag(Tag* tag) {
	switch(tag->type) {
		case ttLink: { // open css
			Property* href = tag->GetProperty("href");
			Property* rel = tag->GetProperty("rel");
			Property* type = tag->GetProperty("type");
			if(href and !strcmp(rel->value,"stylesheet") and !strcmp(type->value,"text/css")) {	
				CSSParser cssparser(href->value,styles);
			}
			break;
		}
		default: {};
	}
	
	// process children
	for(unsigned int i = 0;i < tag->children.size();i++) {
		PostProcessTag(tag->children[i]);
	}
}

void HTMLParser::PostProcessTags() {
	
	// Verwerk title, css, etc
	for(unsigned int i = 0;i < tags->size();i++) {
		PostProcessTag((*tags)[i]);
	}
}

void HTMLParser::PostProcess() {

	// <link href="css">, <title>
	PostProcessTags();

	// Maak de stringlist
	PostProcessText();
}

void HTMLParser::PostProcessText() {
	
	// Add text snippets, separated by <>
	const char* ptr = buffer;
	while(1) {
		unsigned int textdelim = strcspn(ptr,"<>");
		switch(*(ptr + textdelim)) {
			case '<': {
				
				// Copy and move to string list
				char* content = new char[textdelim + 1];
				strncpy(content,ptr,textdelim);
				content[textdelim] = 0;
				content = Trim(content);
				
				// if not empty...
				if(strlen(content) > 0) {
					AddSnippet(content,ptr - buffer);
				} else {
					delete[] content;
				}
				break;
			}
			case '>': { // ending found, ignore
				break;
			}
			default: {
				return; // null terminator
			}
		}
		ptr += textdelim + 1; // step over delimiter
	}
}

Tag* HTMLParser::GetTagByIndex(TagVector* list,unsigned int index) {
	for(unsigned int i = 0;i < list->size();i++) {
		Tag* tag = (*list)[i];
		
		// Inside tags
		if(tag->starttagend < index and index < tag->endtagbegin) {
			if(tag->children.size() > 0) {
				Tag* result = GetTagByIndex(&tag->children,index);
				if(result) { // found in a child
					return result;					
				} else {
					return tag;
				}
			} else {
				return tag;
			}	
		}
	}
	return NULL;
}

void HTMLParser::Load(const char* filename) {
	FILE* html = fopen(filename,"rb");
	if(html) {
		
		// How big is the file?
		fseek(html,0,SEEK_END);
		unsigned int size = ftell(html);
		fseek(html,0,SEEK_SET);
		
		// Read the whole thing
		buffer = new char[size+1];
		fread(buffer,sizeof(char),size,html);
		buffer[size] = 0;
		
		Parse(buffer);
		
		delete[] buffer;	
		
		fclose(html);
	} else {
		MsgBox(filename,"Error opening file!");
	}
}
