#include "FindPath.h"
#include "Useful.h"
#include <cmath>
#include <cstring>



bool FindPath::find(Vector3D& source,
                  Vector3D& destination,
                  SPolygon* polygons,
                  unsigned polygonCount,
                  Vector3D* additionalPathPoints,
                  unsigned additionalPathPointsCount)
{
    path.destroy();
    debugPoints.destroy();
    polyIndexesWherePointsBelong.destroy();
    deleteGraph();


    if (!isPointReachable(source, destination, 16.f, polygons, polygonCount))
    {
        bool isPointOutsideMainPoly  = isPointOutsidePolygon(destination, &polygons[0], 0);
        bool isPointInsideOtherPolys = false;

        for(unsigned i = 1; i < polygonCount; ++i)
        {
            if (!isPointOutsidePolygon(destination, &polygons[i], i))
            {
                printf("destination is inside polygon %u\n", i);
                isPointInsideOtherPolys = true;
            }
        }
        
        if ( isPointOutsideMainPoly
            || isPointInsideOtherPolys
            || CollisionCirclePolygon(destination, 16.f, polygons, polygonCount))
        {
            destination = findBetterDestination(destination, polygons, polygonCount);
        }

        /*if (CollisionCirclePolygon(destination, 16.f, polygons, polygonCount))
        {
            return false;
        }*/

        makeGraph(source, destination, polygons, polygonCount, additionalPathPoints, additionalPathPointsCount);

        if (checkLinks(nullptr))
        {
            return true;
        }

        path.destroy();
        return false;
    }

   
    path.add(Vector3D(source.x, source.y,0));
    path.add(Vector3D(destination.x, destination.y, 0));
    return true;
}

bool FindPath::checkLinks(Link* link)
{
    Node* currentNode = graph[0];

    if (link != nullptr)
    {
        currentNode = link->linkedNode;
    }

    path.add(currentNode->pos);

    if (currentNode == graph[graph.count() - 1])
    {
        return true;
    }

    bool found = false;

    while (!found)
    {

        float shortest = 999999.f;
        int shortestLink = -1;

        for (unsigned i = 0; i < currentNode->links.count(); ++i)
        {
            Link* link = &currentNode->links[i];

            if (link->distanceToGoal < shortest && !link->visited)
            {
                shortest = currentNode->links[i].distanceToGoal;
                shortestLink = i;
            }
        }
        //printf("checked %lu links\n", currentNode->links.count());

        if (shortestLink != -1)
        {
            currentNode->links[shortestLink].visited = true;
            //printf("jumping to node %d\n", shortestLink);

            found = checkLinks(&currentNode->links[shortestLink]);

            if (found)
            {
                return true;
            }

        }
        else
        {
            path.remove(path.count() - 1);
            //printf("No more links :( going back?\n");
            return false;
        }
    }

    return true;
    
}

void FindPath::destroy(){

    deleteGraph();

    debugPoints.destroy();
    polyIndexesWherePointsBelong.destroy();
    collidedEdges.destroy();
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

unsigned FindPath::getNodeCount()
{
    return graph.count();
}

Vector3D* FindPath::getNodePos(unsigned index)
{
    if (index < graph.count())
    {
        return &(graph[index]->pos);
    }

    return nullptr;

}

unsigned FindPath::getDebugPointsCount()
{
    return debugPoints.count();
}

Vector3D* FindPath::getDebugPoint(unsigned index)
{
    if (index < debugPoints.count())
    {
        return &debugPoints[index];
    }

    return nullptr;
}


void FindPath::makeGraph(Vector3D& source,
                               Vector3D& destination,
                               SPolygon* polygons,
                               unsigned polygonCount,
                               Vector3D* additionalPathPoints,
                               unsigned additionalPathPointsCount)
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

                Vector3D normalizedNorm = norm;

                norm.x *= 32;
                norm.y *= 32;

                Vector3D nodePosition = polygons[i].points[j] + norm;

                bool rayColides = false;

                if (!isPointVisible(polygons[i].points[j], nodePosition, polygons, polygonCount))
                {
                    
                    rayColides = true;
                }

                if (CollisionCirclePolygon(polygons[i].points[j] + norm, 16.f, polygons, polygonCount)
                    || rayColides)
                {
                    norm = normalizedNorm;
                    norm.x *= 18;
                    norm.y *= 18;
                }

                if (!CollisionCirclePolygon(polygons[i].points[j] + norm, 16.f, polygons, polygonCount))
                {
                    Node* n = new Node(polygons[i].points[j] + norm);
                    graph.add(n);
                }
            }

        }
    }

    for (unsigned i = 0; i < additionalPathPointsCount; ++i)
    {
        Node* n = new Node(additionalPathPoints[i]);
        graph.add(n);
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

bool FindPath::isPointOutsidePolygon(Vector3D& destination,
                                     SPolygon* polygon,
                                     unsigned polygonIndex,
                                     bool justCheck)
{
    int collisionCount = 0;


    if (polygon->points.count() > 2)
    {
        for (unsigned i = 1; i < polygon->points.count(); ++i)
        {
            Vector3D* p2 = &polygon->points[i];
            Vector3D* p1 = &polygon->points[i - 1];

            Vector3D start0(destination.x, -10, 0);

            if (collisionLineEdge(p1, p2, &start0, &destination, !justCheck))
            {
                ++collisionCount;

                if (!justCheck)
                {
                    polyIndexesWherePointsBelong.add(polygonIndex);
                }
            }

            if (!justCheck)
            {
                Vector3D start1(destination.x, 99000, 0);

                if (collisionLineEdge(p1, p2, &start1, &destination))
                {
                    polyIndexesWherePointsBelong.add(polygonIndex);
                }

                Vector3D start2(-10, destination.y, 0);

                if (collisionLineEdge(p1, p2, &start2, &destination))
                {
                    polyIndexesWherePointsBelong.add(polygonIndex);
                }

                Vector3D start3(99999, destination.y, 0);

                if (collisionLineEdge(p1, p2, &start3, &destination))
                {
                    polyIndexesWherePointsBelong.add(polygonIndex);
                }
            }
        }
    }

    if (collisionCount < 1 || collisionCount % 2 == 0)//even count of hits -> point is ouside of the polygon
    {
        return true;
    }

    return false;
}

Vector3D FindPath::findBetterDestination(Vector3D& destination,
                                          SPolygon* polygons,
                                          unsigned polygonCount)
{
    float shortest = 999999;
    int nearestPointIndex = -1;
    int indexOfAIndex = 10;

    DArray<PointIndex> collisionPointIndexes;

    for (unsigned i = 0; i < debugPoints.count(); ++i)
    {
        PointIndex pi;
        pi.removed = false;
        pi.pointIndex = i;
        collisionPointIndexes.add(pi);
    }

    while (indexOfAIndex != -1)
    {

        indexOfAIndex = -1;
        shortest = 999999;
        nearestPointIndex = -1;

        for (unsigned i = 0; i < collisionPointIndexes.count(); ++i)
        {
            Vector3D v = debugPoints[collisionPointIndexes[i].pointIndex] - destination;

            if (v.length() < shortest && !collisionPointIndexes[i].removed)
            {
                nearestPointIndex = collisionPointIndexes[i].pointIndex;
                indexOfAIndex = i;
                shortest = v.length();
            }
        }

        if (nearestPointIndex != -1)
        {
            float shiftDirection = -1; //move inside

            if (polyIndexesWherePointsBelong[nearestPointIndex] > 0)
            {
                shiftDirection = 1; //move outside
            }


            CollisionCirclePolygon(debugPoints[nearestPointIndex], 16.f,
                    polygons,
                    polygonCount,
                    true);

            if (collidedEdges.count() > 1)
            {
                Vector3D vShift;

                for (unsigned i = 1; i < collidedEdges.count(); i += 2)
                {

                    Vector3D v = collidedEdges[i] - collidedEdges[i - 1];

                    v.normalize();


                    if (collidedEdges.count() == 2 || shiftDirection > 0)
                    {
                        Vector3D vperp1(-v.y, v.x, 0);
                        Vector3D vperp2(v.y, -v.x, 0);
                        Vector3D vperp = vperp2 - vperp1;
                        vperp.normalize();
                        vShift = vperp;
                    }
                    else
                    {
                        if (i == 1)
                        {
                            vShift = v;
                        }
                        else
                        {
                            vShift = vShift - v;
                        }
                    }
                }


                float shiftSize = 18.f * shiftDirection;

                Vector3D newDestination = Vector3D(debugPoints[nearestPointIndex].x + vShift.x * shiftSize,
                                                   debugPoints[nearestPointIndex].y + vShift.y * shiftSize,
                                                   0);

                printf("let's validate new destination\n");

                bool isOutsideMainPolygon = isPointOutsidePolygon(newDestination, &polygons[0], 0, true);

                bool isInsideOtherPolys = false;

                for (unsigned i = 1; i < polygonCount; ++i)
                {
                    if (!isPointOutsidePolygon(newDestination, &polygons[i], i, true))
                    {
                        isInsideOtherPolys = true;
                    }
                }

                if (!CollisionCirclePolygon(newDestination, 16.f,
                                            polygons,
                                            polygonCount)
                        && !isOutsideMainPolygon && !isInsideOtherPolys)
                {
                    collisionPointIndexes.destroy();
                    return newDestination;
                }
                else
                {
                    collisionPointIndexes[indexOfAIndex].removed = true;
                }

            }

        }
    }

    collisionPointIndexes.destroy();

    return Vector3D(0,0,0);

}



bool FindPath::isPointReachable(Vector3D& source,
                                Vector3D& destination,
                                float radius,
                                SPolygon* polygons,
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
        SPolygon* polygons,
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

bool FindPath::isPointConcave(SPolygon* polygonlist,
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


bool FindPath::CollisionCirclePolygon(Vector3D circle, float radius,
                                SPolygon* polygons,
                                unsigned polygonCount,
                                bool checkAll
                                )
{
    collidedEdges.destroy();
    bool result = false;

    for (unsigned long i = 0; i < polygonCount; ++i)
    {
        SPolygon* poly = &polygons[i];

        if (poly->points.count() < 2)
        {
            continue;
        }


        for (unsigned long j = 1; j < poly->points.count(); ++j)
        {
            if (CollisionCircleLineSegment(poly->points[j - 1].x,
                                       poly->points[j - 1].y,
                                       poly->points[j].x,
                                       poly->points[j].y,
                                       circle.x, circle.y, radius))
            {
                result = true;

                if (!checkAll)
                {
                    return result;
                }

                collidedEdges.add(poly->points[j-1]);
                collidedEdges.add(poly->points[j]);
            }
        }

    }


    return result;
            
}

bool FindPath::collisionLineEdge(Vector3D* p1, Vector3D* p2, Vector3D* d1, Vector3D* d2, bool addPoint)
{
    float r = 0.f;
    float s = 0.f;

    if (CollisionLineSegmentLineSegment(d1->x, d1->y, 
                d2->x, d2->y,
                p1->x, p1->y,
                p2->x, p2->y,
                &r, &s))
    {

        if (addPoint)
        {
            Vector3D segment = *p2 - *p1;
            float len = segment.length();
            float newLen = len * s;
            segment.normalize();
            Vector3D collisionPoint(p1->x + segment.x * newLen, p1->y + segment.y * newLen, 0);
            debugPoints.add(collisionPoint);
        }

        return true;

    }

    return false;

}

