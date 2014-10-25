#include "Rect.h"

Rect::Rect() {
	
}

Rect::Rect(int left,int top,int right,int bottom) {
	this->left   = left;
	this->top    = top;
	this->right  = right;
	this->bottom = bottom;
	Update();
}

Rect::Rect(RECT rect) {
	this->left   = rect.left;
	this->top    = rect.top;
	this->right  = rect.right;
	this->bottom = rect.bottom;
	Update();
}

Rect::~Rect() {
	
}

void Rect::Update() {
	width = right - left;
	height = bottom - top;	
}

void Rect::GetClientRectangle(HWND hwnd) {
	GetClientRect(hwnd,this);
	Update();
}

void Rect::SetLeft(int value) {
	left = value;
	Update();
}
void Rect::SetTop(int value) {
	top = value;
	Update();
}
void Rect::SetRight(int value) {
	right = value;
	Update();
}
void Rect::SetBottom(int value) {
	bottom = value;
	Update();
}

void Rect::MoveX(int value) {
	left += value;
	right += value;	
}

void Rect::MoveY(int value) {
	top += value;
	bottom += value;
}

void Rect::Inflate(int dx,int dy) {
	InflateRect(this,dx,dy);
	Update();
}
