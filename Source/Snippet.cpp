#include <string.h>

#include "Snippet.h"

Snippet::Snippet(const char* text,Tag* parent,unsigned int index) {
	this->text = new char[strlen(text) + 1];
	strcpy(this->text,text);
	this->index = index;
	this->parent = parent;
}

Snippet::~Snippet() {
	delete[] text;
}
