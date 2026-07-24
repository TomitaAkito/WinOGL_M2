#include "pch.h"
#include "CShape.h"

CShape::CShape() {
	vertex_head = NULL;
	vertex_tail = NULL;
}


CShape::~CShape() {
}

CVertex* CShape::GetVertexHead() {
	return vertex_head;
}

CVertex* CShape::GetVertexTail() {
	return vertex_tail;
}

bool CShape::AddVertex(CVertex* newVertex) {
	if (!vertex_head) {
		vertex_head = newVertex;
		vertex_tail = newVertex;
		return true;
	}
	else {
		vertex_tail->SetNextVertex(newVertex);
		newVertex->SetPreVertex(vertex_tail);
		vertex_tail = newVertex;
		return true;
	}

	//return false;
}
