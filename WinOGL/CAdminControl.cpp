#include "pch.h"
#include "CAdminControl.h"


CAdminControl::CAdminControl() {
	shape_head = NULL;
	shape_tail = NULL;
}


CAdminControl::~CAdminControl() {
}

void CAdminControl::Draw() {
	// 例外処理
	if (!shape_head) return;

	for (CShape* currentShape = shape_head;currentShape != NULL;currentShape = currentShape->GetNextShape()) {
		for (CVertex* currentV = currentShape->GetVertexHead();currentV != NULL;currentV = currentV->GetNextVertex()) {
			DrawVertex(currentV, 1.0, 1.0, 1.0, 10, GL_POINTS);
			DrawLine(currentV, 1.0, 1.0, 1.0, 2.0, GL_LINE_STRIP);
		}
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

	// もしshape_headが無ければ，生成する
	if (!shape_head) {
		CShape* shape = new CShape();
		shape_head = shape;
	}


	for (CShape* currentShape = shape_head;currentShape != NULL;currentShape = currentShape->GetNextShape()) {
		// 探索している図形で頂点を作れた場合は終了
		bool vertexAddflag = currentShape->AddVertex(newVertex);
		if (!currentShape->GetCloseFlag() && vertexAddflag) return;

		// 今回の処理で図形が閉じた場合は新しい図形を設ける
		if (currentShape->GetCloseFlag() && vertexAddflag) {
			CShape* newShape = new CShape();
			currentShape->SetNextShape(newShape);
			shape_tail = newShape;
			shape_tail->SetPreShape(currentShape);
			return;
		}
		
		// 探索図形の形状が閉じていないで無理だった場合，終了
		// 自交差など…
		if (currentShape->GetCloseFlag()==false && !vertexAddflag) return;
		
	}
}
