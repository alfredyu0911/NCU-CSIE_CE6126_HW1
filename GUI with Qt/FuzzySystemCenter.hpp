//
//  FuzzySystemCenter.hpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/30.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef FuzzySystemCenter_hpp
#define FuzzySystemCenter_hpp

#include "FuzzyRulesBase.hpp"

class FuzzySystemCenter
{
public:
    FuzzySystemCenter();
    ~FuzzySystemCenter();
    
    float FuzzySystemFinalResult(float sensorLeft, float sensorRight, float sensorFront);
    
protected:
    FuzzyRulesBase *m_rulesObj;
    
    float doInferenceAndDeFuzzy(float dHorizontalDeviate, float dVerticalDistanceRemain, float leftDistanceRemain, float rightDistanceRemain);
};

#endif /* FuzzySystemCenter_hpp */
