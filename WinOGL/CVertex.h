#pragma once
#include "pch.h"

class CVertex {
#pragma region 変数
	float x;
	float y;
	CVertex* next;
	CVertex* pre;

#pragma endregion

#pragma region コンストラクタ・デストラクタ
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CVertex();

	/// <summary>
	/// コンストラクタ(引数)
	/// </summary>
	/// <param name="mouse_x">マウス：x座標</param>
	/// <param name="mouse_y">マウス：y座標</param>
	CVertex(float mouse_x, float mouse_y);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CVertex();

#pragma endregion

#pragma region Set
public:

	/// <summary>
	/// マウス座標を頂点座標にSetする
	/// </summary>
	/// <param name="mouse_x">マウス：x座標</param>
	/// <param name="mouse_y">マウス：y座標</param>
	void SetVertex(float mouse_x, float mouse_y);

	/// <summary>
	/// 次の座標をSetする
	/// </summary>
	/// <param name="nextVertex">アドレス</param>
	void SetNextVertex(CVertex* nextVertex);

	/// <summary>
	/// 前の座標をSetする
	/// </summary>
	/// <param name="preVertex">アドレス</param>
	void SetPreVertex(CVertex* preVertex);

#pragma endregion

#pragma region Get

	/// <summary>
	/// 頂点座標(X)を返す
	/// </summary>
	/// <returns>float : x座標</returns>
	float GetX();

	/// <summary>
	/// 頂点座標(Y)を返す
	/// </summary>
	/// <returns>float : x座標</returns>
	float GetY();

	/// <summary>
	/// 次の頂点アドレスを返す
	/// </summary>
	/// <returns>アドレス</returns>
	CVertex* GetNextVertex();

	/// <summary>
	/// 前の頂点アドレスを返す
	/// </summary>
	/// <returns>アドレス</returns>
	CVertex* GetPreVertex();

#pragma endregion

};

