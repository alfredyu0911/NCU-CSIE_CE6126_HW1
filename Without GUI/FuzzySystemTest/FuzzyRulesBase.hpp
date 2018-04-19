//
//  FuzzyRulesBase.hpp
//  FuzzySystemTest
//
//  Created by Alfred Yu on 2018/4/4.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef FuzzyRulesBase_hpp
#define FuzzyRulesBase_hpp

#include "AllLibrary.hpp"

using namespace std;

typedef enum enHorizontalDeviate
{
    HD_right_little=0, HD_right_normal, HD_right_very, HD_left_little, HD_left_normal, HD_left_very,
    HD_enumEnd
}HorizontalDeviate;

typedef enum enVerticalDistanceRemain
{
    VDR_large=0, VDR_normal, VDR_critical,
    VDR_enumEnd
}VerticalDistanceRemain;

typedef enum enWheelAngle
{
    WA_left_exLarge=-5, WA_left_large=-4, WA_left_medium=-3, WA_left_little=-2, WA_left_exLittle=-1,
    WA_right_exLarge=5, WA_right_large=4, WA_right_medium=3, WA_right_little=2, WA_right_exLittle=1,
}WheelAngle;

typedef struct stFuzzyRulesResult
{
    HorizontalDeviate ConditionA_HD;
    VerticalDistanceRemain ConditionB_VDR;
    double alphaA, alphaB;
}FuzzyRulesResult;

class FuzzyRulesBase
{
public:
    /*
     由 input X & Y 比對符合的模糊規則(啟動強度為零者視為不符合)
     回傳所有符合的模糊規則，以及各自的啟動強度
     */
    vector<FuzzyRulesResult> searchAllRules(double X, double Y);
    
    /*
     取得模糊規則 if X is A && Y is B, then Z is C 的後鑑步 C
     */
    WheelAngle when_if_XisA_and_YisB(HorizontalDeviate ConditionA, VerticalDistanceRemain ConditionB);
    
    /*
     取得 WheelAngle 對應的的歸屬函數資訊
     可支援一般矩形、梯形、三角形
     目前定義為三角形
     */
    void getMembershipFuncOfWheelAngle(WheelAngle WA, double &center, double &width_L, double &width_R);
    
    /*
     取得特定模糊規則對應的後鍵步之歸屬度
     */
    double getMuOfRule_ifXisA_and_YisB_atSinglePoint(WheelAngle WA, double Z);

    string getDebugString(WheelAngle WA);
    
protected:
    // Condition A
    double horizontallyDeviate_right_little(double distance_of_left_Subtract_right);
    double horizontallyDeviate_right_normal(double distance_of_left_Subtract_right);
    double horizontallyDeviate_right_very(double distance_of_left_Subtract_right);
    double horizontallyDeviate_left_little(double distance_of_left_Subtract_right);
    double horizontallyDeviate_left_normal(double distance_of_left_Subtract_right);
    double horizontallyDeviate_left_very(double distance_of_left_Subtract_right);
    
    // Condition B
    double verticalDistanceRemain_critical(double distance);
    double verticalDistanceRemain_normal(double distance);
    double verticalDistanceRemain_large(double distance);
    
    // Condition C (as a result)
    double WheelAngle_left_exLarge(double degree);
    double WheelAngle_left_large(double degree);
    double WheelAngle_left_medium(double degree);
    double WheelAngle_left_little(double degree);
    double WheelAngle_left_exLittle(double degree);
    double WheelAngle_right_exLarge(double degree);
    double WheelAngle_right_large(double degree);
    double WheelAngle_right_medium(double degree);
    double WheelAngle_right_little(double degree);
    double WheelAngle_right_exLittle(double degree);
    
    double WheelAngle_generalize(double degree, double boundL, double boundR, double interval_LR);
};

#endif /* FuzzyRulesBase_hpp */
