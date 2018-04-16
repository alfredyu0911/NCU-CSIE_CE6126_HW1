//
//  trackInitializer.hpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/28.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef Initializer_hpp
#define Initializer_hpp

#include "AllLibrary.hpp"
#include "CarObject.hpp"
#include "Angle.hpp"

using namespace std;

typedef enum enInitStatue
{
    TI_NO_ERROR,
    TI_ERROR_TRACK_NOT_CLOSE,
    TI_ERROR_CAR_INIT_INFO,
    TI_ERROR_DESTINATION_INFO,
    TI_ERROR_TRACK_EMPTY
}initStatus;

class Initializer
{
public:
    Initializer();
    virtual ~Initializer();
    
    initStatus initCheck();
    
    Coordinate getCarInitCoordinate();
    Degree getCarInitAngle();
    
    vector<Point> getTrackInfo();
    vector<Point> getEndZoneInfo();
    
private:
    initStatus m_status;
    Coordinate m_carCoordinate;
    Degree m_carAngle;
    vector<Point> m_vTrackInfo;
    vector<Point> m_vEndZoneInfo;
    
    bool loadData();
    bool parserCarCoordinate(string str);
    bool parserEndZone(string strStart, string strEnd);
    Point stringToPoint(string strPoint, string strGap);
};

#endif /* Initializer_hpp */
