#pragma once
#include "pch.h"
#include "CVertex.h"
#include "CMath.h"

class CShape {
#pragma region 変数
private: 
	CVertex* vertex_head;
	CVertex* vertex_tail;
	bool close_flag;
	CShape* next;
	CShape* pre;
	float close_dis;
	int vertex_count;

#pragma endregion

#pragma region コンストラクタ・デストラクタ
public:
	CShape();
	~CShape();

#pragma endregion

#pragma region Set
public:
	/// <summary>
	/// 次の図形を格納する
	/// </summary>
	/// <param name="nextShape">ポインタ</param>
	void SetNextShape(CShape* nextShape);

	/// <summary>
	/// 前の図形を格納する
	/// </summary>
	/// <param name="preShape">ポインタ</param>
	void SetPreShape(CShape* preShape);
#pragma endregion

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

	/// <summary>
	/// 次の図形のポインタを返す
	/// </summary>
	/// <returns>ポインタ</returns>
	CShape* GetNextShape();

	/// <summary>
	/// 前の図形のポインタを返す
	/// </summary>
	/// <returns>ポインタ</returns>
	CShape* GetPreShape();

	/// <summary>
	/// close_flagを返す
	/// </summary>
	/// <returns>[true]閉じている [false]開いている</returns>
	bool GetCloseFlag();

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

