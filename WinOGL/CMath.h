#pragma once
#include "pch.h"
#include "CVector.h"
#include <math.h>
class CShape;   // 前方宣言を追加
class CVertex;  // 前方宣言を追加

class CMath {
public:	
	/// <summary>
	/// 円周率を返す
	/// </summary>
	/// <returns>円周率</returns>
	float GetPie();

	/// <summary>
	/// 2点間の距離を算出する
	/// </summary>
	/// <param name="vertex1">頂点1</param>
	/// <param name="vertex2">頂点2</param>
	/// <returns>float:距離</returns>
	float distanceVertex2Vertex(CVertex* vertex1,CVertex* vertex2);

	/// <summary>
	/// 2つのベクトルから外積を算出する
	/// </summary>
	/// <param name="vector1">ベクトル1</param>
	/// <param name="vector2">ベクトル2</param>
	/// <returns>外積結果</returns>
	float crossProduct(CVector vector1, CVector vector2);

	/// <summary>
	/// 2つのベクトルから内積を算出する
	/// </summary>
	/// <param name="vector1">ベクトル1</param>
	/// <param name="vector2">ベクトル2</param>
	/// <returns>内積結果</returns>
	float innerProduct(CVector vector1, CVector vector2);

	/// <summary>
	/// 絶対値を返す
	/// </summary>
	/// <param name="num">数字</param>
	/// <returns>正の数字</returns>
	float abs(float num);

	/// <summary>
	/// 2線分のなす角度を算出する
	/// </summary>
	/// <param name="vector1">ベクトル1</param>
	/// <param name="vector2">ベクトル2</param>
	/// <returns>なす角度</returns>
	float angle2Line(CVector vector1, CVector vector2);

	/// <summary>
	/// 判定したい頂点と図形の各頂点の角度の総和を算出する
	/// </summary>
	/// <param name="shape">図形</param>
	/// <param name="baseVertex">頂点</param>
	/// <returns>角度の総和</returns>
	float angle(CShape* shape, CVertex* baseVertex);

};