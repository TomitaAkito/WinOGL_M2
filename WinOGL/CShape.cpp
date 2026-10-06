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

void CShape::SetCloseFlag(bool flag) {
	close_flag = flag;
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
		if (currentVertex==deleteVertex) {

			// 図形が閉じている場合
			if (isVertexCoordinate(vertex_head, vertex_tail)) {
				close_flag = false;
			}

			// headでありtailであるかどうか判定
			// ※処理が煩雑になるためbreak
			if(vertex_count == 1) {
				vertex_head = NULL;
				vertex_tail = NULL;
				vertex_count--;
				delete currentVertex;
				return true;
			}

			// ポインタ比較でHeadかどうかを判定
			if (vertex_head == currentVertex) {
				vertex_head = currentVertex->GetNextVertex();
				vertex_head->SetPreVertex(NULL);
			}
			// ポインタ比較でTailかどうかを判定
			if (vertex_tail == currentVertex) {
				vertex_tail = currentVertex->GetPreVertex();
				vertex_tail->SetNextVertex(NULL);
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

bool CShape::InsertVertex(CVertex* newVertex, CVertex* preVertex) {
	if (!newVertex) return false;

	if (!preVertex) {
		newVertex->SetNextVertex(this->vertex_head);
		newVertex->SetPreVertex(nullptr);

		if (this->vertex_head) {
			this->vertex_head->SetPreVertex(newVertex);
		}
		else {
			// 頂点が1つもない状態から追加した場合は、Tailにもなる
			this->vertex_tail = newVertex;
		}
		this->vertex_head = newVertex;
	}
	else {
		newVertex->SetNextVertex(preVertex->GetNextVertex());
		newVertex->SetPreVertex(preVertex);

		preVertex->SetNextVertex(newVertex);

		if (newVertex->GetNextVertex()) newVertex->GetNextVertex()->SetPreVertex(newVertex);
		else this->vertex_tail = newVertex;
	}

	vertex_count++;
	return true;
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

//bool CShape::IsSelfCrossingByMoving(CVertex* selectVertex) {
//	// 例外処理
//	if (vertex_count < 3) return false;
//
//	for (CVertex* currentV = vertex_head; currentV != NULL; currentV = currentV->GetNextVertex()) {
//
//		if (!currentV->GetNextVertex())
//			return false;
//
//		if(selectVertex->GetNextVertex() && IsCrossing2Lines(currentV, currentV->GetNextVertex(), selectVertex, selectVertex->GetNextVertex()))
//			return true;
//
//		if (selectVertex->GetPreVertex() && IsCrossing2Lines(currentV, currentV->GetNextVertex(), selectVertex->GetPreVertex(), selectVertex))
//			return true;
//	}
//
//	return false;
//}

bool CShape::IsSelfCrossingByMoving(CVertex* selectVertex, CShape* shape_head) {
	// 例外処理
	if (vertex_count < 2) return false;

	// 図形が閉じており，headかtailを選んでいる場合も取得
	CVertex* preV = selectVertex->GetPreVertex();
	if (!preV && close_flag && isVertexCoordinate(selectVertex, vertex_head))
		preV = vertex_tail->GetPreVertex();

	CVertex* nextV = selectVertex->GetNextVertex();
	if (!nextV && close_flag && isVertexCoordinate(selectVertex, vertex_tail))
		nextV = vertex_head->GetNextVertex();

	for (CShape* currentS = shape_head; currentS != NULL; currentS = currentS->GetNextShape()) {
		for (CVertex* currentV = currentS->GetVertexHead(); currentV != NULL; currentV = currentV->GetNextVertex()) {

			CVertex* currentNextV = currentV->GetNextVertex();
			if (!currentNextV) break;

			// 選択した頂点 -> next
			if (nextV) {
				bool isSharedNodeNext = (
					isVertexCoordinate(selectVertex, currentV) ||
					isVertexCoordinate(selectVertex, currentNextV) ||
					isVertexCoordinate(nextV, currentV) ||
					isVertexCoordinate(nextV, currentNextV));

				if (!isSharedNodeNext && IsCrossing2Lines(currentV, currentNextV, selectVertex, nextV)) return true;
			}

			// preV -> 選択した頂点
			if (preV) {
				bool isSharedNodePre = (
					isVertexCoordinate(preV, currentV) ||
					isVertexCoordinate(preV, currentNextV) ||
					isVertexCoordinate(selectVertex, currentV) ||
					isVertexCoordinate(selectVertex, currentNextV));

				if (!isSharedNodePre && IsCrossing2Lines(currentV, currentNextV, preV, selectVertex)) return true;
			}
		}
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

	//// 座標が一致しているか
	if (v1->GetX() == v2->GetX() && v1->GetY() == v2->GetY())
		return true;

	return false;
}
