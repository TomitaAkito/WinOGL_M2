#pragma once
#include "pch.h"
#include <gl/GL.h>
#include "CShape.h"
#include "CVertex.h"


class CAdminControl {
#pragma region 変数
	CShape* shape_head;
	CShape* shape_tail;
	CVertex* mouseVertex;

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
	void DrawLine(CVertex* v1,CVertex* v2, float R, float G, float B, float size, char mode);

	/// <summary>
	/// 予測線を描画する
	/// </summary>
	void DrawForecastLine();

#pragma endregion

#pragma region Set
public:
	/// <summary>
	/// マウス座標を保存する
	/// </summary>
	/// <param name="mouse_x">float:マウスx座標</param>
	/// <param name="mouse_y">float:マウスy座標</param>
	void SetMouseVertex(float mouse_x, float mouse_y);
#pragma endregion

#pragma region 頂点関係

	/// <summary>
	/// 頂点を追加する
	/// </summary>
	/// <param name="mouse_x">マウス：x座標</param>
	/// <param name="mouse_y">マウス：y座標</param>
	void AddVertex(float mouse_x, float mouse_y);
#pragma endregion

#pragma region 判定

public:

	/// <summary>
	/// 他の図形と交差していないか(他交差)を判定する
	/// </summary>
	/// <param name="newVertex">新しい頂点</param>
	/// <returns>[true]交差している　[false]交差していない</returns>
	bool isOtherCrossing(CVertex* newVertex);

	/// <summary>
	/// 頂点が格納されている図形を返す
	/// </summary>
	/// <param name="vertex">頂点</param>
	/// <returns>図形のアドレス，なければNULL</returns>
	CShape* isVertexInShape(CVertex* vertex);

#pragma endregion

};

