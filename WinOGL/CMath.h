#pragma once
#include "pch.h"
#include"CVertex.h"
#include"CVector.h"
#include <math.h>

class CMath {
public:
	
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
};

