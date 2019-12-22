#include "Usefull.h"
#include <cmath>
#include "Matrix.h"
 
 
int roundDouble2Int(double x){
    return int(x > 0.0 ? x + 0.5 : x - 0.5);
}
//-----------------------------------------
bool CollisionCircleCircle(float x1, float y1, float radius1,
                             float x2, float y2, float radius2){
 
    float difx = (float)fabs(x1-x2);
    float dify = (float)fabs(y1-y2);
 
    float distance = (float)sqrt(difx*difx+dify*dify);
    if (distance < radius1 + radius2)
        return true;
 
 
    return false;
}
//-------------------------------------------------------
bool CollisionCircleRectangle(float circleCenterX, float circleCenterY, float circleRadius,
                              float rectX, float rectY, float rectWidth, float rectHeight){
    
    float tmpX = circleCenterX;
    float tmpY = circleCenterY;

    if (tmpX > rectX + rectWidth)  tmpX = rectX + rectWidth;
    if (tmpX < rectX)              tmpX = rectX;
    if (tmpY > rectY + rectHeight) tmpY = rectY + rectHeight;
    if (tmpY < rectY)              tmpY = rectY;


    float difx = (float) fabs (tmpX - circleCenterX);
    float dify = (float) fabs (tmpY - circleCenterY);
    float distance = (float) sqrt (difx * difx + dify * dify);

    if (distance < circleRadius) return true;

    return false; 
}


//-------------------------------------------------------------------
Vector3D MakeVector(float speedx, float speedy, float _angle ){
    Matrix rot;
    Vector3D pn(speedx, speedy, 0);
    MatrixRotationAxis(_angle, Vector3D(0,0,-1), rot);
    pn.transform(rot);

    return pn;
}
//-------------------------------------------------------------------
Vector3D Vec2Reflection(Vector3D& vector, Vector3D& plane){
 
    Vector3D perp(plane.y, -plane.x, 0);
 
    float dot1 = perp * perp;
    float dot2 = perp * vector;
    Vector3D v1(vector.v[0] * dot1, vector.v[1] * dot1, vector.v[2] * dot1);
    dot2 = dot2 * 2;
    Vector3D v2(perp.v[0] * dot2, perp.v[1] * dot2, perp.v[2] * dot2); 

    Vector3D reflection = v1 - v2;
    reflection.normalize();
 
    return reflection;
}

Vector3D Lerp(Vector3D org, Vector3D dest, float fProgress)
{

    Vector3D result;

    result.v[0] = org.v[0] + fProgress * (dest.v[0] - org.v[0]);
    result.v[1] = org.v[1] + fProgress * (dest.v[1] - org.v[1]);
    result.v[2] = org.v[2] + fProgress * (dest.v[2] - org.v[2]);

    return result;

}

