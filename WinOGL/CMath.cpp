#include "pch.h"
#include "CMath.h"
#include "CShape.h"
#include "CVertex.h"

float CMath::GetPie() {
    return 3.14;
}

float CMath::distanceVertex2Vertex(CVertex* vertex1, CVertex* vertex2) {
    return sqrt(pow(vertex1->GetX() - vertex2->GetX(), 2) + pow(vertex1->GetY() - vertex2->GetY(), 2));
}

float CMath::crossProduct(CVector vector1, CVector vector2) {
    return vector1.GetX() * vector2.GetY() - vector1.GetY() * vector2.GetX();
}

float CMath::innerProduct(CVector vector1, CVector vector2) {
    return vector1.GetX() * vector2.GetX() + vector1.GetY() * vector2.GetY();
}

float CMath::abs(float num) {
    if (num < 0) return -1 * num;
    else return num;
}

float CMath::angle2Line(CVector vector1, CVector vector2) {
    return atan2(abs(crossProduct(vector1, vector2)), innerProduct(vector1, vector2));
}

float CMath::angle(CShape* shape, CVertex* baseVertex) {
    float result = 0;

    for (CVertex* currentV = shape->GetVertexHead();currentV != shape->GetVertexTail();currentV = currentV->GetNextVertex()) {
        if (!currentV->GetNextVertex()) break;

        // アングル計算
        CVector vec1(baseVertex, currentV);
        CVector vec2(baseVertex, currentV->GetNextVertex());

        if(crossProduct(vec1, vec2) > 0)
            result += angle2Line(vec1, vec2);
        else
            result -= angle2Line(vec1, vec2);
    }
    return abs(result);
}
