//
//  CarObject.hpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/28.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef CarObject_hpp
#define CarObject_hpp

#include "AllLibrary.hpp"
#include "Angle.hpp"
#include "AllConstant.hpp"
#include "Geometry.hpp"

using namespace std;

class CarObject
{
public:
    CarObject(float carDegree, Coordinate carCoordinate);
    virtual ~CarObject();
    
    Degree m_directionDegree;
    
    /*
     取得感測器的角度
     */
    float getCarRightSensorDegree(Degree currentDegree);
    float getCarLeftSensorDegree(Degree currentDegree);
    
    /*
     更新感測器(即車體)所在位置
     */
    void updateSensorLocation(Point pt);
    
    /*
     感測器接收來自軌道的訊號，也就是直線交點
     */
    void sensorFront_receiveIntersectionPointSignal(Point pt);
    void sensorRight_receiveIntersectionPointSignal(Point pt);
    void sensorLeft_receiveIntersectionPointSignal(Point pt);
    
    /*
     自動駕駛系統進行判斷並控制
     此即模糊系統之部分
     */
    void autopilotSystemTackControl();
    
    /*
     車輛行進一個時間單位
     */
    void move(Degree &carDegree, Coordinate &carCoordinate);
    
protected:
    CarObject();

    void wheelTurnRight(Degree degree);
    void wheelTurnLeft(Degree degree);
    
    float m_length;
    Degree m_wheelAngle;
    Point m_selfLoaction;
    float m_distance_front, m_distance_right, m_distance_left;
};

#endif /* CarObject_hpp */
