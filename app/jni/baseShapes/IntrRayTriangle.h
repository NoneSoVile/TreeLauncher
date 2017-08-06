#ifndef INTRRAYTRIANGLE_H
#define INTRRAYTRIANGLE_H


#include "Ray.h"
#include "Triangle.h"



class  IntrRayTriangle
 {
public:
    IntrRayTriangle (const Ray& ray,
        const Triangle& triangle);

    // Object access.
    const Ray& GetRay () const;
    const Triangle& GetTriangle () const;

    // Test-intersection query.
    virtual bool Test ();
    enum
    {
        IT_EMPTY,
        IT_POINT,
        IT_SEGMENT,
        IT_RAY,
        IT_LINE,
        IT_POLYGON,
        IT_PLANE,
        IT_POLYHEDRON,
        IT_OTHER
    };


private:
    int mIntersectionType;

    // The objects to intersect.
    const Ray* mRay;
    const Triangle* mTriangle;

    // Information about the intersection set.
    float mRayParameter, mTriBary0, mTriBary1, mTriBary2;
};




#endif
