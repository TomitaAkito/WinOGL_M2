#include "pch.h"
#include "CAdminControl.h"


CAdminControl::CAdminControl() {
	shape_head = NULL;
	shape_tail = NULL;
	mouseVertex = new CVertex();
	POINTSIZE = 10;
	LINEWIDTH = 2.0;
	AxisFlag = false;
}


CAdminControl::~CAdminControl() {
}

void CAdminControl::Draw() {
	// 座標軸の表示
	if (AxisFlag) DrawAxis();

	// 例外処理
	if (!shape_head) return;

	// 図形リストの描画
	for (CShape* currentShape = shape_head;currentShape != NULL;currentShape = currentShape->GetNextShape()) {
		for (CVertex* currentV = currentShape->GetVertexHead();currentV != NULL;currentV = currentV->GetNextVertex()) {
			DrawVertex(currentV, 1.0, 1.0, 1.0, POINTSIZE, GL_POINTS);
			if (currentV->GetNextVertex())
				DrawLine(currentV,currentV->GetNextVertex(), 1.0, 1.0, 1.0, LINEWIDTH, GL_LINE_STRIP);
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

	// 他交差 & 内包判定
	if (!drawFlag && shape_head!=shape_tail) {
		// 一旦図形に仮の頂点を挿入
		CVertex* tmpVertex = new CVertex(mouseVertex->GetX(), mouseVertex->GetY());
		shape_tail->AddVertex(tmpVertex);

		// 他交差判定
		drawFlag = isOtherCrossing(tmpVertex);

		// 内包判定
		if(!drawFlag) drawFlag = isContains(tmpVertex);

		// 仮の頂点を削除
		if (!shape_tail->GetCloseFlag())shape_tail->freeVertex(tmpVertex);
		else shape_tail->freeVertex(shape_tail->GetVertexTail());
	}

	// 破線を可能にする
	glEnable(GL_LINE_STIPPLE);
	//破線のパターンの指定（0xF0F0の部分がそれ）
	glLineStipple(1, 0xF0F0);

	if (drawFlag) DrawLine(mouseVertex, shape_tail->GetVertexTail(), 1.0, 0.0, 0.0, LINEWIDTH, GL_LINE_STRIP);
	else DrawLine(mouseVertex, shape_tail->GetVertexTail(), 0.0, 1.0, 0.0, LINEWIDTH, GL_LINE_STRIP);

	// 破線の終了
	glDisable(GL_LINE_STIPPLE);
}

// 座標軸の表示
void CAdminControl::DrawAxis() {
	glBegin(GL_LINES);
	// x軸
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	// y軸
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	// z軸
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
}

void CAdminControl::SetMouseVertex(float mouse_x, float mouse_y) {
	mouseVertex->SetVertex(mouse_x, mouse_y);
}

void CAdminControl::AddVertex(float mouse_x, float mouse_y) {
	CVertex* newVertex = new CVertex(mouse_x, mouse_y);

	// もしshape_headが無ければ，生成する
	if (!shape_head && !shape_tail) {
		CShape* shape = new CShape();
		shape_head = shape;
		shape_tail = shape;
	}

	// 自交差判定込みでshape_tailにくっつける
	if (!shape_tail->AddVertex(newVertex)) return;
		
	// 他交差判定
	if (isOtherCrossing(newVertex)) shape_tail->freeVertex(newVertex);

	// 内包判定
	if(isContains(newVertex))	shape_tail->freeVertex(newVertex);

	// 図形を閉じない場合は終了
	if (!shape_tail->GetCloseFlag()) return;

	// 今回の処理で図形が閉じた場合は新しい図形を設ける
	CShape* newShape = new CShape();
	shape_tail->SetNextShape(newShape);
	newShape->SetPreShape(shape_tail);
	shape_tail = newShape;
	return;		
}

bool CAdminControl::isOtherCrossing(CVertex* newVertex) {
	// 例外処理
	if (!shape_head->GetNextShape())return false;
	//CShape* newShape = isVertexInShape(newVertex);
	if (shape_tail->GetVertex_count() <= 1) return false;

	CShape* newVertexShape = shape_tail;

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

bool CAdminControl::isContains(CVertex* newVertex) {
	// 新しく加えた頂点が内包しているか
	if(isContainsVertex(newVertex))return true;

	// 形状を閉じた際に図形を内包していないか
	if(isContainsShape()) return true;

	return false;
}


bool CAdminControl::isContainsVertex(CVertex* newVertex) {
	// 例外判定
	if (shape_head == shape_tail)return false;

	CMath calc;
	CShape* newShape = isVertexInShape(newVertex);
	float angle;

	for (CShape* currentShape = shape_head;currentShape != NULL;currentShape = currentShape->GetNextShape()) {
		angle = calc.angle(currentShape, newVertex);

		if (calc.GetPie() * 2 * 0.9 <= angle && angle <= calc.GetPie() * 2 * 1.1)
			return true;
	}
	return false;
}

bool CAdminControl::isContainsShape() {
	// 例外処理
	if(!shape_tail->GetCloseFlag()) return false;
	
	CMath calc;
	float angle;

	for(CShape* currentShape = shape_head;currentShape != NULL;currentShape = currentShape->GetNextShape()) {
		angle = calc.angle(shape_tail, currentShape->GetVertexHead());

		if(calc.GetPie() * 2 * 0.9 <= angle && angle <= calc.GetPie() * 2 * 1.1)
			return true;
	}
	return false;
}

void CAdminControl::DrawSizeChange(int mode) {
	if (mode > 0) {
		if (LINEWIDTH <= 10.0) {
			POINTSIZE += 0.7;
			LINEWIDTH += 0.4;
		}
	}
	else {
		if (LINEWIDTH >= 1) {
			POINTSIZE -= 0.7;
			LINEWIDTH -= 0.4;
		}
	}	
}

