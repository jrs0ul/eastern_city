#include "FindPath.h"
#include "Useful.h"
#include <cmath>
#include <cstring>



bool FindPath::find(Vector3D& source,
                  Vector3D& destination,
                  Polygon* polygons,
                  unsigned polygonCount)
{
    path.destroy();
    deleteGraph();


    if (!isPointReachable(source, destination, 16.f, polygons, polygonCount))
    {

        makeGraph(source, destination, polygons, polygonCount);

               

        Node* currentNode = graph[0];

        float shortest = 999999.f;
        int shortestLink = 2;

        while (shortestLink != -1)
        {
            shortestLink = -1;

            path.add(currentNode->pos);

            for (unsigned i = 0; i < currentNode->links.count(); ++i)
            {
                if (currentNode->links[i].distanceToGoal < shortest)
                {
                    shortest = currentNode->links[i].distanceToGoal;
                    shortestLink = i;
                }
            }

            if (shortestLink != -1)
            {
                currentNode = currentNode->links[shortestLink].linkedNode;
            }
        }


        return false;
    }

   
    path.add(Vector3D(source.x, source.y,0));
    path.add(Vector3D(destination.x, destination.y, 0));
    return true;
}

void FindPath::destroy(){

    deleteGraph();
    path.destroy(); 
}

unsigned FindPath::getPathLength()
{
    return path.count();
}

Vector3D* FindPath::getPathStep(unsigned index)
{
    if (index < path.count())
    {
        return &path[index];
    }

    return nullptr;
}

void FindPath::makeGraph(Vector3D& source,
                               Vector3D& destination,
                               Polygon* polygons,
                               unsigned polygonCount)
{
    Node* start = new Node(source);
    graph.add(start);

    for (unsigned i = 0; i < polygonCount; ++i)
    {
        if (polygons[i].points.count() < 2)
        {
            continue;
        }

        for (unsigned j = 0; j < polygons[i].points.count() - 1; ++j)
        {
            if (isPointConcave(polygons, i, j))
            {
                Vector3D next = polygons[i].points[(j + 1 < polygons[i].points.count() - 1) ? j + 1 : 0];
                Vector3D previous = polygons[i].points[j == 0 ? polygons[i].points.count() - 2 : j - 1];

                Vector3D first = polygons[i].points[j] - previous;
                Vector3D second = polygons[i].points[j] - next;
                Vector3D norm = first + second;
                norm.normalize();

                norm.x *= 32;
                norm.y *= 32;


                Node* n = new Node(polygons[i].points[j] + norm);
                graph.add(n);
            }

        }
    }

    Node* end = new Node(destination);
    graph.add(end);

    for (unsigned i = 0; i < graph.count(); ++i)
    {
        for (unsigned j = 0; j < graph.count(); ++j)
        {
            if ( i != j && isPointReachable(graph[i]->pos, graph[j]->pos, 16.f, polygons, polygonCount))
            {
                Link l;
                l.linkedNode = graph[j];
                l.visited = false;
                Vector3D dist = destination - graph[j]->pos;
                l.distanceToGoal = dist.length();

                graph[i]->links.add(l);
            }
        }
    }

}

bool FindPath::isPointReachable(Vector3D& source,
                                Vector3D& destination,
                                float radius,
                                Polygon* polygons,
                                unsigned polygonCount)
{
    Vector3D ray = destination - source;

    Vector3D r1(-ray.y, ray.x, 0);
    Vector3D r2(ray.y, -ray.x, 0);
    Vector3D pRay = r2 - r1;
    pRay.normalize();

    Vector3D posRight(pRay.x * radius, pRay.y * radius, 0);
    Vector3D posLeft(pRay.x * -radius, pRay.y * -radius, 0);
    Vector3D ray2Start = source + posRight;
    Vector3D ray3Start = source + posLeft;

    float rayLen = ray.length();
    ray.normalize();

    Vector3D rayL(ray.x * rayLen, ray.y * rayLen, 0);

    Vector3D ray2End = ray2Start + rayL;
    Vector3D ray3End = ray3Start + rayL;


    if (   isPointVisible(source, destination, polygons, polygonCount)
            && isPointVisible(ray2Start, ray2End, polygons, polygonCount)
            && isPointVisible(ray3Start, ray3End, polygons, polygonCount)
       )
    {
        return true;
    }

    return false;

}

bool FindPath::isPointVisible(Vector3D& source,
        Vector3D& destination,
        Polygon* polygons,
        unsigned polygonCount)
{
    for (unsigned i = 0; i < polygonCount; ++i)
    {
        if (polygons[i].points.count() < 2)
        {
            continue;
        }

        for (unsigned j = 1; j < polygons[i].points.count(); ++j)
        {
            Vector3D* p1 = &(polygons[i].points[j - 1]);
            Vector3D* p2 = &(polygons[i].points[j]);

            if (CollisionLineSegmentLineSegment(source.x, source.y, destination.x, destination.y,
                        p1->x, p1->y, p2->x, p2->y))
            {
                return false;
            }

        }

    }

    return true;
}

bool FindPath::isPointConcave(Polygon* polygonlist,
                              unsigned polygonIndex,
                              unsigned pointIndex)
{
    if (polygonIndex > 0)
    {
        return true;
    }

    if (polygonlist[0].points.count() < 3)
    {
        return true;
    }

    Vector3D current = polygonlist[0].points[pointIndex];
    unsigned nextPointIndex = (pointIndex + 1 < polygonlist[0].points.count() - 1) ? pointIndex + 1 : 0;
    Vector3D next = polygonlist[0].points[nextPointIndex];
    Vector3D previous = polygonlist[0].points[pointIndex == 0 ? polygonlist[0].points.count() - 2 : pointIndex - 1];

    Vector3D left(current.x - previous.x, current.y - previous.y, 0);
    Vector3D right(next.x - current.x, next.y - current.y, 0);

    float cross = (left.x * right.y) - (left.y * right.x);

    return cross < 0;
}

void FindPath::deleteGraph()
{
    for (unsigned i = 0; i < graph.count(); ++i)
    {
        graph[i]->links.destroy();
        delete graph[i];
    }

    graph.destroy();
}

