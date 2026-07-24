#include "pch.h"
#include "CMath.h"

float CMath::distanceVertex2Vertex(CVertex* vertex1, CVertex* vertex2) {
    return sqrt(pow(vertex1->GetX() - vertex2->GetX(), 2) + pow(vertex1->GetY() - vertex2->GetY(), 2));;
}
