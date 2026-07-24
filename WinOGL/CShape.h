#pragma once
#include "pch.h"
#include "CVertex.h"

class CShape {
#pragma region 変数
private: 
	CVertex* vertex_head;
	CVertex* vertex_tail;

#pragma endregion

#pragma region コンストラクタ・デストラクタ
public:
	CShape();
	~CShape();

#pragma endregion

//#pragma region Set
//public:
//	
//#pragma endregion

#pragma region Get
public:
	/// <summary>
	/// vertex_headを返す
	/// </summary>
	/// <returns>ポインタ</returns>
	CVertex* GetVertexHead();


	/// <summary>
	/// vertex_tailを返す
	/// </summary>
	/// <returns>ポインタ</returns>
	CVertex* GetVertexTail();

#pragma endregion

#pragma region Vertex
public:
	/// <summary>
	/// 頂点を追加する．本図形に加えられるか判定を返す
	/// </summary>
	/// <param name="newVertex">加える座標</param>
	/// <returns>[True]座標追加完了　[False]座標追加不可</returns>
	bool AddVertex(CVertex* newVertex);

#pragma endregion
};

