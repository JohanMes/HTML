#ifndef PROPERTY_H
#define PROPERTY_H

#include <vector>

// Scheelt getyp
class Property;
typedef std::vector<Property*> PropertyVector;

enum PropertyType {
	
	// Content colors
	ptBackgroundColor,
	ptColor,
	
	// Size
	ptHeight,
	ptWidth,

	// Margin size
	ptMarginLeft,
	ptMarginTop,
	ptMarginRight,
	ptMarginBottom,
	
	// Border
	ptBorderColor,
	ptBorderWidth,
	
	// Fonts
	ptFont,
	ptFontSize,
	
	// Misc.
	ptUnknown,
};

class Property {
	public:
		Property(const char* name,const char* value);
		~Property();
		void SetType();
		char* name;
		char* value;
		PropertyType type;
};

#endif
