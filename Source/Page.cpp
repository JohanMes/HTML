#include <string.h>

#include "HTMLParser.h"
#include "Page.h"

Page::Page() : drawer(this), parser(this) {
	// ...
}

Page::Page(const char* filename) : drawer(this), parser(this) {
	Load(filename);
}

void Page::Load(const char* filename) {
	this->filename = new char[strlen(filename) + 1];
	strcpy(this->filename,filename);
	
	// Let a HTML parser do all the work
	tags.clear();
	styles.clear();
	text.clear();
	parser.Load(filename);
}

void Page::Draw(HDC hdc) {
	drawer.DrawAll(hdc);
}

void Page::Refresh() { 
	Load(filename);
}

Tag* Page::GetFirstTag(TagVector* tags,TagType tagtype) {
	for(unsigned int i = 0;i < tags->size();i++) {
		Tag* currenttag = (*tags)[i];
		if(currenttag->type == tagtype) {
			return currenttag;
		} else {
			// try children
			Tag* result = GetFirstTag(&currenttag->children,tagtype);
			if(result) {
				return result;
			} // else, keep going
		}
	}
	return NULL;
}
