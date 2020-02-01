#ifndef PATH_H
#define PATH_H

#include "Vectors.h"
#include "GameStuff/Polygon.h"

struct Node;

struct Link
{
    float distanceToGoal;
    bool  visited;
    Node* linkedNode;
};

struct Node
{
    Vector3D pos;
    DArray<Link> links;

    Node(Vector3D v)
    : pos(v)
    {
    }

};

struct PointIndex
{
    unsigned pointIndex;
    bool removed;
};


class FindPath{

public:
    
 
    FindPath(){}

    bool find(Vector3D& source,
                  Vector3D& destination,
                  SPolygon* polygons,
                  unsigned polygonCount,
                  Vector3D* additionalPathPoints,
                  unsigned additionalPathPointsCount);
    
    void destroy();

    unsigned getPathLength();
    unsigned getNodeCount();
    Vector3D* getPathStep(unsigned index);
    Vector3D* getNodePos(unsigned index);

    unsigned getDebugPointsCount();
    Vector3D* getDebugPoint(unsigned index);

private:

    void makeGraph(Vector3D& source,
                   Vector3D& destination,
                   SPolygon* polygons,
                   unsigned polygonCount,
                   Vector3D* additionalPathPoints,
                   unsigned additionalPathPointsCount);

    bool isPointOutsidePolygon(Vector3D& destination,
                               SPolygon* polygons,
                               unsigned polygonIndex,
                               bool justCheck = false
                               );

    Vector3D findBetterDestination(Vector3D& destination,
                                    SPolygon* polygons,
                                    unsigned polygonCount);

    bool isPointReachable(Vector3D& source,
                          Vector3D& destination,
                          float radius,
                          SPolygon* polygons,
                          unsigned polygonCount);

    bool isPointVisible(Vector3D& source,
                        Vector3D& destination,
                        SPolygon* polygons,
                        unsigned polygonCount);

    bool isPointConcave(SPolygon* polygonlist,
                         unsigned polygonIndex,
                         unsigned pointIndex);

    bool checkLinks(Link* link);

    void deleteGraph();

    bool CollisionCirclePolygon(Vector3D circle, float radius,
                                SPolygon* polygons,
                                unsigned polygonCount,
                                bool checkAll = false
                                );

    bool collisionLineEdge(Vector3D* p1, Vector3D* p2, Vector3D* d1, Vector3D* d2, bool addPoint = true);



private:
    DArray<Vector3D> path;

    //-----------
    DArray<Vector3D> debugPoints;
    DArray<unsigned> polyIndexesWherePointsBelong;
    //-----------

    DArray<Node*> graph;
    DArray<Vector3D> collidedEdges;

};

#endif //PATH_H
