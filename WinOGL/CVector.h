#pragma once
#include"CVertex.h"

class CVector{
#pragma region 変数

private:
	float x;
	float y;

#pragma endregion

#pragma region コンストラクタ・デストラクタ
public:
	CVector();
	CVector(CVertex* v1, CVertex* v2);
	~CVector();

#pragma endregion

#pragma Set
public:
	
#pragma endregion
#pragma region Get
public:
	/// <summary>
	/// X軸方向のベクトルを返す
	/// </summary>
	/// <returns>float:X軸方向のベクトル</returns>
	float GetX();

	/// <summary>
	/// Y軸方向のベクトルを返す
	/// </summary>
	/// <returns>float:Y軸方向のベクトル</returns>
	float GetY();

};

