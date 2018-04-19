//
//  CarObject.cpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/28.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#include "AllLibrary.hpp"

inline float distance(Point p1, Point p2) { return sqrt(pow(p2.x-p1.x, 2) + pow(p2.y-p1.y, 2)); }

CarObject::CarObject(float carDegree, Coordinate carCoordinate)
{
    m_length = CAR_INIT_LENGHT;
    m_wheelAngle = Degree(CAR_INIT_WHEEL_ANGLE);
    m_directionDegree = Degree(carDegree);
    m_selfLoaction = carCoordinate;
}

CarObject::~CarObject()
{
}

void CarObject::updateSensorLocation(Point pt)
{
    m_selfLoaction = pt;
}

void CarObject::sensorFront_receiveIntersectionPointSignal(Point pt)
{
    m_distance_front = distance(m_selfLoaction, pt);
}

void CarObject::sensorRight_receiveIntersectionPointSignal(Point pt)
{
    m_distance_right = distance(m_selfLoaction, pt);
}

void CarObject::sensorLeft_receiveIntersectionPointSignal(Point pt)
{
    m_distance_left = distance(m_selfLoaction, pt);
}

void CarObject::autopilotSystemTackControl()
{
    FuzzySystemCenter *sys = new FuzzySystemCenter();
    float deg = sys->FuzzySystemFinalResult(m_distance_left, m_distance_right, m_distance_front);
    
    Degree degree(deg);
    
    if ( deg < 0 )
    {
        wheelTurnLeft(degree);
        cout << "最終結論，向左：" << fixed << setprecision(5) << deg << "度" << endl;
        cout << "目前方向盤角度：" << fixed << setprecision(5) << m_wheelAngle.getDegree() << "度" << endl;
    }
    else
    {
        wheelTurnRight(degree);
        cout << "最終結論，向左：" << fixed << setprecision(5) << deg << "度" << endl;
        cout << "目前方向盤角度：" << fixed << setprecision(5) << m_wheelAngle.getDegree() << "度" << endl;
    }
}

void CarObject::move(Degree &carDegree, Coordinate &carCoordinate)
{
    double x = carCoordinate.x + cos(carDegree.getRadians() + m_wheelAngle.getRadians()) + sin(m_wheelAngle.getRadians()) * sin(carDegree.getRadians());
    
    double y = carCoordinate.y + sin(carDegree.getRadians() + m_wheelAngle.getRadians()) -
    sin(m_wheelAngle.getRadians()) * cos(carDegree.getRadians());
    
    carCoordinate.x = x;
    carCoordinate.y = y;
    carDegree.setAsRadians(carDegree.getRadians() - asin(2.0*sin(m_wheelAngle.getRadians()) / m_length ));
}

void CarObject::wheelTurnRight(Degree deg)
{
    Degree oldAngle = m_wheelAngle;
    double newDegree = oldAngle.getDegree() + deg.getDegree();
    if ( newDegree > WHEEL_ANGLE_RIGHT_MAX )
        newDegree = WHEEL_ANGLE_RIGHT_MAX;
    m_wheelAngle = Degree(newDegree);
}

void CarObject::wheelTurnLeft(Degree deg)
{
    Degree oldAngle = m_wheelAngle;
    double newDegree = oldAngle.getDegree() + deg.getDegree();
    if ( newDegree < WHEEL_ANGLE_LEFT_MAX )
        newDegree = WHEEL_ANGLE_LEFT_MAX;
    m_wheelAngle = Degree(newDegree);
}

float CarObject::getCarLeftSensorDegree(Degree currentDegree)
{
    return currentDegree.getDegree() + 45.0;
}

float CarObject::getCarRightSensorDegree(Degree currentDegree)
{
    return currentDegree.getDegree() - 45.0;
}




