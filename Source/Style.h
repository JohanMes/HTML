#ifndef STYLE_H
#define STYLE_H

#include <vector>
#include "Property.h"

// Scheelt getyp
class Style;
typedef std::vector<Style*> StyleVector;

class Style {
	public:
		Style();
		Style(const char* name);
		~Style();
		
		char* name;
		PropertyVector properties;
		
		void Process(const char* styletext);
		Property* GetProperty(const char* name);
		Property* GetProperty(PropertyType type);
		void AddProperty(const char* ptr,unsigned int length);
		void AddProperty(const char* name,const char* value);
		void AddProperty(Property* property);
};

#endif
