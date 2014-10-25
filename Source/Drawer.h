#ifndef DRAWER_H
#define DRAWER_H

#include "Tag.h"
#include "Color.h"
#include "Style.h"
#include "Snippet.h"
#include "Rect.h"
#include "Pen.h"
#include "Font.h"

class Page;

class Drawer {
	
	// Caches
	Rect contentrect; // content
	Rect elementrect; // content + border
	Rect marginrect;  // content + border + margin
	Rect currentrect; // stores position of document flow
	HDC hdc;
	Font font;
	Pen pen;
	Color backgroundcolor;
	Color bordercolor;
	Color fontcolor; // font color
	Snippet* snippet; // current snippet
	Page* parent;

	// Style getters
	Style* GetSingleTagStyle(Tag* tag); // only check tag
	Style GetInheritedTagStyle(Tag* tag); // inheritance too
	Style GetInheritedSnippetStyle(Snippet* snippet); // inheritance too
	
	// Style setters
	void SetColors(Style* style); // brush color, pen color, font color
	void SetContentRect(Style* style); // pen width
	void SetElementRect(Style* style);
	void SetMarginRect(Style* style);
	void SetRects(Style* style);
	void SetFont(Style* style); // font type, font size
	
	// Drawers
	void DrawRect(Rect* rect);
	void DrawBody(Tag* tag);
	void DrawSnippet(Snippet* snippet);
	public:
		Drawer(Page* parent);
		~Drawer();
		
		// Draw this
		TagVector* tags; // html result
		StyleVector* styles; // css result
		SnippetVector* text; // linear representation
		
		void DrawAll(HDC hdc);
};

#endif

//		// Add HTML stuff
//		for(unsigned int j = 0;j < snippet->parent->properties.size();i++) {
//			
//			// Not all styles inherit...
//			switch(tagstyle->properties[j]->type) {
//				case ptHeight:
//				case ptWidth: { // ignore
//					break;
//				}
//				default: {
//					style.AddProperty(snippet->parent->properties[j]);
//				}
//			}
//		}
