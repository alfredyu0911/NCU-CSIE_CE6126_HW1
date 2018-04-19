//
//  Track.hpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/30.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef Track_hpp
#define Track_hpp

#include "AllLibrary.hpp"

class Track
{
public:
    Track();
    virtual ~Track();
    
    /*
     建立軌道資訊
     */
    void createTrack(vector<Point> vPt);
    
    /*
     置入車體物件
     */
    void putCarIntoTrack(CarObject *car);
    
    /*
     更新車輛位置、方向等資訊
     */
    void setupComplete_gettingStart();
    
protected:
    vector<Point> m_vTrackInfo;
    vector<Point> m_vEndZoneInfo;

    CarObject *m_car;
    Coordinate m_carCoordinate;
    Degree m_carDegree;
    
    Point getIntersectionPoint_SensorRight();
    Point getIntersectionPoint_SensorLeft();
    Point getIntersectionPoint_SensorFront();
    
    Point nextIntersectionPoint(float degree);
    bool checkLineIntersection(Point ptStart, Point ptEnd, Point car, Degree &carAngle, Point &ptIntersection);
};


#endif /* Track_hpp */
