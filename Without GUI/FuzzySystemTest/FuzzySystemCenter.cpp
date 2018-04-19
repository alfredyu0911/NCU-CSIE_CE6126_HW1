//
//  FuzzySystemCenter.cpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/30.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#include "AllLibrary.hpp"

FuzzySystemCenter::FuzzySystemCenter()
{
}

double FuzzySystemCenter::FuzzySystemFinalResult(double sensorLeft, double sensorRight, double sensorFront)
{
    double deviate = sensorLeft - sensorRight;
    return doInferenceAndDeFuzzy(deviate, sensorFront);
}

double FuzzySystemCenter::doInferenceAndDeFuzzy(double dHorizontalDeviate, double dVerticalDistanceRemain)
{
    FuzzyRulesBase *fRules = new FuzzyRulesBase();
    
    vector<FuzzyRulesResult> vRes = fRules->searchAllRules(dHorizontalDeviate, dVerticalDistanceRemain);
    vector<MassCenter> vMassCenter;
    vMassCenter.clear();
    
    for ( vector<FuzzyRulesResult>::iterator it=vRes.begin() ; it != vRes.end() ; it++ )
    {
        FuzzyRulesResult res1 = (*it);
        WheelAngle WA = fRules->when_if_XisA_and_YisB(res1.ConditionA_HD, res1.ConditionB_VDR);
        cout << "符合：" << fRules->getDebugString(WA) << "，強度：" << min(res1.alphaA, res1.alphaB) << endl;
        
        // 歸屬度最高的中心點(eg. 歸屬函數為三角形，center 為三角形頂點)，非底部中心點.
        double center;
        double Lwidth, Rwidth;
        fRules->getMembershipFuncOfWheelAngle(WA, center, Lwidth, Rwidth);
        
        double LBound = center - Lwidth;
        double RBound = center + Rwidth;
        double steps = 100;
        double dZ = fabs(LBound - RBound) / steps;
        double Z = LBound;
        
        // 交集取最小，為此模糊規則之啟動強度.
        double alpha_1 = min(res1.alphaA, res1.alphaB);
        
        // WA 為模糊規則推論結果，Z 為明確輸出結果，由對應的歸屬函數計算出 Z 點對應的歸屬度.
        double mu_1 = fRules->getMuOfRule_ifXisA_and_YisB_atSinglePoint(WA, Z);
        
        // 歸屬度不超過啟動強度.
        if ( mu_1 > alpha_1 )
            mu_1 = alpha_1;
        
        // 計算離散型的質量中心，作為去模糊化的過程.
        while ( Z < RBound )
        {
            double mu_2=0.0;
            // 製造後鍵步的圖形聯集.
            // 搜尋其他相符的模糊規則，在相符的模糊規則中，同一個 Z 座標點，取各個歸屬度之最大值.
            for ( vector<FuzzyRulesResult>::iterator it2=vRes.begin() ; it2 != vRes.end() ; it2++ )
            {
                if ( it == it2 )
                    continue;
                
                // 同上述，找出 Z 於 res2 對應的歸屬度.
                FuzzyRulesResult fRes2 = (*it2);
                double alpha_2 = min(fRes2.alphaA, fRes2.alphaB);
                mu_2 = fRules->getMuOfRule_ifXisA_and_YisB_atSinglePoint(fRules->when_if_XisA_and_YisB(fRes2.ConditionA_HD, fRes2.ConditionB_VDR), Z);
                if ( mu_2 > alpha_2 )
                    mu_2 = alpha_2;
            }
            
            // 圖形聯集，取最大為該點對應的歸屬度.
            vMassCenter.push_back(MassCenter(max(mu_1, mu_2), Z));
            Z += dZ;
        }
    }
    
    double sigma_mu=0.0, sigma_mu_mul_x=0.0;
    for ( vector<MassCenter>::iterator itm=vMassCenter.begin() ; itm != vMassCenter.end() ; itm++ )
    {
        MassCenter mc = (*itm);
        sigma_mu_mul_x += mc.point * mc.mu;
        sigma_mu += mc.mu;
    }
    
    if ( sigma_mu == 0.0 )
        return 0.0;
    
    delete fRules;
    cout << "去模糊化結果，質心：" << fixed << setprecision(5) << sigma_mu_mul_x / sigma_mu << endl;
    return sigma_mu_mul_x / sigma_mu;
}

FuzzySystemCenter::~FuzzySystemCenter()
{
}
