#pragma once
#include "pch.h"
#include <gl/GL.h>
#include "CShape.h"
#include "CVertex.h"


class CAdminControl {
#pragma region 変数
	CShape* shape_head;
	CShape* shape_tail;

#pragma endregion

#pragma region コンストラクタ・デストラクタ
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CAdminControl();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CAdminControl();

#pragma endregion

#pragma region Draw
public:

	/// <summary>
	/// AC側のDraw全般を司る関数
	/// </summary>
	void Draw();

	/// <summary>
	/// 頂点を描画する
	/// </summary>
	/// <param name="currentV">頂点アドレス</param>
	/// <param name="R">赤</param>
	/// <param name="G">緑</param>
	/// <param name="B">青</param>
	/// <param name="size">頂点のサイズ</param>
	/// <param name="mode">描画モード</param>
	void DrawVertex(CVertex* currentV,float R,float G, float B,float size, char mode);

	/// <summary>
	/// 稜線を描画する
	/// </summary>
	/// <param name="currentV">頂点アドレス</param>
	/// <param name="R">赤</param>
	/// <param name="G">緑</param>
	/// <param name="B">青</param>
	/// <param name="size">頂点のサイズ</param>
	/// <param name="mode">描画モード</param>
	void DrawLine(CVertex* currentV, float R, float G, float B, float size, char mode);

#pragma endregion

#pragma region 頂点関係

	/// <summary>
	/// 頂点を追加する
	/// </summary>
	/// <param name="mouse_x">マウス：x座標</param>
	/// <param name="mouse_y">マウス：y座標</param>
	void AddVertex(float mouse_x, float mouse_y);
#pragma endregion

};

