#include "pch.h"
#include "CShape.h"

CShape::CShape() {
	vertex_head = NULL;
	vertex_tail = NULL;
	close_flag = false;
	next = NULL;
	pre = NULL;
	close_dis = 0.2;
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
		newVertex->SetVertex(vertex_head);
		close_flag = true;

		vertex_tail->SetNextVertex(newVertex);
		newVertex->SetPreVertex(vertex_tail);
		vertex_tail = newVertex;
		vertex_count++;
		return true;
	}
		
	vertex_tail->SetNextVertex(newVertex);
	newVertex->SetPreVertex(vertex_tail);
	vertex_tail = newVertex;
	vertex_count++;
	return true;	
}