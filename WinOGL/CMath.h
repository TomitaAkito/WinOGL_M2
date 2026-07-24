#pragma once
#include "pch.h"
#include"CVertex.h"
#include <math.h>

class CMath {
public:
	
	/// <summary>
	/// 2点間の距離を算出する
	/// </summary>
	/// <param name="vertex1">頂点1</param>
	/// <param name="vertex2">頂点2</param>
	/// <returns></returns>
	float distanceVertex2Vertex(CVertex* vertex1,CVertex* vertex2);
};

