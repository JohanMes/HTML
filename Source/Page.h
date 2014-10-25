#ifndef PAGE_H
#define PAGE_H

#include "Tag.h"
#include "Style.h"
#include "Drawer.h"
#include "Snippet.h"
#include "HTMLParser.h"

class Page {
	Drawer drawer; // don't recreate on WM_PAINT
	HTMLParser parser; // Spawns CSSParser when needed
	public:
		Page();
		Page(const char* filename);
		~Page();
		
		char* filename;
		TagVector tags; // html result
		StyleVector styles; // css result
		SnippetVector text; // linear representation

		void Refresh();
		void Load(const char* filename);
		void Draw(HDC hdc);
		Tag* GetFirstTag(TagVector* tags,TagType tagtype);
};

#endif
