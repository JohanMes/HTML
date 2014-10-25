#include "Pen.h"

Pen::Pen() {
}

Pen::Pen(int style,int width,COLORREF color) {
	this->style = style;
	this->width = width;
	this->color = color;
	hdc = NULL;
	updatecount = 0;
	Update();
}

void Pen::Update() {
	if(updatecount == 0) {
		DeleteObject(handle);
		handle = CreatePen(style,width,color);
		if(hdc) {
			SelectObject(hdc,handle);
		}
	}
}

void Pen::SetDC(HDC hdc) {
	this->hdc = hdc;
	SelectObject(hdc,handle);
}

void Pen::SetStyle(int value) {
	if(this->style != value) {
		this->style = value;
		Update();
	}	
}

void Pen::SetWidth(int value) {
	if(this->width != value) {
		this->width = value;
		Update();
	}
}

void Pen::SetColor(COLORREF value) {
	if(this->color != value) {
		this->color = value;
		Update();
	}
}

void Pen::BeginUpdate() {
	updatecount++;
}

void Pen::EndUpdate() {
	updatecount--;
	if(updatecount == 0) {
		Update();
	}
}
