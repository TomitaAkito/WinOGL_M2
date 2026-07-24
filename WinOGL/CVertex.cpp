#include "pch.h"
#include "CVertex.h"

CVertex::CVertex() {
	x = 0.0;
	y = 0.0;
	next = NULL;
	pre = NULL;
}

CVertex::CVertex(float mouse_x, float mouse_y) {
	x = mouse_x;
	y = mouse_y;
	next = NULL;
	pre = NULL;
}

CVertex::~CVertex() {
}

void CVertex::SetVertex(float mouse_x, float mouse_y) {
	x = mouse_x;
	y = mouse_y;
}

void CVertex::SetVertex(CVertex* vertex) {
	x = vertex->GetX();
	y = vertex->GetY();
}

void CVertex::SetNextVertex(CVertex* nextVertex) {
	next = nextVertex;
}

void CVertex::SetPreVertex(CVertex* preVertex) {
	pre = preVertex;
}

float CVertex::GetX() {
	return x;
}

float CVertex::GetY() {
	return y;
}

CVertex* CVertex::GetNextVertex() {
	return next;
}

CVertex* CVertex::GetPreVertex() {
	return pre;
}

