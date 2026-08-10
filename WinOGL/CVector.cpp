#include "pch.h"
#include "CVector.h"

CVector::CVector() {
	x = 0.0f;
	y = 0.0f;
}

CVector::CVector(CVertex* v1, CVertex* v2) {
	x = v2->GetX() - v1->GetX();
	y = v2->GetY() - v1->GetY();
}

CVector::~CVector() {
}

float CVector::GetX() {
	return x;
}

float CVector::GetY() {
	return y;
}
