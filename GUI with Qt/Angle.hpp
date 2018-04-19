//
//  Angle.hpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/28.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef Angle_hpp
#define Angle_hpp

#include <iostream>
#include <cstdlib>
#include <cmath>

typedef enum enQuadrant
{
    Quadrant_First, Quadrant_Second, Quadrant_Third, Quadrant_Fourth,
    X_Axis_Positive, X_Axis_Negative, Y_Axis_Positive, Y_Axis_Negative
}Quadrant;

typedef enum enAngleType
{
    ANGLE_TYPE_DEGREE, ANGLE_TYPE_RADIANS
}angleType;

inline long mod(long a, long b) { return ( a % b + b ) % b; }

typedef struct stAngleWithDegree
{
protected:
    float angle;
    
public:
    stAngleWithDegree() { angle = 0.0; }
    stAngleWithDegree(float degree) { angle = degree; }
    
    float getDegree() { return angle; }
    float getRadians() { return angle * (M_PI / 180.0); }
    void setAsDegree(float degree) { angle = degree; }
    void setAsRadians(float radians) { angle = radians * (180.0 / M_PI); }
    
    Quadrant getQuadrant()
    {
        double degree = mod(angle, 360);
        
        if ( degree == 0.0 )
        {
            return X_Axis_Positive;
        }
        else if ( degree > 0.0 && degree < 90.0 )
        {
            return Quadrant_First;
        }
        else if ( degree == 90.0 )
        {
            return Y_Axis_Positive;
        }
        else if ( degree > 90.0 && degree < 180.0 )
        {
            return Quadrant_Second;
        }
        else if ( degree == 180.0 )
        {
            return X_Axis_Negative;
        }
        else if ( degree > 180.0 && degree < 270.0 )
        {
            return Quadrant_Third;
        }
        else if ( degree == 270.0 )
        {
            return Y_Axis_Negative;
        }
        else
        {
            return Quadrant_Fourth;
        }
    }
    
}AngleWithDegree, Degree;

#endif /* Angle_hpp */
