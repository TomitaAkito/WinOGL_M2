#include "pch.h"
#include "CAdminControl.h"


CAdminControl::CAdminControl() {
	shape_head = NULL;
	shape_tail = NULL;
	mouseVertex = new CVertex();
	pre_mouseVertex = new CVertex();
	POINTSIZE = 10;
	LINEWIDTH = 2.0;
	AxisFlag = false;
	EditFlag = false;
	selectVertex = NULL;
	selectLineStart = NULL;
	selectShape = NULL;
	SELECT_THRESHOLD = 0.03;
	selectCount = 0;
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
			DrawVertex(currentV, 1.0, 1.0, 1.0, POINTSIZE);
			if (currentV->GetNextVertex())
				DrawLine(currentV,currentV->GetNextVertex(), 1.0, 1.0, 1.0, LINEWIDTH);
		}
	}

	// 予測線の描画
	if(!EditFlag)DrawForecastLine();

	// 選択モードの描画
	if (EditFlag) DrawSelect();
}

void CAdminControl::DrawVertex(CVertex* currentV, float R, float G, float B, float size) {
	glColor3f(R, G, B);
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex2f(currentV->GetX(), currentV->GetY());
	glEnd();
}

void CAdminControl::DrawLine(CVertex* v1, CVertex* v2, float R, float G, float B, float size) {
	glColor3f(R, G, B);
	glLineWidth(size);
	glBegin(GL_LINE_STRIP);

	glVertex2f(v1->GetX(), v1->GetY());
	glVertex2f(v2->GetX(), v2->GetY());
	glEnd();
}

void CAdminControl::DrawForecastLine() {
	// 例外処理
	if (!shape_head || !shape_tail) return;
	if (!shape_tail->GetVertexHead()) return;
	if (shape_tail->GetCloseFlag())return;
	
	// 自交差判定
	bool drawFlag = shape_tail->IsSelfCrossing(mouseVertex);

	// 他交差 & 内包判定
	if (!drawFlag && shape_head!=shape_tail) {
		// 一旦図形に仮の頂点を挿入
		CVertex* tmpVertex = new CVertex(mouseVertex->GetX(), mouseVertex->GetY());
		shape_tail->AddVertex(tmpVertex);

		// 他交差判定 & 内包判定
		drawFlag = (isOtherCrossing(tmpVertex)||isContains(tmpVertex));

		// 仮の頂点を削除
		if (!shape_tail->GetCloseFlag())shape_tail->freeVertex(tmpVertex);
		else shape_tail->freeVertex(shape_tail->GetVertexTail());
	}

	// 破線を可能にする
	glEnable(GL_LINE_STIPPLE);
	//破線のパターンの指定（0xF0F0の部分がそれ）
	glLineStipple(1, 0xF0F0);

	if (drawFlag) DrawLine(mouseVertex, shape_tail->GetVertexTail(), 1.0, 0.0, 0.0, LINEWIDTH);
	else DrawLine(mouseVertex, shape_tail->GetVertexTail(), 0.0, 1.0, 0.0, LINEWIDTH);

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

void CAdminControl::DrawSelect() {
	// 頂点選択時
	if (selectVertex) DrawVertex(selectVertex, 1.0, 0.0, 0.0, POINTSIZE);
	// 線選択時
	else if (selectLineStart) DrawLine(selectLineStart,selectLineStart->GetNextVertex(), 1.0, 0.0, 0.0, LINEWIDTH);
	// 図形選択時
	else if (selectShape) {
		for (CVertex* currentVertex = selectShape->GetVertexHead(); currentVertex != NULL; currentVertex = currentVertex->GetNextVertex()) {
			DrawVertex(currentVertex, 1.0, 0.0, 0.0, POINTSIZE);
			if (currentVertex->GetNextVertex())
				DrawLine(currentVertex, currentVertex->GetNextVertex(), 1.0, 0.0, 0.0, LINEWIDTH);
		}
	}
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
	else if (shape_tail->GetCloseFlag()) {
		// 図形が閉じている場合は新しい図形を設ける
		CShape* newShape = new CShape();
		shape_tail->SetNextShape(newShape);
		newShape->SetPreShape(shape_tail);
		shape_tail = newShape;
	}

	// 重なる場合は終了
	if(isOverlapVertex(newVertex)) return;

	// 自交差判定込みでshape_tailにくっつける
	if (!shape_tail->AddVertex(newVertex)) return;
		
	// 他交差判定
	if (isOtherCrossing(newVertex)) shape_tail->freeVertex(newVertex);

	// 内包判定
	if(isContains(newVertex))	shape_tail->freeVertex(newVertex);
	return;		
}

void CAdminControl::Edit(float mouse_x, float mouse_y) {
	// マウス座標を格納
	SetMouseVertex(mouse_x, mouse_y);
	
	// マウス座標をpreに格納
	pre_mouseVertex->SetVertex(mouse_x, mouse_y);

	// 図形・点・線の選択
	Select();

}

void CAdminControl::EditReset() {
	selectVertex = NULL;
	selectLineStart = NULL;
	selectShape = NULL;
	selectCount = 1;
}

void CAdminControl::Move(float mouse_x, float mouse_y) {
	// いずれも選んでなければ無視する
	if (!selectVertex && !selectLineStart && !selectShape)return;

	// 選択回数が満たさない場合はbreak
	if (selectCount < 2) return;

	if (selectVertex)MoveVertex(mouse_x, mouse_y);

	if(selectShape)MoveShape(mouse_x, mouse_y);
}

void CAdminControl::MoveVertex(float mouse_x, float mouse_y) {
	// 図形を取得
	CShape* selectS = isVertexInShape(selectVertex);
	
	// 前の頂点を保持
	float beforeX = selectVertex->GetX();
	float beforeY = selectVertex->GetY();

	// 一旦動かす
	selectVertex->SetVertex(mouse_x, mouse_y);

	// もしvertex_headを選んでいたら，vertex_tailも移動させる
	if (selectS->isVertexCoordinate(selectVertex, selectS->GetVertexHead()))
		selectS->GetVertexTail()->SetVertex(mouse_x, mouse_y);

	// 問題があった場合は図形を戻す
	// ->交差判定と内包判定
	if (selectS->IsSelfCrossingByMoving(selectVertex, shape_head) || isContainsShapeByMoving()) {
		selectVertex->SetVertex(beforeX, beforeY);
		if (selectS->isVertexCoordinate(selectVertex, selectS->GetVertexHead()))
			selectS->GetVertexTail()->SetVertex(beforeX, beforeY);
	}	
}

void CAdminControl::MoveShape(float mouse_x, float mouse_y) {
	// 例外処理
	if(!selectShape) return;

	// マウス座標から移動量をベクトルとして算出する
	CVector* momentum = new CVector(pre_mouseVertex, mouseVertex);

	// 図形をコピーする
	CShape* cpShape = new CShape();
	copyShape(selectShape, cpShape);

	// 一旦全部移動させる
	MoveShapeByMomentum(selectShape, momentum);
	
	// 判定
	bool ErrFlag = isContainsShapeByMoving();
	if(!ErrFlag)ErrFlag = isOtherCrossingByMoving();

	// どこかの頂点が内包する場合は元に戻す
	if(ErrFlag) MoveShapeByShape(selectShape, cpShape);
	else pre_mouseVertex->SetVertex(mouseVertex);

	// 削除
	delete momentum;
	delete cpShape;
}

void CAdminControl::MoveShapeByMomentum(CShape* shape, CVector* momentum) {
	for(CVertex* currentVertex = shape->GetVertexHead();currentVertex != NULL;currentVertex = currentVertex->GetNextVertex()) {
		currentVertex->SetVertex(currentVertex->GetX() + momentum->GetX(), currentVertex->GetY() + momentum->GetY());
	}
}

void CAdminControl::MoveShapeByShape(CShape* shape, CShape* afterShape) {
	// 例外処理
	if(!shape || !afterShape)return;
	if(shape->GetVertex_count() != afterShape->GetVertex_count())return;

	CVertex* afV = afterShape->GetVertexHead();
	for(CVertex* currentV = shape->GetVertexHead();currentV != NULL;currentV = currentV->GetNextVertex(), afV = afV->GetNextVertex()) {
		currentV->SetVertex(afV);
	}
}

void CAdminControl::InsertVertex() {

	CVector ab(selectLineStart, selectLineStart->GetNextVertex());
	CVector ap(selectLineStart, mouseVertex);
	CMath calc;

	float t = calc.projectionT(ab, ap);
	
	// 端的と重なる場合はbreak
	if (t == 0 || t == 1) {
		EditReset();
		return;
	}

	float newX = selectLineStart->GetX() + (selectLineStart->GetNextVertex()->GetX() - selectLineStart->GetX()) * t;
	float newY = selectLineStart->GetY() + (selectLineStart->GetNextVertex()->GetY() - selectLineStart->GetY()) * t;
	CVertex* newVertex = new CVertex(newX, newY);

	isVertexInShape(selectLineStart)->InsertVertex(newVertex, selectLineStart);
	EditReset();
	selectVertex = newVertex;
	selectCount++;
}

bool CAdminControl::freeVertex(CVertex* DeleteV) {
	// 例外処理
	if (!DeleteV)return false;
	CShape* selectS = isVertexInShape(DeleteV);
	if (selectS->GetCloseFlag() && selectS->GetVertex_count() <= 3) return false;

	bool headTailFlag = false;

	// 最新の図形が消える場合
	if (selectS->GetVertex_count() == 1) {
		CShape* tmpShape = shape_tail->GetPreShape();
		delete shape_tail;
		shape_tail = tmpShape;
		if (shape_tail == NULL) shape_head = NULL;
		return true;
	}
	
	// vertex_headとvertex_tailを消す場合
	if (selectS->GetCloseFlag() && selectS->isVertexCoordinate(selectS->GetVertexHead(), DeleteV))
		headTailFlag = true;
	
	selectS->freeVertex(DeleteV);
	selectS->SetCloseFlag(true);

	if (headTailFlag) 
		selectS->GetVertexTail()->SetVertex(selectS->GetVertexHead()->GetX(), selectS->GetVertexHead()->GetY());

	return true;
}

void CAdminControl::DeleteVertex() {
	// 例外処理
	if(!selectVertex) return;
	CShape* selectS = isVertexInShape(selectVertex);
	if(!shape_tail) return;
	if(selectS->GetCloseFlag() && selectS->GetVertex_count() == 4) {
		EditReset();
		return;
	}

	// 無理だった時用
	CVertex* originV = new CVertex(selectVertex);
	CVertex* originalV_Pre = selectVertex->GetPreVertex();
	CVertex* originalV_Next = selectVertex->GetNextVertex();

	// 頂点がvertex_headを選択しているか
	bool headTailFlag = false;
	if(selectS->GetCloseFlag() && selectS->isVertexCoordinate(selectS->GetVertexHead(), selectVertex)) 
		headTailFlag = true;
	
	// 選択している場合はpreを移動
	if(headTailFlag) originalV_Pre = selectS->GetVertexTail()->GetPreVertex();

	// 問題判定用フラグ
	bool ErrFlag = false;

	// 一旦削除を実行
	freeVertex(selectVertex);

	// 図形がないなら終了
	if(!shape_tail || shape_tail->GetVertex_count() <= 1) {
		delete originV;
		EditReset();
		return;
	}


	// 交差判定
	ErrFlag = selectS->IsSelfCrossingByDelete(originalV_Pre, originalV_Next, shape_head);

	 //内包判定
	if(!ErrFlag) ErrFlag = isContainsShapeByMoving();

	// --- 問題があった場合、元の状態に戻す ---
	if(ErrFlag) {
		selectS->InsertVertex(originV, originalV_Pre);
		if(headTailFlag) {
			selectS->GetVertexTail()->SetVertex(selectS->GetVertexHead()->GetX(), selectS->GetVertexHead()->GetY());
		}
	} else {
		// 問題なく削除成功した場合は復元用データを破棄
		delete originV;
	}

	EditReset();
}

bool CAdminControl::Select() {
	//EditReset();
	// 点の選択
	CVertex* selectV = SelectVertex(mouseVertex);
	if (selectV) {
		if(selectV != selectVertex) {
			EditReset();
			selectVertex = selectV;
		}
		else selectCount++;
		return true;
	}

	// 線の選択
	CVertex* selectLineStartV = SelectLine(mouseVertex);
	if (selectLineStartV) {
		if(selectLineStart != selectLineStartV) {
			EditReset();
			selectLineStart = selectLineStartV;
		}
		else selectCount++;

		if(selectCount > 1)InsertVertex();
		return true;
	}

	// 図形の選択
	CShape* selectS = SelectShape(mouseVertex);
	if (selectS) {
		if(selectS != selectShape) {
			EditReset();
			selectShape = selectS;
		}
		else selectCount++;
		return true;
	}

	EditReset();
	return false;
}

CVertex* CAdminControl::SelectVertex(CVertex* clickVertex) {
	// 定数定義
	CVertex* closedVertex = NULL;
	float closedDistance = INFINITY;
	CMath calc;

	for (CShape* currentShape = shape_head; currentShape!=NULL; currentShape = currentShape->GetNextShape()) {
		for (CVertex* currentVertex = currentShape->GetVertexHead(); currentVertex!=NULL; currentVertex = currentVertex->GetNextVertex()) {
			// 距離を算出
			float dis = calc.distanceVertex2Vertex(currentVertex, clickVertex);

			if (dis < closedDistance && dis < SELECT_THRESHOLD) {
				closedDistance = dis;
				closedVertex = currentVertex;
			}
		}
	}

	if (closedVertex) return closedVertex;
	return NULL;
}

CVertex* CAdminControl::SelectLine(CVertex* clickVertex) {
	// 定数定義
	CVertex* closedLineStartVertex = NULL;
	float closedDistance = INFINITY;
	CMath calc;

	for (CShape* currentShape = shape_head; currentShape != NULL; currentShape = currentShape->GetNextShape()) {
		for (CVertex* currentVertex = currentShape->GetVertexHead(); currentVertex->GetNextVertex() != NULL; currentVertex = currentVertex->GetNextVertex()) {
			// 距離を算出
			float dis = calc.distanceVertex2Line(clickVertex,currentVertex);

			if (dis < closedDistance && dis < SELECT_THRESHOLD && isInternalDivision(clickVertex,currentVertex)) {
				closedDistance = dis;
				closedLineStartVertex = currentVertex;
			}
		}
	}

	if (closedLineStartVertex) return closedLineStartVertex;
	return NULL;
}

CShape* CAdminControl::SelectShape(CVertex* clickVertex) {
	// 定数定義
	CMath calc;

	for (CShape* currentShape = shape_head; currentShape != NULL; currentShape = currentShape->GetNextShape()) {
		if (isContainsVertexInShape(clickVertex,currentShape)) return currentShape;
	}
	
	return NULL;
}

bool CAdminControl::isOtherCrossing(CVertex* newVertex) {
	// 例外処理
	if (!shape_head->GetNextShape())return false;
	CShape* newShape = isVertexInShape(newVertex);
	if (shape_tail->GetVertex_count() <= 1) return false;

	CShape* newVertexShape = shape_tail;

	for (CShape* currentShape = shape_head; currentShape != NULL; currentShape = currentShape->GetNextShape()) {

		// 自分の図形は例外
		if (currentShape == newVertexShape) continue;

		for (CVertex* currentVertex = currentShape->GetVertexHead(); currentVertex != NULL; currentVertex = currentVertex->GetNextVertex()) {
			if (currentShape->IsCrossing2Lines(currentVertex, currentVertex->GetNextVertex(), newShape->GetVertexTail()->GetPreVertex(), newVertex))
				return true;
		}
	}
	return false;
}

bool CAdminControl::isOtherCrossingByMoving() {
	// 例外処理
	if(!selectShape) return false;

	// 移動中の図形の各線分（v1-v2）を取得
	for(CVertex* v1 = selectShape->GetVertexHead(); v1 != NULL && v1->GetNextVertex() != NULL; v1 = v1->GetNextVertex()) {
		CVertex* v2 = v1->GetNextVertex();

		for(CShape* otherShape = shape_head; otherShape != NULL; otherShape = otherShape->GetNextShape()) {
			if(otherShape == selectShape) continue;

			// 他の図形の各線分（otherV1-otherV2）を取得
			for(CVertex* otherV1 = otherShape->GetVertexHead(); otherV1 != NULL && otherV1->GetNextVertex() != NULL; otherV1 = otherV1->GetNextVertex()) {
				CVertex* otherV2 = otherV1->GetNextVertex();

				// 2本の線分が交差しているか判定
				if(otherShape->IsCrossing2Lines(v1, v2, otherV1, otherV2)) return true;
			}
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

	for (CShape* currentShape = shape_head;currentShape != NULL;currentShape = currentShape->GetNextShape()) {
		if (currentShape == shape_tail) continue;
		if (isContainsVertexInShape(newVertex, currentShape)) return true;
	}
	return false;
}

bool CAdminControl::isContainsShape() {
	// 例外処理
	if(!shape_tail->GetCloseFlag()) return false;
	
	for(CShape* currentShape = shape_head;currentShape != NULL;currentShape = currentShape->GetNextShape()) {
		if (currentShape == shape_tail) continue;
		if (isContainsVertexInShape(currentShape->GetVertexHead(), shape_tail)) return true;
	}
	return false;
}

bool CAdminControl::isContainsShapeByMoving() {

	for (CShape* currentShape = shape_head;currentShape != NULL;currentShape = currentShape->GetNextShape()) {
		for (CVertex* currentVertex = currentShape->GetVertexHead();currentVertex != NULL;currentVertex = currentVertex->GetNextVertex()) {
			for (CShape* searchShape = shape_head;searchShape != NULL;searchShape = searchShape->GetNextShape()) {
				if (currentShape == searchShape)continue;
				if (isContainsVertexInShape(currentVertex, searchShape)) return true;
			}
		}
		
	}
	return false;
}

bool CAdminControl::isContainsVertexInShape(CVertex* clickVertex, CShape* serchShape) {
	CMath calc;
	float angle;

	angle = calc.angle(serchShape, clickVertex);

	if (calc.GetPie() * 2 * 0.99 <= angle && angle <= calc.GetPie() * 2 * 1.01) return true;

	return false;
}

bool CAdminControl::isInternalDivision(CVertex* vertex, CVertex* lineStartVertex) {

	/*- 判定方法1：両端の頂点からベクトルを2つ作り，なす角から判定する -*/
	// 学部時代ではこっちの方法．両端のなす角が90度以下なら内分点と判定する
	//CVector ab(lineStartVertex, lineStartVertex->GetNextVertex());
	//CVector ap(lineStartVertex, vertex);
	//CMath calc;
	//float StartAngle = calc.angle2Line(ab, ap);
	//
	//CVector ba(lineStartVertex->GetNextVertex(), lineStartVertex);
	//CVector bp(lineStartVertex->GetNextVertex(), vertex);
	//float EndAngle = calc.angle2Line(ba, bp);

	//if (StartAngle < calc.GetPie() / 2 && EndAngle < calc.GetPie() / 2) return true;
	//return false;

	/*- 判定方法2：パラメータtによる判定 -*/
	// 線分ABを A+tAB とし，tの割合に応じて内分点か判定する
	CVector ab(lineStartVertex, lineStartVertex->GetNextVertex());
	CVector ap(lineStartVertex, vertex);
	CMath calc;

	float t = calc.projectionT(ab, ap);

	// t=0なら始点，t=1なら終点，0<t<1なら内分点となる
	if (0.0f <= t && t <= 1.0f) return true;
	return false;
}

bool CAdminControl::isOverlapVertex(CVertex* new_vertex) {
	for(CShape* currentS = shape_head;currentS != NULL;currentS = currentS->GetNextShape()) {
		for(CVertex* currentV = currentS->GetVertexHead();currentV != NULL;currentV = currentV->GetNextVertex()) {
			if(currentS->isVertexCoordinate(currentV, new_vertex))return true;
		}
	}
	return false;
}

void CAdminControl::copyShape(CShape* baseShape, CShape* cpShape) {
	// 例外処理
	if(!baseShape || !cpShape) return;

	for(CVertex* currentVertex = baseShape->GetVertexHead();currentVertex != NULL;currentVertex = currentVertex->GetNextVertex()) {
		cpShape->appentVertex(currentVertex);
	}
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

