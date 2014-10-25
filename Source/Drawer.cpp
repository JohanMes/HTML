#include <windows.h>

#include "Drawer.h"
#include "Length.h"
#include "utils.h"

Drawer::Drawer(Page* parent) : pen(PS_NULL,0,0), backgroundcolor(255,255,255), bordercolor(255,255,255), fontcolor(0,0,0) {
	
	// draw this
	this->tags = &parent->tags; // read here
	this->styles = &parent->styles; // ...
	this->text = &parent->text;
	this->parent = parent;
}

Drawer::~Drawer() {

}

void Drawer::DrawAll(HDC hdc) {
	
	// Begin hiermee
	this->hdc = hdc;
	
	snippet = NULL;

	// Default brush (reuse)
	SelectObject(hdc,GetStockObject(DC_BRUSH)); // Brush class maken?

	// Default border pen
	pen.SetDC(hdc);

	// Default font
	font.SetDC(hdc);
	SetBkMode(hdc,TRANSPARENT); // Teken tekst altijd transparant
	
	// Draw some standard tags, use body for global settings
	Tag* bodytag = parent->GetFirstTag(&parent->tags,ttBody);
	if(bodytag) {
		DrawBody(bodytag);
	}

	// Draw snippets
	for(unsigned int i = 0;i < text->size();i++) {
		Snippet* snippet = (*text)[i];
		Tag* snippettag = snippet->parent;
		switch(snippettag->type) {
			case ttDiv:
			case ttBody:
			case ttHtml: {
				DrawSnippet((*text)[i]);
				break;
			}
			case ttTitle: {
				SendMessage(hwnd,WM_SETTEXT,0,(LPARAM)snippet->text);
				break;
			}
			default: {}// ignore
		}
	}
}

void Drawer::DrawRect(Rect* rect) { // lazy
	Rectangle(hdc,rect->left,rect->top,rect->right,rect->bottom);
}

void Drawer::SetColors(Style* style) {
	
	// Update background color
	Property* prop = style->GetProperty(ptBackgroundColor);
	if(prop) {
		backgroundcolor.SetValue(prop->value);
		SetDCBrushColor(hdc,backgroundcolor.colorref);
	}

	// Update border color
	prop = style->GetProperty(ptBorderColor);
	if(prop) {
		bordercolor.SetValue(prop->value);
		pen.SetColor(bordercolor.colorref);
	} else { // not specified? use background-color
		pen.SetColor(backgroundcolor.colorref);
	}
	
	// Set color
	prop = style->GetProperty(ptColor);
	if(prop) {
		fontcolor.SetValue(prop->value);
		font.SetColor(fontcolor.colorref);
	}
}

void Drawer::SetContentRect(Style* style) { // sets content
	
	// use parent rect as horizontal begin
	if(snippet and snippet->parent->parent) { // not top level
		contentrect = currentrect;
	} else { // top level thing
		contentrect.GetClientRectangle(hwnd);
		return;
	}
	
	// Adjust width
	Property* prop = style->GetProperty(ptWidth);
	if(prop) {
		Length length(hdc,elementrect.width,prop->value);
		contentrect.SetRight(contentrect.left + length.value);
	}

	// Adjust height to content
	prop = style->GetProperty(ptHeight);
	if(prop) {
		Length length(hdc,elementrect.height,prop->value);
		contentrect.SetBottom(contentrect.top + length.value);

	} else { // property not found? match content height
		RECT textrect = {contentrect.left,0,contentrect.right,0}; // set x boundaries, let it calculate y
		DrawText(hdc,snippet->text,-1,&textrect,DT_WORDBREAK|DT_CALCRECT);
		contentrect.SetBottom(contentrect.top + textrect.bottom);
	}
}

void Drawer::SetElementRect(Style* style) { // sets content + border

	elementrect = contentrect;
	
	// Check border width
	Property* prop = style->GetProperty(ptBorderWidth);
	if(prop) {
		Length length(hdc,0,prop->value);
		pen.SetWidth(length.value);
		if(pen.style == PS_NULL) { // make visible
			pen.SetStyle(PS_SOLID);
		}
	} else {
		pen.SetStyle(PS_NULL); // make invisible
	}
	contentrect.Inflate(-pen.width,-pen.width);
}

void Drawer::SetMarginRect(Style* style) { // sets content + border + margin

	marginrect = elementrect;
	
	// Set margins
	Property* prop = style->GetProperty(ptMarginLeft);
	if(prop) {
		Length length(hdc,elementrect.width,prop->value);
		contentrect.SetLeft(contentrect.left + length.value); // squash content
		elementrect.SetLeft(elementrect.left + length.value);
	}
	
	prop = style->GetProperty(ptMarginTop);
	if(prop) {
		Length length(hdc,elementrect.height,prop->value);
		contentrect.MoveY(length.value);
		elementrect.MoveY(length.value);
		marginrect.SetBottom(elementrect.bottom);
	}

	prop = style->GetProperty(ptMarginRight);
	if(prop) {
		Length length(hdc,elementrect.width,prop->value);
		contentrect.SetRight(contentrect.right - length.value); // squash content
		elementrect.SetRight(elementrect.right - length.value);
	}

	prop = style->GetProperty(ptMarginBottom);
	if(prop) {
		Length length(hdc,elementrect.height,prop->value);
		marginrect.SetBottom(elementrect.bottom + length.value);
	}
}

void Drawer::SetRects(Style* style) {
	SetContentRect(style);
	SetElementRect(style);
	SetMarginRect(style);
}

void Drawer::SetFont(Style* style) {
	
	// Set typeface
	Property* prop = style->GetProperty(ptFont);
	if(prop) {
		font.SetTypeFace(prop->value);
	}
}

Style* Drawer::GetSingleTagStyle(Tag* tag) {
	
	// Try to find style by #id
	Property* property = tag->GetProperty("id");
	if(property) {
		for(unsigned int i = 0;i < styles->size();i++) {
			Style* style = (*styles)[i];
			if(!strcmp(&style->name[1],property->value) && style->name[0] == '#') {
				return style;
			}
		}
	}
	
	// Then try to find style by tag kind
	for(unsigned int i = 0;i < styles->size();i++) {
		if(!strcmp((*styles)[i]->name,tag->name)) {
			return (*styles)[i];
		}
	}

	return NULL;
}
	
Style Drawer::GetInheritedTagStyle(Tag* tag) {
	
	// Create list of styles to apply
	Style style;
	
	// Apply styles of this list
	TagVector list;
	
	// Get topmost parent
	Tag* parent = tag;
	while(parent) {
		list.insert(list.begin(),parent);
		parent = parent->parent;
	}
	
	// Then walk down, adding styles to style of each parent tag
	for(unsigned int i = 0;i < list.size()-1;i++) { // ignore our own tag
	
		// Add CSS stuff
		Style* tagstyle = GetSingleTagStyle(list[i]);
		if(tagstyle) {
			
			// Add this one to the final list
			for(unsigned int j = 0;j < tagstyle->properties.size();j++) {
				
				// Not all styles inherit...
				switch(tagstyle->properties[j]->type) {
					case ptColor:
					case ptBackgroundColor:
					case ptBorderColor: {
						style.AddProperty(tagstyle->properties[j]);
					}
					default: {}
				}
			}
		}
	}
	
	// Then add styles of our own tag
	Style* tagstyle = GetSingleTagStyle(tag);
	if(tagstyle) {
		for(unsigned int i = 0;i < tagstyle->properties.size();i++) {
			style.AddProperty(tagstyle->properties[i]);
		}
	}
	return style;
}
	
Style Drawer::GetInheritedSnippetStyle(Snippet* snippet) {
	return GetInheritedTagStyle(snippet->parent);
}

void Drawer::DrawBody(Tag* tag) { // body, html
	
	// Create list of styles to apply
	Style style = GetInheritedTagStyle(tag);
	
	// Apply colors only
	SetColors(&style);
	
	// Fill whole but apply borders
	SetRects(&style);
	
	// Draw
	DrawRect(&elementrect); // don't draw margin
	
	currentrect = marginrect;
}

void Drawer::DrawSnippet(Snippet* snippet) {
	
	// Create list of styles to apply
	this->snippet = snippet;
	Style style = GetInheritedSnippetStyle(snippet);
		
	// Don't update handles too often
	pen.BeginUpdate();
	
	// Only recreate HFONT once
	font.BeginUpdate();
	
	// Update font
	SetFont(&style); // font type, font width
	SetColors(&style); // brush color, pen color, font color
	
	font.EndUpdate(); // update font object

	// content, border, margin
	SetRects(&style); // needs finished font
	
	pen.EndUpdate();
	
	// Draw
	DrawRect(&elementrect); // content + border
	DrawText(hdc,snippet->text,-1,&contentrect,DT_WORDBREAK); // content
	
	// Move current rect down
	currentrect.MoveY(marginrect.height);
}
