#ifndef TAG_H
#define TAG_H

#include <vector>
#include <windows.h>

#include "Property.h"
#include "Rect.h"

// Scheelt getyp
class Tag;
typedef std::vector<Tag*> TagVector;

enum TagType {
	ttHtml,
	ttBody,
	ttB,
	ttHead,
	ttTitle,
	ttDiv,
	ttLink, // css
	ttUnknown,
};

class Tag {
	void SetType(const char* tagtext);
	void SetProperties(const char* tagtext);
	public:
		Tag(const char* tagtext);
		Tag(const char* ptr,unsigned int length); // direct copy
		~Tag();
		
		PropertyVector properties; // width height color font
		TagType type;
		Tag* parent;
		char* name;
		TagVector children;
		Rect rect; // drawn width (by Drawer)
		unsigned int starttagbegin;
		unsigned int starttagend;
		unsigned int endtagbegin;
		unsigned int endtagend;

		void AddChild(Tag* child);
		void AddProperty(const char* ptr,unsigned int length);
		void AddProperty(Property* property);
		Property* GetProperty(const char* name);
};

#endif
