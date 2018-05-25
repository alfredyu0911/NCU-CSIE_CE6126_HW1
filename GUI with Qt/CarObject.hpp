//
//  CarObject.hpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/28.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef CarObject_hpp
#define CarObject_hpp

#include "Geometry.hpp"
#include "Angle.hpp"
#include "AllConstant.hpp"
#include "SystemDataCenter.h"
#include "FuzzyRulesBase.hpp"

using namespace std;

class CarObject
{
public:
    CarObject(SystemDataCenter *data, FuzzyRulesBase *rule)
    {
        m_data = data;
        m_rule = rule;
        m_lastFuzzyRuleResult_wheelAngle = 0.0;
    }

    virtual ~CarObject();
    
    /*
     自動駕駛系統進行判斷並控制
     此即模糊系統之部分
     */
    void autopilotSystemTackControl();
    
    /*
     車輛行進一個時間單位
     */
    void move();

    bool collisionCheck();

    float m_lastFuzzyRuleResult_wheelAngle;
    
protected:
    CarObject();
    SystemDataCenter *m_data;
    FuzzyRulesBase *m_rule;
};

#endif /* CarObject_hpp */
