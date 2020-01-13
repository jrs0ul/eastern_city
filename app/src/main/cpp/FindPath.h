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


class FindPath{

public:
    
 
    FindPath(){}

    bool find(Vector3D& source,
                  Vector3D& destination,
                  Polygon* polygons,
                  unsigned polygonCount);
    
    void destroy();

    unsigned getPathLength();
    Vector3D* getPathStep(unsigned index);

private:

    void makeGraph(Vector3D& source,
                   Vector3D& destination,
                   Polygon* polygons,
                   unsigned polygonCount);

    bool isPointReachable(Vector3D& source,
                          Vector3D& destination,
                          float radius,
                          Polygon* polygons,
                          unsigned polygonCount);

    bool isPointVisible(Vector3D& source,
                        Vector3D& destination,
                        Polygon* polygons,
                        unsigned polygonCount);

    bool isPointConcave(Polygon* polygonlist,
                         unsigned polygonIndex,
                         unsigned pointIndex);

    void deleteGraph();



private:
    DArray<Vector3D> path;
    DArray<Node*> graph;

};

#endif //PATH_H
