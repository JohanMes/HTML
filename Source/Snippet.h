#ifndef SNIPPET_H
#define SNIPPET_H

#include <vector>

#include "Tag.h"
#include "Rect.h"

// Scheelt getyp
class Snippet;
typedef std::vector<Snippet*> SnippetVector;

class Snippet {
	public:
		Snippet(const char* text,Tag* parent,unsigned int index);
		~Snippet();
		
		char* text;
		unsigned int index;
		Tag* parent;
		Rect rect; // drawn width (by Drawer)
};

#endif
