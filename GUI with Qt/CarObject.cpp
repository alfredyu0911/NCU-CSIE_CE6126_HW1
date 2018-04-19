//
//  CarObject.cpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/28.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#include <QDebug>
#include "AllConstant.hpp"
#include "CarObject.hpp"
#include "FuzzySystemCenter.hpp"

CarObject::CarObject()
{
}

CarObject::~CarObject()
{
}

void CarObject::autopilotSystemTackControl()
{
    Geometry geoObj;
    float dL = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_L) - CAR_INIT_LENGHT;
    float dR = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_R) - CAR_INIT_LENGHT;
    float dF = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_F) - CAR_INIT_LENGHT;

    FuzzySystemCenter *sys = new FuzzySystemCenter();
    m_lastFuzzyRuleResult_wheelAngle = sys->FuzzySystemFinalResult(dL, dR, dF);
    
    Degree degree(m_lastFuzzyRuleResult_wheelAngle);
    
    if ( m_lastFuzzyRuleResult_wheelAngle < 0 )
    {
        m_data->carTurnLeft(degree);
    }
    else
    {
        m_data->carTurnRight(degree);
    }

    delete sys;
}

void CarObject::move()
{
    float Theta=m_data->m_carWheelAngle.getRadians(), Phi=m_data->m_carAngle.getRadians();
    float x=m_data->m_carPosition.x, y=m_data->m_carPosition.y;

    x = x + cos(Theta + Phi) + (sin(Theta) * sin(Phi));
    y = y + sin(Theta + Phi) - (sin(Theta) * cos(Phi));
    Phi = Phi - asin(2.0 * sin(Theta) / CAR_INIT_LENGHT );
    
    m_data->m_carPosition.x = x;
    m_data->m_carPosition.y = y;
    m_data->m_carAngle.setAsRadians(Phi);
}




