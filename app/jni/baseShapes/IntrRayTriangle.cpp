
#include "IntrRayTriangle.h"
#include "../baseGraphics/Common.h"
using nv::dot;
using nv::vec3f;

IntrRayTriangle::IntrRayTriangle(const Ray& ray,
                 const Triangle& triangle)
    :
    mRay(&ray),
    mTriangle(&triangle)
{
}
const Ray& IntrRayTriangle::GetRay() const
{
    return *mRay;
}

const Triangle& IntrRayTriangle::GetTriangle() const
{
    return *mTriangle;
}

bool IntrRayTriangle::Test()
{
    // Compute the offset origin, edges, and normal.
    vec3f diff = mRay->Origin - mTriangle->V[0];
    vec3f edge1 = mTriangle->V[1] - mTriangle->V[0];
    vec3f edge2 = mTriangle->V[2] - mTriangle->V[0];
    vec3f normal = edge1.cross(edge2);

    // Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
    // E1 = kEdge1, E2 = kEdge2, N = Cross(E1,E2)) by
    //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
    //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
    //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
    float DdN = dot(mRay->Direction, normal);//  Dot(normal);
    float sign;
    if (DdN > ZERO_TOLERANCE)
    {
        sign = (float)1;
    }
    else if (DdN < -ZERO_TOLERANCE)
    {
        sign = (float)-1;
        DdN = -DdN;
    }
    else
    {
        // Ray and triangle are parallel, call it a "no intersection"
        // even if the ray does intersect.
        mIntersectionType = IT_EMPTY;
        return false;
    }

    float DdQxE2 = sign * dot(mRay->Direction, diff.cross(edge2));//sign*mRay->Direction.Dot(diff.Cross(edge2));
    if (DdQxE2 >= (float)0)
    {
        float DdE1xQ = sign * dot(mRay->Direction, diff.cross(edge1));
        if (DdE1xQ >= (float)0)
        {
            if (DdQxE2 + DdE1xQ <= DdN)
            {
                // Line intersects triangle, check if ray does.
                float QdN = -sign * dot(diff, normal);//-sign*diff.Dot(normal);
                if (QdN >= (float)0)
                {
                    // Ray intersects triangle.
                    mIntersectionType = IT_POINT;
                    return true;
                }
                // else: t < 0, no intersection
            }
            // else: b1+b2 > 1, no intersection
        }
        // else: b2 < 0, no intersection
    }
    // else: b1 < 0, no intersection

    mIntersectionType = IT_EMPTY;
    return false;
}


//----------------------------------------------------------------------------

