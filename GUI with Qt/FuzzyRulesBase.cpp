//
//  FuzzyRulesBase.cpp
//  FuzzySystemTest
//
//  Created by Alfred Yu on 2018/4/4.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#include "FuzzyRulesBase.hpp"
#include "AllConstant.hpp"
#include "Geometry.hpp"

vector<FuzzyRulesResult> FuzzyRulesBase::searchAllRules(float X, float Y)
{
    FuzzyRulesResult res;
    vector<FuzzyRulesResult> vRes;
    vRes.clear();
    
    for ( int ConditionA=0 ; ConditionA < HD_enumEnd ; ConditionA++ )
    {
        float alphaA=0.0, alphaB=0.0;
        
        switch ( ConditionA )
        {
            case HD_left_very:
            {
                alphaA = horizontallyDeviate_left_very(X);
                break;
            }
            case HD_left_normal:
            {
                alphaA = horizontallyDeviate_left_normal(X);
                break;
            }
            case HD_left_little:
            {
                alphaA = horizontallyDeviate_left_little(X);
                break;
            }
            case HD_right_very:
            {
                alphaA = horizontallyDeviate_right_very(X);
                break;
            }
            case HD_right_normal:
            {
                alphaA = horizontallyDeviate_right_normal(X);
                break;
            }
            case HD_right_little:
            {
                alphaA = horizontallyDeviate_right_little(X);
                break;
            }
                
            default:
                break;
        }
        
        if ( alphaA == 0.0 )
            continue;
        
        for ( int ConditionB=0 ; ConditionB < VDR_enumEnd ; ConditionB++ )
        {
            switch ( ConditionB )
            {
                case VDR_large:
                {
                    alphaB = verticalDistanceRemain_large(Y);
                    break;
                }
                case VDR_normal:
                {
                    alphaB = verticalDistanceRemain_normal(Y);
                    break;
                }
                case VDR_critical:
                {
                    alphaB = verticalDistanceRemain_critical(Y);
                    break;
                }
                    
                default:
                    break;
            }
            
            if ( alphaB == 0.0 )
                continue;
            
            res.ConditionA_HD = (HorizontalDeviate)ConditionA;
            res.ConditionB_VDR = (VerticalDistanceRemain)ConditionB;
            res.alphaA = alphaA;
            res.alphaB = alphaB;
            vRes.push_back(res);
        }
    }
    
    return vRes;
}

vector<HorizontalDeviate> FuzzyRulesBase::search_if_XisA(float X)
{
    vector<HorizontalDeviate> vRes;
    vRes.clear();

    for ( int ConditionA=0 ; ConditionA < HD_enumEnd ; ConditionA++ )
    {
        float alphaA=0.0;

        switch ( ConditionA )
        {
            case HD_left_very:
            {
                alphaA = horizontallyDeviate_left_very(X);
                break;
            }
            case HD_left_normal:
            {
                alphaA = horizontallyDeviate_left_normal(X);
                break;
            }
            case HD_left_little:
            {
                alphaA = horizontallyDeviate_left_little(X);
                break;
            }
            case HD_right_very:
            {
                alphaA = horizontallyDeviate_right_very(X);
                break;
            }
            case HD_right_normal:
            {
                alphaA = horizontallyDeviate_right_normal(X);
                break;
            }
            case HD_right_little:
            {
                alphaA = horizontallyDeviate_right_little(X);
                break;
            }

            default:
                break;
        }

        if ( alphaA == 0.0 )
            continue;

        vRes.push_back((HorizontalDeviate)ConditionA);
    }
    return vRes;
}

vector<VerticalDistanceRemain> FuzzyRulesBase::search_if_YisB(float Y)
{
    vector<VerticalDistanceRemain> vRes;
    vRes.clear();

    for ( int ConditionB=0 ; ConditionB < VDR_enumEnd ; ConditionB++ )
    {
        float alphaB=0.0;

        switch ( ConditionB )
        {
            case VDR_large:
            {
                alphaB = verticalDistanceRemain_large(Y);
                break;
            }
            case VDR_normal:
            {
                alphaB = verticalDistanceRemain_normal(Y);
                break;
            }
            case VDR_critical:
            {
                alphaB = verticalDistanceRemain_critical(Y);
                break;
            }

            default:
                break;
        }

        if ( alphaB == 0.0 )
            continue;

        vRes.push_back((VerticalDistanceRemain)ConditionB);
    }
    return vRes;
}

WheelAngle FuzzyRulesBase::fuzzyRule_GetResultByCondition(HorizontalDeviate ConditionA, VerticalDistanceRemain ConditionB, LeftDistanceRemain ConditionL, RightDistanceRemain ConditionR)
{
    //  when LeftDistanceRemain == LDR_normal && RightDistanceRemain == RDR_normal
    //
    //                  HD_right_little     HD_right_normal     HD_right_very       HD_left_little      HD_left_normal      HD_left_very
    //
    //  VDR_large       WA_left_exLittle    WA_left_little      WA_left_medium      WA_right_exLittle   WA_right_little     WA_right_medium
    //
    //  VDR_normal      WA_left_little      WA_left_medium      WA_left_large       WA_right_little     WA_right_medium     WA_right_large
    //
    //  VDR_critical    WA_left_little      WA_left_medium      WA_left_large       WA_right_little     WA_right_medium     WA_right_large
    //
    //  --------------------------------------------------------------------------------------------------------------------------------------
    //
    //  when LeftDistanceRemain == LDR_critical && RightDistanceRemain == RDR_critical
    //
    //  VDR_critical    WA_left_medium      WA_left_large       WA_left_exLarge     WA_right_medium     WA_right_large      WA_right_exLarge
    //
    //  --------------------------------------------------------------------------------------------------------------------------------------

    WheelAngle RulesArray[3][6] =
    {
        {WA_left_exLittle, WA_left_little, WA_left_medium, WA_right_exLittle, WA_right_little, WA_right_medium},
        {WA_left_little, WA_left_medium, WA_left_large, WA_right_little, WA_right_medium, WA_right_large},
        {WA_left_little, WA_left_medium, WA_left_large, WA_right_little, WA_right_medium, WA_right_large}
    };

    if ( ConditionL == LDR_critical || ConditionR == RDR_critical )
    {
        RulesArray[2][0] = WA_left_medium;
        RulesArray[2][1] = WA_left_large;
        RulesArray[2][2] = WA_left_exLarge;
        RulesArray[2][3] = WA_right_medium;
        RulesArray[2][4] = WA_right_large;
        RulesArray[2][5] = WA_right_exLarge;
    }
    
    return RulesArray[ConditionB][ConditionA];
}

void FuzzyRulesBase::getMembershipFuncOfWheelAngle(WheelAngle WA, float &center, float &width_L, float &width_R)
{
    float gap = (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX) / WHEEL_ANGLE_GAP_NUMBER;
    float temp = (float)WA * gap;
    float width = gap / 2.0;
    
    width_L = width_R = width;
    center = (temp > 0.0) ? (temp - width) : (temp + width);
}

float FuzzyRulesBase::horizontallyDeviate_left_little(float distance_of_left_Subtract_right)
{
    float X = distance_of_left_Subtract_right;
    
    if ( X < -2.0 )
    {
        return 0.0;
    }
    else if ( X >= -2.0 && X < -0.0 )
    {
        // (-2,0) -> (-0,1)
        Point p1 = Point(-2.0, 0.0);
        Point p2 = Point(-0.0, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
//    else if ( X >= -3.0 && X < 0.0 )
//    {
//        return 1.0;
//    }
    else// if ( X >= 0.0 )
    {
        return 0.0;
    }
}

float FuzzyRulesBase::horizontallyDeviate_left_normal(float distance_of_left_Subtract_right)
{
    float X = distance_of_left_Subtract_right;
    
    if ( X < -5.0 )
    {
        return 0.0;
    }
    else if ( X >= -5.0 && X < -3.0 )
    {
        // (-5,0) -> (-3,1)
        Point p1 = Point(-5.0, 0.0);
        Point p2 = Point(-3.0, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else if ( X >= -3.0 && X < -1.0 )
    {
        // (-3,1) -> (-1,0)
        Point p1 = Point(-3.0, 1.0);
        Point p2 = Point(-1.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= -1.0 )
    {
        return 0.0;
    }
}

float FuzzyRulesBase::horizontallyDeviate_left_very(float distance_of_left_Subtract_right)
{
    float X = distance_of_left_Subtract_right;
    
    if ( X < -6.0 )
    {
        return 1.0;
    }
    else if ( X >= -6.0 && X < -3.0 )
    {
        // (-6,1) -> (-3,0)
        Point p1 = Point(-6.0, 1.0);
        Point p2 = Point(-3.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= -3.0 )
    {
        return 0.0;
    }
}

float FuzzyRulesBase::horizontallyDeviate_right_little(float distance_of_left_Subtract_right)
{
    float X = distance_of_left_Subtract_right;
    
    if ( X < 0.0 )
    {
        return 0.0;
    }
//    else if ( X >= 0.0 && X < 3.0 )
//    {
//        return 1.0;
//    }
    else if ( X >= 0.0 && X < 2.0 )
    {
        // (0,1) -> (2,0)
        Point p1 = Point(0.0, 1.0);
        Point p2 = Point(2.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 2.0 )
    {
        return 0.0;
    }
}

float FuzzyRulesBase::horizontallyDeviate_right_normal(float distance_of_left_Subtract_right)
{
    float X = distance_of_left_Subtract_right;
    
    if ( X < 1.0 )
    {
        return 0.0;
    }
    else if ( X >= 1.0 && X < 3.0 )
    {
        // (1,0) -> (3,1)
        Point p1 = Point(1.0, 0.0);
        Point p2 = Point(3.0, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else if ( X >= 3.0 && X < 5.0 )
    {
        // (3,1) -> (5,0)
        Point p1 = Point(3.0, 1.0);
        Point p2 = Point(5.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 5.0 )
    {
        return 0.0;
    }
}

float FuzzyRulesBase::horizontallyDeviate_right_very(float distance_of_left_Subtract_right)
{
    float X = distance_of_left_Subtract_right;
    
    if ( X < 3.0 )
    {
        return 0.0;
    }
    else if ( X >= 3.0 && X < 6.0 )
    {
        // (3,0) -> (6,1)
        Point p1 = Point(3.0, 0.0);
        Point p2 = Point(6.0, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 6.0 )
    {
        return 1.0;
    }
}

float FuzzyRulesBase::verticalDistanceRemain_critical(float distance)
{
    float X = distance;
    
    if ( X < 13.0 )
    {
        return 1.0;
    }
    else if ( X >= 13.0 && X < 18.0 )
    {
        // (13,1) -> (18,0)
        Point p1 = Point(13.0, 1.0);
        Point p2 = Point(18.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 18.0 )
    {
        return 0.0;
    }
}

float FuzzyRulesBase::verticalDistanceRemain_normal(float distance)
{
    float X = distance;
    
    if ( X < 16.0 )
    {
        return 0.0;
    }
    else if ( X >= 16.0 && X < 18.0 )
    {
        // (16,0) -> (18,1)
        Point p1 = Point(16.0, 0.0);
        Point p2 = Point(18.0, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else if ( X >= 18.0 && X < 20.0 )
    {
        return  1.0;
    }
    else if ( X >= 20.0 && X < 22.0 )
    {
        // (20,1) -> (22,0)
        Point p1 = Point(20.0, 1.0);
        Point p2 = Point(22.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 22.0 )
    {
        return 0.0;
    }
}

float FuzzyRulesBase::verticalDistanceRemain_large(float distance)
{
    float X = distance;
    
    if ( X < 20.0 )
    {
        return 0.0;
    }
    else if ( X >= 20.0 && X < 25.0 )
    {
        // (20,0) -> (25,1)
        Point p1 = Point(20.0, 0.0);
        Point p2 = Point(25.0, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 25.0 )
    {
        return 1.0;
    }
}

float FuzzyRulesBase::WheelAngle_left_exLarge(float degree)
{
    return WheelAngle_generalize(degree, -40, -32, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

float FuzzyRulesBase::WheelAngle_left_large(float degree)
{
    return WheelAngle_generalize(degree, -32, -24, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

float FuzzyRulesBase::WheelAngle_left_medium(float degree)
{
    return WheelAngle_generalize(degree, -24, -16, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

float FuzzyRulesBase::WheelAngle_left_little(float degree)
{
    return WheelAngle_generalize(degree, -16, -8, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

float FuzzyRulesBase::WheelAngle_left_exLittle(float degree)
{
    return WheelAngle_generalize(degree, -8, 0, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

float FuzzyRulesBase::WheelAngle_right_exLittle(float degree)
{
    return WheelAngle_generalize(degree, 0, 8, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

float FuzzyRulesBase::WheelAngle_right_little(float degree)
{
    return WheelAngle_generalize(degree, 8, 16, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

float FuzzyRulesBase::WheelAngle_right_medium(float degree)
{
    return WheelAngle_generalize(degree, 16, 24, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

float FuzzyRulesBase::WheelAngle_right_large(float degree)
{
    return WheelAngle_generalize(degree, 24, 32, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

float FuzzyRulesBase::WheelAngle_right_exLarge(float degree)
{
    return WheelAngle_generalize(degree, 32, 40, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

float FuzzyRulesBase::WheelAngle_generalize(float degree, float boundL, float boundR, float intervalLR)
{
    if ( degree < boundL - intervalLR )
    {
        return 0.0;
    }
    else if ( degree >= boundL - intervalLR && degree < boundL )
    {
        Point p1 = Point(boundL-intervalLR, 0.0);
        Point p2 = Point(boundL, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (degree - p1.x) + p1.y;
    }
    else if ( degree >= boundL && degree < boundR )
    {
        return 1.0;
    }
    else if ( degree >= boundR && degree < boundR + intervalLR )
    {
        Point p1 = Point(boundR, 1.0);
        Point p2 = Point(boundR+intervalLR, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (degree - p1.x) + p1.y;
    }
    else// if ( degree >= boundR + intervalLR )
    {
        return 0.0;
    }
}

float FuzzyRulesBase::leftDistanceRemain_normal(float distance)
{
    float X = distance;

    if ( X < 3.0 )
    {
        return 0.0;
    }
    else if ( X >= 3.0 && X < 4.0 )
    {
        // (3,0) -> (4,1)
        Point p1 = Point(3.0, 0.0);
        Point p2 = Point(4.0, 1.0);

        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.

        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 4.0 )
    {
        return 1.0;
    }
}

float FuzzyRulesBase::leftDistanceRemain_critical(float distance)
{
    float X = distance;

    if ( X < 2.0 )
    {
        return 1.0;
    }
    else if ( X >= 2.0 && X < 4.0 )
    {
        // (2,1) -> (4,0)
        Point p1 = Point(2.0, 1.0);
        Point p2 = Point(4.0, 0.0);

        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        float m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.

        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 4.0 )
    {
        return 0.0;
    }
}

float FuzzyRulesBase::rightDistanceRemain_normal(float distance)
{
    return leftDistanceRemain_normal(distance);
}

float FuzzyRulesBase::rightDistanceRemain_critical(float distance)
{
    return leftDistanceRemain_critical(distance);
}

float FuzzyRulesBase::getMuOfRule_ifXisA_and_YisB_atSinglePoint(WheelAngle WA, float Z)
{
    switch ( WA )
    {
        case WA_left_exLarge:
            return WheelAngle_left_exLarge(Z);
        case WA_left_large:
            return WheelAngle_left_large(Z);
        case WA_left_medium:
            return WheelAngle_left_medium(Z);
        case WA_left_little:
            return WheelAngle_left_little(Z);
        case WA_left_exLittle:
            return WheelAngle_left_exLittle(Z);
        case WA_right_exLarge:
            return WheelAngle_right_exLarge(Z);
        case WA_right_large:
            return WheelAngle_right_large(Z);
        case WA_right_medium:
            return WheelAngle_right_medium(Z);
        case WA_right_little:
            return WheelAngle_right_little(Z);
        case WA_right_exLittle:
            return WheelAngle_right_exLittle(Z);
            
        default:
            return -1;
    }
}

string FuzzyRulesBase::getDebugString(WheelAngle WA)
{
    switch ( WA )
    {
        case WA_left_exLarge:
            return "左 ＸＬ";
        case WA_left_large:
            return "左 Ｌ";
        case WA_left_medium:
            return "左 Ｍ";
        case WA_left_little:
            return "左 Ｓ";
        case WA_left_exLittle:
            return "左 ＸＳ";
        case WA_right_exLarge:
            return "右 ＸＬ";
        case WA_right_large:
            return "右 Ｌ";
        case WA_right_medium:
            return "右 Ｍ";
        case WA_right_little:
            return "右 Ｓ";
        case WA_right_exLittle:
            return "右 ＸＳ";
            
        default:
            return "";
    }
}

string FuzzyRulesBase::getDebugString_HD(HorizontalDeviate HD)
{
    switch ( HD )
    {
        case HD_right_little:
            return "微微偏右";
        case HD_right_normal:
            return "偏右";
        case HD_right_very:
            return "非常偏右";
        case HD_left_little:
            return "微微偏左";
        case HD_left_normal:
            return "偏左";
        case HD_left_very:
            return "非常偏左";

        default:
            return "";
    }
}

string FuzzyRulesBase::getDebugString_VDR(VerticalDistanceRemain VDR)
{
    switch ( VDR )
    {
        case VDR_large:
            return "前方剩餘空間足夠";
        case VDR_normal:
            return "前方剩餘空間不多";
        case VDR_critical:
            return "前方剩餘空間危急";

        default:
            return "";
    }
}

