//
//  FuzzySystemCenter.hpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/30.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef FuzzySystemCenter_hpp
#define FuzzySystemCenter_hpp

#include "AllLibrary.hpp"
#include "FuzzyRulesBase.hpp"

typedef struct stMassCenter
{
    double mu, point;
    stMassCenter(double dMu, double X)
    {
        mu = dMu;
        point = X;
    }
}MassCenter;

class FuzzySystemCenter
{
public:
    FuzzySystemCenter();
    ~FuzzySystemCenter();
    
    double FuzzySystemFinalResult(double sensorLeft, double sensorRight, double sensorFront);
    
protected:
    FuzzyRulesBase *m_rulesObj;
    
    double doInferenceAndDeFuzzy(double dHorizontalDeviate, double dVerticalDistanceRemain);
};

#endif /* FuzzySystemCenter_hpp */
