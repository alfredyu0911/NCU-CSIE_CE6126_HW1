//
//  FuzzyRulesBase.cpp
//  FuzzySystemTest
//
//  Created by Alfred Yu on 2018/4/4.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#include "AllLibrary.hpp"

vector<FuzzyRulesResult> FuzzyRulesBase::searchAllRules(double X, double Y)
{
    FuzzyRulesResult res;
    vector<FuzzyRulesResult> vRes;
    vRes.clear();
    
    for ( int ConditionA=0 ; ConditionA < HD_enumEnd ; ConditionA++ )
    {
        double alphaA=0.0, alphaB=0.0;
        
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

WheelAngle FuzzyRulesBase::when_if_XisA_and_YisB(HorizontalDeviate ConditionA, VerticalDistanceRemain ConditionB)
{
    //                  HD_right_little     HD_right_normal     HD_right_very       HD_left_little      HD_left_normal      HD_left_very
    //
    //  VDR_large       WA_left_exLittle    WA_left_little      WA_left_medium      WA_right_exLittle   WA_right_little     WA_right_medium
    //
    //  VDR_normal      WA_left_little      WA_left_medium      WA_left_large       WA_right_little     WA_right_medium     WA_right_large
    //
    //  VDR_critical    WA_left_medium      WA_left_large       WA_left_exLarge     WA_right_medium     WA_right_large      WA_right_exLarge

    WheelAngle RulesArray[3][6] =
    {
        {WA_left_exLittle, WA_left_little, WA_left_medium, WA_right_exLittle, WA_right_little, WA_right_medium},
        {WA_left_little, WA_left_medium, WA_left_large, WA_right_little, WA_right_medium, WA_right_large},
        {WA_left_medium, WA_left_large, WA_left_exLarge, WA_right_medium, WA_right_large, WA_right_exLarge}
    };
    
    return RulesArray[ConditionB][ConditionA];
}

void FuzzyRulesBase::getMembershipFuncOfWheelAngle(WheelAngle WA, double &center, double &width_L, double &width_R)
{
    double gap = (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX) / WHEEL_ANGLE_GAP_NUMBER;
    double temp = (double)WA * gap;
    double width = gap / 2.0;
    
    width_L = width_R = width;
    center = (temp > 0.0) ? (temp - width) : (temp + width);
}

double FuzzyRulesBase::horizontallyDeviate_left_little(double distance_of_left_Subtract_right)
{
    double X = distance_of_left_Subtract_right;
    
    if ( X < -6.0 )
    {
        return 0.0;
    }
    else if ( X >= -6.0 && X < -3.0 )
    {
        // (-6,0) -> (-3,1)
        Point p1 = Point(-6.0, 0.0);
        Point p2 = Point(-3.0, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else if ( X >= -3.0 && X < 0.0 )
    {
        return 1.0;
    }
    else// if ( X >= 0.0 )
    {
        return 0.0;
    }
}

double FuzzyRulesBase::horizontallyDeviate_left_normal(double distance_of_left_Subtract_right)
{
    double X = distance_of_left_Subtract_right;
    
    if ( X < -9.0 )
    {
        return 0.0;
    }
    else if ( X >= -9.0 && X < -6.0 )
    {
        // (-9,0) -> (-6,1)
        Point p1 = Point(-9.0, 0.0);
        Point p2 = Point(-6.0, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else if ( X >= -6.0 && X < -3.0 )
    {
        // (-6,1) -> (-3,0)
        Point p1 = Point(-6.0, 1.0);
        Point p2 = Point(-3.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= -3.0 )
    {
        return 0.0;
    }
}

double FuzzyRulesBase::horizontallyDeviate_left_very(double distance_of_left_Subtract_right)
{
    double X = distance_of_left_Subtract_right;
    
    if ( X < -9.0 )
    {
        return 1.0;
    }
    else if ( X >= -9.0 && X < -6.0 )
    {
        // (-9,1) -> (-6,0)
        Point p1 = Point(-9.0, 1.0);
        Point p2 = Point(-6.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= -6.0 )
    {
        return 0.0;
    }
}

double FuzzyRulesBase::horizontallyDeviate_right_little(double distance_of_left_Subtract_right)
{
    double X = distance_of_left_Subtract_right;
    
    if ( X < 0.0 )
    {
        return 0.0;
    }
    else if ( X >= 0.0 && X < 3.0 )
    {
        return 1.0;
    }
    else if ( X >= 3.0 && X < 6.0 )
    {
        // (3,1) -> (6,0)
        Point p1 = Point(3.0, 1.0);
        Point p2 = Point(6.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 6.0 )
    {
        return 0.0;
    }
}

double FuzzyRulesBase::horizontallyDeviate_right_normal(double distance_of_left_Subtract_right)
{
    double X = distance_of_left_Subtract_right;
    
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
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else if ( X >= 6.0 && X < 9.0 )
    {
        // (2,1) -> (3,0)
        Point p1 = Point(6.0, 1.0);
        Point p2 = Point(9.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 9.0 )
    {
        return 0.0;
    }
}

double FuzzyRulesBase::horizontallyDeviate_right_very(double distance_of_left_Subtract_right)
{
    double X = distance_of_left_Subtract_right;
    
    if ( X < 6.0 )
    {
        return 0.0;
    }
    else if ( X >= 6.0 && X < 9.0 )
    {
        // (6,0) -> (9,1)
        Point p1 = Point(6.0, 0.0);
        Point p2 = Point(9.0, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 9.0 )
    {
        return 1.0;
    }
}

double FuzzyRulesBase::verticalDistanceRemain_critical(double distance)
{
    double X = distance;
    
    if ( X < 5.0 )
    {
        return 1.0;
    }
    else if ( X >= 5.0 && X < 8.0 )
    {
        // (5,1) -> (8,0)
        Point p1 = Point(5.0, 1.0);
        Point p2 = Point(8.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 8.0 )
    {
        return 0.0;
    }
}

double FuzzyRulesBase::verticalDistanceRemain_normal(double distance)
{
    double X = distance;
    
    if ( X < 2.0 )
    {
        return 0.0;
    }
    else if ( X >= 2.0 && X < 5.0 )
    {
        // (2,0) -> (5,1)
        Point p1 = Point(2.0, 0.0);
        Point p2 = Point(5.0, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else if ( X >= 5.0 && X < 10.0 )
    {
        return  1.0;
    }
    else if ( X >= 10.0 && X < 13.0 )
    {
        // (10,1) -> (13,0)
        Point p1 = Point(10.0, 1.0);
        Point p2 = Point(13.0, 0.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 13.0 )
    {
        return 0.0;
    }
}

double FuzzyRulesBase::verticalDistanceRemain_large(double distance)
{
    double X = distance;
    
    if ( X < 12.0 )
    {
        return 0.0;
    }
    else if ( X >= 12.0 && X < 15.0 )
    {
        // (12,0) -> (15,1)
        Point p1 = Point(12.0, 0.0);
        Point p2 = Point(15.0, 1.0);
        
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (X - p1.x) + p1.y;
    }
    else// if ( X >= 15.0 )
    {
        return 1.0;
    }
}

double FuzzyRulesBase::WheelAngle_left_exLarge(double degree)
{
    return WheelAngle_generalize(degree, -40, -32, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

double FuzzyRulesBase::WheelAngle_left_large(double degree)
{
    return WheelAngle_generalize(degree, -32, -24, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

double FuzzyRulesBase::WheelAngle_left_medium(double degree)
{
    return WheelAngle_generalize(degree, -24, -16, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

double FuzzyRulesBase::WheelAngle_left_little(double degree)
{
    return WheelAngle_generalize(degree, -16, -8, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

double FuzzyRulesBase::WheelAngle_left_exLittle(double degree)
{
    return WheelAngle_generalize(degree, -8, 0, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

double FuzzyRulesBase::WheelAngle_right_exLittle(double degree)
{
    return WheelAngle_generalize(degree, 0, 8, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

double FuzzyRulesBase::WheelAngle_right_little(double degree)
{
    return WheelAngle_generalize(degree, 8, 16, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

double FuzzyRulesBase::WheelAngle_right_medium(double degree)
{
    return WheelAngle_generalize(degree, 16, 24, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

double FuzzyRulesBase::WheelAngle_right_large(double degree)
{
    return WheelAngle_generalize(degree, 24, 32, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

double FuzzyRulesBase::WheelAngle_right_exLarge(double degree)
{
    return WheelAngle_generalize(degree, 32, 40, (WHEEL_ANGLE_RIGHT_MAX - WHEEL_ANGLE_LEFT_MAX)/WHEEL_ANGLE_GAP_NUMBER/2);
}

double FuzzyRulesBase::WheelAngle_generalize(double degree, double boundL, double boundR, double intervalLR)
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
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
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
        double m = (p2.y - p1.y) / (p2.x - p1.x);   // 斜率.
        
        return m * (degree - p1.x) + p1.y;
    }
    else// if ( degree >= boundR + intervalLR )
    {
        return 0.0;
    }
}

double FuzzyRulesBase::getMuOfRule_ifXisA_and_YisB_atSinglePoint(WheelAngle WA, double Z)
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

