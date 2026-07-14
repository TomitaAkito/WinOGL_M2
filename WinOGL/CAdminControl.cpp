#include "pch.h"
#include "CAdminControl.h"


CAdminControl::CAdminControl() {
	vertex_head = NULL;
	vertex_tail = NULL;
}


CAdminControl::~CAdminControl() {
}

void CAdminControl::Draw() {

	for (CVertex* currentV = vertex_head;currentV != NULL;currentV = currentV->GetNextVertex()) {
		DrawVertex(currentV, 1.0, 1.0, 1.0, 10, GL_POINTS);
		DrawLine(currentV, 1.0, 1.0, 1.0, 2.0, GL_LINE_STRIP);
	}
}

void CAdminControl::DrawVertex(CVertex* currentV, float R, float G, float B, float size, char mode) {
	glColor3f(R, G, B);
	glPointSize(size);
	glBegin(mode);
	glVertex2f(currentV->GetX(), currentV->GetY());
	glEnd();
}

void CAdminControl::DrawLine(CVertex* currentV, float R, float G, float B, float size, char mode) {
	// 例外処理
	if (!currentV->GetNextVertex()) return;

	glColor3f(R, G, B);
	glLineWidth(size);
	glBegin(mode);

	glVertex2f(currentV->GetX(), currentV->GetY());
	glVertex2f(currentV->GetNextVertex()->GetX(), currentV->GetNextVertex()->GetY());
	glEnd();
}

void CAdminControl::AddVertex(float mouse_x, float mouse_y) {
	CVertex* newVertex = new CVertex(mouse_x, mouse_y);

	// もしvertex_headが無ければ，それを最初のVertexにする
	if(!vertex_head){
		vertex_head = newVertex;
		vertex_tail = newVertex;
	}else {
		vertex_tail->SetNextVertex(newVertex);
		newVertex->SetPreVertex(vertex_tail);
		vertex_tail = newVertex;
	}
}
