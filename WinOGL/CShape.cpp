#include "pch.h"
#include "CShape.h"

CShape::CShape() {
	vertex_head = NULL;
	vertex_tail = NULL;
	close_flag = false;
	next = NULL;
	pre = NULL;
	close_dis = 0.05;
	vertex_count = 0;
}


CShape::~CShape() {
}

void CShape::SetNextShape(CShape* nextShape) {
	next = nextShape;
}

void CShape::SetPreShape(CShape* preShape) {
	pre = preShape;
}

CVertex* CShape::GetVertexHead() {
	return vertex_head;
}

CVertex* CShape::GetVertexTail() {
	return vertex_tail;
}

CShape* CShape::GetNextShape() {
	return next;
}

CShape* CShape::GetPreShape() {
	return pre;
}

bool CShape::GetCloseFlag() {
	return close_flag;
}

int CShape::GetVertex_count() {
	return vertex_count;
}

bool CShape::AddVertex(CVertex* newVertex) {
	// 例外処理
	// --形状を閉じている場合
	if (close_flag) return false;


	// 頂点がない場合
	if (!vertex_head) {
		vertex_head = newVertex;
		vertex_tail = newVertex;
		vertex_count++;
		return true;
	}

	//-------------頂点追加処理-------------

	CMath calc;
	// もし形状を閉じるのであれば
	if (vertex_count >= 3 && calc.distanceVertex2Vertex(newVertex, vertex_head) < close_dis) {
		
		// 自交差判定込み
		newVertex->SetVertex(vertex_head);

		if (IsSelfCrossing_SandglassType()) {
			delete newVertex;
			return false;
		}

		close_flag = true;

		vertex_tail->SetNextVertex(newVertex);
		newVertex->SetPreVertex(vertex_tail);
		vertex_tail = newVertex;
		vertex_count++;
		return true;
	}
	
	// 交差判定
	if (IsSelfCrossing(newVertex)) return false;

	vertex_tail->SetNextVertex(newVertex);
	newVertex->SetPreVertex(vertex_tail);
	vertex_tail = newVertex;
	vertex_count++;
	return true;	
}

bool CShape::freeVertex(CVertex* deleteVertex) {

	for (CVertex* currentVertex = vertex_tail; currentVertex != NULL; currentVertex = currentVertex->GetPreVertex()) {
		if (isVertexCoordinate(currentVertex, deleteVertex)) {

			// 図形が閉じている場合
			if (isVertexCoordinate(vertex_head, vertex_tail)) {
				close_flag = false;
			}

			// ポインタ比較でHeadかどうかを判定
			if (vertex_head == currentVertex) {
				vertex_head = currentVertex->GetNextVertex();
			}
			// ポインタ比較でTailかどうかを判定
			if (vertex_tail == currentVertex) {
				vertex_tail = currentVertex->GetPreVertex();
			}

			// 前の頂点が存在するなら、そのNextを次の頂点に繋ぐ
			if (currentVertex->GetPreVertex()) {
				currentVertex->GetPreVertex()->SetNextVertex(currentVertex->GetNextVertex());
			}

			// 次の頂点が存在するなら、そのPreを前の頂点に繋ぐ
			if (currentVertex->GetNextVertex()) {
				currentVertex->GetNextVertex()->SetPreVertex(currentVertex->GetPreVertex());
			}
			vertex_count--;
			delete currentVertex;
			return true;
		}
	}

	return false;
}

bool CShape::IsSelfCrossing(CVertex* newVertex) {
	// 例外処理
	if (vertex_count < 3) return false;

	for (CVertex* currentV = vertex_head; currentV != vertex_tail->GetPreVertex(); currentV = currentV->GetNextVertex()) {

		if (!currentV->GetNextVertex())
			return false;

		if(IsCrossing2Lines(currentV,currentV->GetNextVertex(),vertex_tail,newVertex))
			return true;
	}

	return false;
}

bool CShape::IsSelfCrossing_SandglassType() {
	// 例外処理
	if (vertex_count < 3) return false;

	for (CVertex* currentV = vertex_head->GetNextVertex(); currentV != vertex_tail->GetPreVertex(); currentV = currentV->GetNextVertex()) {

		if (!currentV->GetNextVertex())
			return false;

		if (IsCrossing2Lines(currentV, currentV->GetNextVertex(), vertex_tail, vertex_head))
			return true;
	}

	return false;
}

bool CShape::IsCrossing2Lines(CVertex* As, CVertex* Ae, CVertex* Bs, CVertex* Be) {
	// 例外処理
	if (!As || !Ae || !Bs || !Be) return false;
	
	CMath calc;

	// ベクトル
	CVector a(As, Ae);
	CVector a1(As, Bs);
	CVector a2(As, Be);
	CVector b(Bs, Be);
	CVector b1(Bs, As);
	CVector b2(Bs, Ae);

	// 外積の計算
	float ca1 = calc.crossProduct(a, a1);
	float ca2 = calc.crossProduct(a, a2);
	float cb1 = calc.crossProduct(b, b1);
	float cb2 = calc.crossProduct(b, b2);

	// 交差判定
	if ((ca1 * ca2 <= 0) && (cb1 * cb2 <= 0)) 
		return true;
	
	return false;
}

bool CShape::isVertexCoordinate(CVertex* v1, CVertex* v2) {

	// アドレスが一致しているか
	if (v1 == v2) return true;

	// 座標が一致しているか
	if (v1->GetX() == v2->GetX() && v1->GetY() == v2->GetY())
		return true;

	return false;
}
