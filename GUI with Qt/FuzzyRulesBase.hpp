//
//  FuzzyRulesBase.hpp
//  FuzzySystemTest
//
//  Created by Alfred Yu on 2018/4/4.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef FuzzyRulesBase_hpp
#define FuzzyRulesBase_hpp

#include <vector>
#include <string>
#include "Geometry.hpp"

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

typedef enum enLeftDistanceRemain
{
    LDR_normal=0, LDR_critical,
    LDR_enumEnd
}LeftDistanceRemain;

typedef enum enRightDistanceRemain
{
    RDR_normal=0, RDR_critical,
    RDR_enumEnd
}RightDistanceRemain;

typedef enum enWheelAngle
{
    WA_left_exLarge=-5, WA_left_large=-4, WA_left_medium=-3, WA_left_little=-2, WA_left_exLittle=-1,
    WA_right_exLarge=5, WA_right_large=4, WA_right_medium=3, WA_right_little=2, WA_right_exLittle=1,
}WheelAngle;

typedef struct stFuzzyRulesResult
{
    HorizontalDeviate ConditionA_HD;
    VerticalDistanceRemain ConditionB_VDR;
    float alphaA, alphaB;
}FuzzyRulesResult;

class FuzzyRulesBase
{
public:
    FuzzyRulesBase();

    /*
     比對符合的模糊規則(啟動強度為零者視為不符合)
     回傳所有符合的模糊規則，以及各自的啟動強度
     */
    vector<FuzzyRulesResult> searchAllRules(float X, float Y);
    
    /*
     取得模糊規則 if X is A && Y is B, then Z is C 的後鑑步 C
     */
    WheelAngle fuzzyRule_GetResultByCondition(HorizontalDeviate ConditionA, VerticalDistanceRemain ConditionB, LeftDistanceRemain ConditionL, RightDistanceRemain ConditionR);

    vector<HorizontalDeviate> search_if_XisA(float X);
    vector<VerticalDistanceRemain> search_if_YisB(float Y);
    
    /*
     取得 WheelAngle 對應的的歸屬函數資訊
     可支援一般矩形、梯形、三角形
     目前定義為三角形
     */
    void getMembershipFuncOfWheelAngle(WheelAngle WA, float &center, float &width_L, float &width_R);
    
    /*
     取得特定模糊規則對應的後鍵步之歸屬度
     */
    float getMuOfRule_ifXisA_and_YisB_atSinglePoint(WheelAngle WA, float Z);

    string getDebugString(WheelAngle WA);
    string getDebugString_HD(HorizontalDeviate HD);
    string getDebugString_VDR(VerticalDistanceRemain VDR);

    bool loadRuleSetting(string strPath);
    
protected:
    // Condition A
    float horizontallyDeviate_right_little(float distance_of_left_Subtract_right);
    float horizontallyDeviate_right_normal(float distance_of_left_Subtract_right);
    float horizontallyDeviate_right_very(float distance_of_left_Subtract_right);
    float horizontallyDeviate_left_little(float distance_of_left_Subtract_right);
    float horizontallyDeviate_left_normal(float distance_of_left_Subtract_right);
    float horizontallyDeviate_left_very(float distance_of_left_Subtract_right);
    
    // Condition B
    float verticalDistanceRemain_critical(float distance);
    float verticalDistanceRemain_normal(float distance);
    float verticalDistanceRemain_large(float distance);
    
    // Condition C (as a result)
    float WheelAngle_left_exLarge(float degree);
    float WheelAngle_left_large(float degree);
    float WheelAngle_left_medium(float degree);
    float WheelAngle_left_little(float degree);
    float WheelAngle_left_exLittle(float degree);
    float WheelAngle_right_exLarge(float degree);
    float WheelAngle_right_large(float degree);
    float WheelAngle_right_medium(float degree);
    float WheelAngle_right_little(float degree);
    float WheelAngle_right_exLittle(float degree);

    float getAlphaFromSetting(float distance_of_left_Subtract_right, vector<Point> vSetting);

public:
    // Condition L
    float leftDistanceRemain_normal(float distance);
    float leftDistanceRemain_critical(float distance);

    // Condition R
    float rightDistanceRemain_normal(float distance);
    float rightDistanceRemain_critical(float distance);
    
    float WheelAngle_generalize(float degree, float boundL, float boundR, float interval_LR);

private:
    vector<Point> vHD_L_l, vHD_L_n, vHD_L_v, vHD_R_l, vHD_R_n, vHD_R_v;
    vector<Point> vVDR_c, vVDR_n, vVDR_l;
    vector<Point> vWA_L_exL, vWA_L_L, vWA_L_M, vWA_L_S, vWA_L_exS;
    vector<Point> vWA_R_exS, vWA_R_S, vWA_R_M, vWA_R_L, vWA_R_exL;

    Point stringToPoint(string strPoint, string strGap);
    vector<Point> parserSetting(string strLine);

    bool m_bDefaultFlag;
};

#endif /* FuzzyRulesBase_hpp */
