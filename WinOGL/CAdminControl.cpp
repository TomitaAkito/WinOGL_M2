#include "pch.h"
#include "CAdminControl.h"


CAdminControl::CAdminControl() {
}


CAdminControl::~CAdminControl() {
}

void CAdminControl::Draw() {
	// 点の描画
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	glVertex2f(-1.0, 0.5);
	glVertex2f(0.0, -0.5);
	glVertex2f(1.0, 0.5);
	glVertex2f(-1.0, 0.5);
	glEnd();

	// 線の描画;
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-1.0, 0.5);
	glVertex2f(0.0, -0.5);
	glVertex2f(1.0, 0.5);
	glVertex2f(-1.0, 0.5);
	glEnd();
}
