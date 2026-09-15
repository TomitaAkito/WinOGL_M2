#pragma once
#include "pch.h"
#include "CVertex.h"
#include "CVector.h"
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

	/// <summary>
	/// vertex_countを返す
	/// </summary>
	/// <returns>[int]頂点の個数</returns>
	int GetVertex_count();

#pragma endregion

#pragma region Vertex
public:
	/// <summary>
	/// 頂点を追加する．本図形に加えられるか判定を返す
	/// </summary>
	/// <param name="newVertex">加える座標</param>
	/// <returns>[True]座標追加完了　[False]座標追加不可</returns>
	bool AddVertex(CVertex* newVertex);

	/// <summary>
	/// 指定された頂点を削除する
	/// </summary>
	/// <param name="deleteVertex">削除対象の頂点</param>
	/// <returns>[True]削除完了　[False]削除不可</returns>
	bool freeVertex(CVertex* deleteVertex);

#pragma endregion

#pragma region 判定
public:

	/// <summary>
	/// 新しく追加する頂点が自交差するか判定する
	/// </summary>
	/// <param name="newVertex">加える座標</param>
	/// <returns>[True]自交差する　[False]自交差しない</returns>
	bool IsSelfCrossing(CVertex* newVertex);

	/// <summary>
	/// [砂時計型]新しく追加する頂点が自交差するか判定する
	/// </summary>
	/// <returns>[True]自交差する　[False]自交差しない</returns>
	bool IsSelfCrossing_SandglassType();


	/// <summary>
	/// 4つの頂点からなる線分が交差するか判定する
	/// </summary>
	/// <param name="As">線分Aの始点</param>
	/// <param name="Ae">線分Aの終点</param>
	/// <param name="Bs">線分Bの始点</param>
	/// <param name="Be">線分Bの終点</param>
	/// <returns>[True]交差する　[False]交差しない</returns>
	bool IsCrossing2Lines(CVertex* As, CVertex* Ae, CVertex* Bs, CVertex* Be);
	

	/// <summary>
	/// 二つの頂点が一致するか調査する
	/// </summary>
	/// <param name="v1">頂点1</param>
	/// <param name="v2">頂点2</param>
	/// <returns>[True]一致　[False]不一致</returns>
	bool isVertexCoordinate(CVertex* v1, CVertex* v2);

#pragma endregion
};

