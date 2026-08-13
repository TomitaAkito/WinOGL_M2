#include "pch.h"
#include "CAdminControl.h"


CAdminControl::CAdminControl() {
	shape_head = NULL;
	shape_tail = NULL;
	mouseVertex = new CVertex();
}


CAdminControl::~CAdminControl() {
}

void CAdminControl::Draw() {
	// 例外処理
	if (!shape_head) return;

	// 図形リストの描画
	for (CShape* currentShape = shape_head;currentShape != NULL;currentShape = currentShape->GetNextShape()) {
		for (CVertex* currentV = currentShape->GetVertexHead();currentV != NULL;currentV = currentV->GetNextVertex()) {
			DrawVertex(currentV, 1.0, 1.0, 1.0, 10, GL_POINTS);
			if (currentV->GetNextVertex())
				DrawLine(currentV,currentV->GetNextVertex(), 1.0, 1.0, 1.0, 2.0, GL_LINE_STRIP);
		}
	}

	// 予測線の描画
	DrawForecastLine();
}

void CAdminControl::DrawVertex(CVertex* currentV, float R, float G, float B, float size, char mode) {
	glColor3f(R, G, B);
	glPointSize(size);
	glBegin(mode);
	glVertex2f(currentV->GetX(), currentV->GetY());
	glEnd();
}

void CAdminControl::DrawLine(CVertex* v1, CVertex* v2, float R, float G, float B, float size, char mode) {
	glColor3f(R, G, B);
	glLineWidth(size);
	glBegin(mode);

	glVertex2f(v1->GetX(), v1->GetY());
	glVertex2f(v2->GetX(), v2->GetY());
	glEnd();
}

void CAdminControl::DrawForecastLine() {
	// 例外処理
	if (!shape_head || !shape_tail) return;
	if (!shape_tail->GetVertexHead()) return;

	
	// 自交差判定
	bool drawFlag = shape_tail->IsSelfCrossing(mouseVertex);

	// 他交差
	if (!drawFlag && shape_head!=shape_tail) {
		CVertex* tmpVertex = new CVertex(mouseVertex->GetX(), mouseVertex->GetY());
		shape_tail->AddVertex(tmpVertex);
		drawFlag = isOtherCrossing(tmpVertex);
		shape_tail->freeVertex(tmpVertex);
	}

	// 破線を可能にする
	glEnable(GL_LINE_STIPPLE);
	//破線のパターンの指定（0xF0F0の部分がそれ）
	glLineStipple(1, 0xF0F0);

	if (drawFlag) DrawLine(mouseVertex, shape_tail->GetVertexTail(), 1.0, 0.0, 0.0, 2.0, GL_LINE_STRIP);
	else DrawLine(mouseVertex, shape_tail->GetVertexTail(), 0.0, 1.0, 0.0, 2.0, GL_LINE_STRIP);

	// 破線の終了
	glDisable(GL_LINE_STIPPLE);
}

void CAdminControl::SetMouseVertex(float mouse_x, float mouse_y) {
	mouseVertex->SetVertex(mouse_x, mouse_y);
}

void CAdminControl::AddVertex(float mouse_x, float mouse_y) {
	CVertex* newVertex = new CVertex(mouse_x, mouse_y);

	// もしshape_headが無ければ，生成する
	if (!shape_head) {
		CShape* shape = new CShape();
		shape_head = shape;
		shape_tail = shape;
	}


	for (CShape* currentShape = shape_head;currentShape != NULL;currentShape = currentShape->GetNextShape()) {
		// 探索している図形で頂点を作れた場合は終了
		
		// 自交差判定込み
		bool vertexAddflag = currentShape->AddVertex(newVertex);
		
		// 他交差判定
		if (vertexAddflag && isOtherCrossing(newVertex)) {
			currentShape->freeVertex(newVertex);
		}
				
		if (vertexAddflag && !currentShape->GetCloseFlag()) return;

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

bool CAdminControl::isOtherCrossing(CVertex* newVertex) {
	// 例外処理
	if (!shape_head->GetNextShape())return false;

	CShape* newVertexShape = isVertexInShape(newVertex);

	for (CShape* currentShape = shape_head; currentShape != NULL; currentShape = currentShape->GetNextShape()) {

		// 自分の図形は例外
		if (currentShape == newVertexShape) continue;

		for (CVertex* currentVertex = currentShape->GetVertexHead(); currentVertex != NULL; currentVertex = currentVertex->GetNextVertex()) {
			if (currentShape->IsCrossing2Lines(currentVertex, currentVertex->GetNextVertex(), shape_tail->GetVertexTail()->GetPreVertex(), newVertex))
				return true;
		}
	}
	return false;
}

CShape* CAdminControl::isVertexInShape(CVertex* vertex) {

	for (CShape* currentShape = shape_head; currentShape != NULL; currentShape = currentShape->GetNextShape()) {
		for (CVertex* currentVertex = currentShape->GetVertexHead(); currentVertex != NULL; currentVertex = currentVertex->GetNextVertex()) {
			if (currentShape->isVertexCoordinate(currentVertex, vertex))
				return currentShape;
		}
	}

	return nullptr;
}
