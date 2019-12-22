#ifndef USEFULL_H
#define USEFULL_H
#include "Vectors.h"


int roundDouble2Int(double x);

bool CollisionCircleCircle(float x1, float y1, float radius1,
                             float x2, float y2, float radius2);

bool CollisionCircleRectangle(float cirleCenterX, float circleCenterY, float circleRadius,
                              float rectX, float rectY, float rectWidth, float rectHeight);

Vector3D MakeVector(float speedx, float speedy, float _angle );

Vector3D Vec2Reflection(Vector3D& vector, Vector3D& plane);

Vector3D Lerp(Vector3D org, Vector3D dest, float fProgress);

#endif// USEFULL_H
