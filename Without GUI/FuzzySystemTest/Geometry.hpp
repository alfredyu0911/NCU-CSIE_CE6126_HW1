//
//  Geometry.hpp
//  FuzzySystemTest
//
//  Created by Alfred Yu on 2018/4/9.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include <cstdio>
#include <string>
#include "AllConstant.hpp"

using namespace std;

#ifndef POINT
#define POINT
typedef struct stPoint
{
    float x, y;
    
    stPoint()
    {
        x = CAR_INIT_POSITION_X;
        y = CAR_INIT_POSITION_Y;
    }
    
    stPoint(float dx, float dy)
    {
        x = dx;
        y = dy;
    }
    
    stPoint(string sx, string sy)
    {
        x = stod(sx);
        y = stod(sy);
    }
}Point, Coordinate;
#endif

#ifndef VECTOR
#define VECTOR
typedef struct stVector
{
    float vx, vy;
    
    stVector(float x, float y)
    {
        vx = x;
        vy = y;
    }
    
    stVector(Point pt1, Point pt2)
    {
        vx = pt2.x - pt1.x;
        vy = pt2.y - pt1.y;
    }
}Vector;
#endif

#endif /* Geometry_hpp */
